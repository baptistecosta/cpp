/*
	OpenCL wrapper	::	2013
	Baptiste Costa
*/

	#include <stdlib.h>
	#include "wrapper.h"
	#include "core/log.h"

using namespace owl;

//---------------------------------------------------------------------------
OpenCLWrapper::OpenCLWrapper(const char* source_code)
	: num_platforms(0)
	, num_devices(0)
	, platform_ids(NULL)
	, device_ids(NULL)
	, ctx(NULL)
	, prog(NULL)
	, kernel(NULL)
	, queue(NULL)
	, kernel_src_code(source_code)
{
	//
}

//---------------------------------------------------------------------------
OpenCLWrapper::~OpenCLWrapper()
{
	clFlush(queue);
	clFinish(queue);
	clReleaseKernel(kernel);
	clReleaseProgram(prog);
	clReleaseCommandQueue(queue);
	clReleaseContext(ctx);

	delete [] device_ids;
	delete [] platform_ids;
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::Init(cl_platform_id platform_id, const char* kernel_name)
{
	GetDeviceIds(platform_id);
	CreateCLContext(platform_id);
	CreateProgram();
	BuildProgram();
	CreateKernel(kernel_name);
	CreateCommandQueue();
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::GetPlatformIds()
{
	__CL_CALL(clGetPlatformIDs(0, NULL, &num_platforms))
	platform_ids = new cl_platform_id[num_platforms];
	__CL_CALL(clGetPlatformIDs(num_platforms, platform_ids, NULL))
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::GetDeviceIds(cl_platform_id platform_id)
{
	__CL_CALL(clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices))
	device_ids = new cl_device_id[num_devices];
	__CL_CALL(clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, num_devices, device_ids, NULL))
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::CreateCLContext(cl_platform_id platform_id)
{
	const cl_context_properties ctx_prop[] =
	{
		CL_CONTEXT_PLATFORM,
		reinterpret_cast<cl_context_properties>(platform_id),
		0,
	};
	ctx = clCreateContext(ctx_prop, num_devices, device_ids, NULL, NULL, &err);
	CheckCLErr(err);
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::CreateProgram()
{
	const char* src = kernel_src_code.cStr();
	size_t src_len = (size_t)kernel_src_code.Size();
	prog = clCreateProgramWithSource(ctx, 1, &src, &src_len, &err);
	CheckCLErr(err);
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::BuildProgram()
{	__CL_CALL(clBuildProgram(prog, 1, device_ids, NULL, NULL, NULL))	}

//---------------------------------------------------------------------------
void			OpenCLWrapper::LogProgramBuildInfo()
{
	size_t len = 0;
	char buf[512] = {0};
	__CL_CALL(clGetProgramBuildInfo(prog, device_ids[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &len))
		__CL_CALL(clGetProgramBuildInfo(prog, device_ids[0], CL_PROGRAM_BUILD_LOG, len, buf, NULL))
		Log::e(buf);
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::CreateKernel(const char* name)
{
	kernel = clCreateKernel(prog, name, &err);
	CheckCLErr(err);
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::CreateCommandQueue()
{
	queue = clCreateCommandQueue(ctx, device_ids[0], 0, &err);
	CheckCLErr(err);
}

//---------------------------------------------------------------------------
cl_mem			OpenCLWrapper::CreateBuffer(cl_mem_flags flags, size_t size, void* host_ptr)
{
	cl_mem buf = clCreateBuffer(ctx, flags, size, host_ptr, &err);
	CheckCLErr(err);
	return buf;
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::SetKernelArg(cl_uint arg_index, size_t arg_size, const void* arg_value)
{	__CL_CALL(clSetKernelArg(kernel, arg_index, arg_size, arg_value))	}

//---------------------------------------------------------------------------
void			OpenCLWrapper::EnqueueTask()
{	__CL_CALL(clEnqueueTask(queue, kernel, 0, NULL, NULL))	}

//---------------------------------------------------------------------------
void			OpenCLWrapper::EnqueueNDRangeKernel	(
														cl_uint work_dim,
														const size_t* global_work_offset,
														const size_t* global_work_size,
														const size_t* local_work_size,
														cl_uint num_events_in_wait_list,
														const cl_event* event_wait_list,
														cl_event* event
													)
{
	__CL_CALL(clEnqueueNDRangeKernel(
		queue,						// A valid command-queue. The kernel will be queued for execution on the device associated with command_queue.
		kernel,						// A valid kernel object. The OpenCL context associated with kernel and command_queue must be the same.
		work_dim,					// The number of dimensions used to specify the global work-items and work-items in the work-group. work_dim must be greater than zero and less than or equal to three. 
		global_work_offset,			// global_work_offset
		global_work_size,			// Points to an array of work_dim unsigned values that describe the number of global work-items in work_dim dimensions that will execute the kernel function.
		local_work_size,			// Points to an array of work_dim unsigned values that describe the number of work-items that make up a work-group that will execute the kernel specified by kernel.
		num_events_in_wait_list,	// event_wait_list
		event_wait_list,			// num_events_in_wait_list
		event						// Returns an event object that identifies this particular kernel execution instance. 
	))
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::EnqueueReadBuffer(cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
{	__CL_CALL(clEnqueueReadBuffer(queue, buffer, blocking_read, offset, size, ptr, num_events_in_wait_list, event_wait_list, event))	}

//---------------------------------------------------------------------------
void			OpenCLWrapper::LogPlatformInfo()
{
	// Get platform info
	for (unsigned int i = 0; i < num_platforms; i++)
	{
		char version[64] = {0}, name[64] = {0}, vendor[64] = {0}, profile[64] = {0};
		__CL_CALL(clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VERSION, sizeof(version), &version, NULL));
		__CL_CALL(clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, sizeof(name), &name, NULL));
		__CL_CALL(clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, sizeof(vendor), &vendor, NULL));
		__CL_CALL(clGetPlatformInfo(platform_ids[i], CL_PLATFORM_PROFILE, sizeof(profile), &profile, NULL));
		Log::HorizontalLine();
		Log::i("#%d", i);
		Log::i("Platform ID:        %d", platform_ids[i]);
		Log::i("Version:            %s", version);
		Log::i("Name:               %s", name);
		Log::i("Vendor:             %s", vendor);
		Log::i("Profile:            %s", profile);

		// Get device ids
		cl_uint num_devices = 0;
		__CL_CALL(clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices))
			cl_device_id dev_ids[16] = {0};
		__CL_CALL(clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, num_devices, dev_ids, NULL))

			Log::i("Number of devices:  %d\n", num_devices);

		// Get device info
		for (unsigned int j = 0; j < num_devices; j++)
		{
			char dev_name[64] = {0};
			cl_device_type type = 0;
			cl_uint max_compute_unit = 0;
			cl_ulong max_mem_alloc_size = 0;
			cl_bool endian_little = true;

			__CL_CALL(clGetDeviceInfo(dev_ids[j], CL_DEVICE_NAME, sizeof(name), &dev_name, NULL));
			__CL_CALL(clGetDeviceInfo(dev_ids[j], CL_DEVICE_TYPE, sizeof(type), &type, NULL));
			__CL_CALL(clGetDeviceInfo(dev_ids[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_unit), &max_compute_unit, NULL));
			__CL_CALL(clGetDeviceInfo(dev_ids[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(max_mem_alloc_size), &max_mem_alloc_size, NULL));
			__CL_CALL(clGetDeviceInfo(dev_ids[j], CL_DEVICE_ENDIAN_LITTLE, sizeof(endian_little), &endian_little, NULL));

			Log::i("    #%d", j);
			Log::i("    Device ID:              %d", dev_ids[j]);
			Log::i("    Name:                   %s", dev_name);
			if		(type == CL_DEVICE_TYPE_CPU)
				Log::i("    Type:                   CL_DEVICE_TYPE_CPU");
			else if (type == CL_DEVICE_TYPE_GPU)
				Log::i("    Type:                   CL_DEVICE_TYPE_GPU");
			else if (type == CL_DEVICE_TYPE_ACCELERATOR)
				Log::i("    Type:                   CL_DEVICE_TYPE_ACCELERATOR");
			else if (type == CL_DEVICE_TYPE_DEFAULT)
				Log::i("    Type:                   CL_DEVICE_TYPE_DEFAULT");
			Log::i("    Max compute units:      %d", max_compute_unit);
			Log::i("    Max memory allocation:  %d", max_mem_alloc_size);
			Log::i("    Device endianess:       %s", endian_little ? "little": "big");
		}
	}
}

//---------------------------------------------------------------------------
void			OpenCLWrapper::CheckCLErr(const cl_int err)
{
	if (err != CL_SUCCESS)
	{
		switch (err)
		{
			case CL_INVALID_VALUE:
				Log::e("CL_INVALID_VALUE");
				break;
			case CL_BUILD_PROGRAM_FAILURE:
				Log::e("CL_BUILD_PROGRAM_FAILURE");
				break;
			case CL_INVALID_PLATFORM:
				Log::e("CL_INVALID_PLATFORM");
				break;
			case CL_INVALID_HOST_PTR:
				Log::e("CL_INVALID_HOST_PTR");
				break;
			case CL_INVALID_PROGRAM_EXECUTABLE:
				Log::e("CL_INVALID_PROGRAM_EXECUTABLE");
				break;
			case CL_INVALID_KERNEL_NAME:
				Log::e("CL_INVALID_KERNEL_NAME");
				break;
			case CL_INVALID_KERNEL_ARGS:
				Log::e("CL_INVALID_KERNEL_ARGS");
				break;
			default:
				Log::e(String::Format("number = %d", err));
				break;
		}
		exit(-1);
	}
}