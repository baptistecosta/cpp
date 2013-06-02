/*
	OpenCL wrapper	::	2013
	Baptiste Costa
*/

#ifndef __OPENCL_WRAPPER__
#define __OPENCL_WRAPPER__

	#include <CL/cl.h>
	#include "core/string.h"

#define __CL_DEBUG

#ifdef __CL_DEBUG
#define	__CL_CALL(_C_)	{	int err = _C_; OpenCLWrapper::CheckCLErr(err);	}
#else
#define __CL_CALL(_C_)	_C_;
#endif

namespace owl
{

class	OpenCLWrapper
{
		cl_uint			num_platforms,
						num_devices;
		cl_platform_id*	platform_ids;
		cl_device_id*	device_ids;
		cl_context		ctx;
		cl_program		prog;
		cl_kernel		kernel;
		cl_command_queue queue;

		String			kernel_src_code;

		int				err;

public:

		OpenCLWrapper(const char* source_code);
		~OpenCLWrapper();

		void			Init(cl_platform_id, const char* kernel_name);

		void			GetPlatformIds();
		void			GetDeviceIds(cl_platform_id);
		void			CreateCLContext(cl_platform_id);
		void			CreateProgram();
		void			BuildProgram();
		void			CreateKernel(const char* name);
		void			CreateCommandQueue();

		cl_mem			CreateBuffer(cl_mem_flags, size_t, void*);
		void			SetKernelArg(cl_uint arg_index, size_t arg_size, const void* arg_value);
		void			EnqueueTask();
		void			EnqueueNDRangeKernel(cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event*);
		void			EnqueueReadBuffer(cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event);

		void			LogPlatformInfo();
		void			LogProgramBuildInfo();

		const cl_platform_id& GetPlatformId(int index)		{	return platform_ids[index];	}
		const cl_context& GetContext()						{	return ctx;	}
		const cl_kernel& GetKernel()						{	return kernel;	}
		const cl_command_queue& GetCommandQueue()			{	return queue;	}

static	void			CheckCLErr(const cl_int err);
};

}		// owl
#endif	// __OPENCL_WRAPPER__