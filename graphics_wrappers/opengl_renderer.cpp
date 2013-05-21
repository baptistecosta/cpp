/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "core/log.h"
	#include "resources/material.h"
	#include "resources/text2d.h"
	#include "resources/shader.h"
	#include "resources/rendering_context.h"
	#include "resources/rendering_geometry.h"
	#include "opengl.h"
	#include "opengl_renderer.h"

	using namespace owl;
	using namespace owl::math;

//-----------------------------------------------------------------------------
void			GLRenderer::Draw(Shader& shader, RenderingContext& context)
{
	__GL_CALL(glUseProgram(shader.GetProgram()))

	shader.SetUniforms(context);

	RenderingGeometry& rg = context.GetRenderingGeometry();
	BindArrayBufferAndAttribPointer(rg.GetBuffer(0), 0, 3, GL_FLOAT);
	BindArrayBufferAndAttribPointer(rg.GetBuffer(1), 1, 2, GL_FLOAT);
	BindArrayBufferAndAttribPointer(rg.GetBuffer(2), 2, 3, GL_FLOAT);
	BindArrayBufferAndAttribPointer(rg.GetBuffer(3), 3, 3, GL_FLOAT);
	BindArrayBufferAndAttribPointer(rg.GetBuffer(4), 4, 3, GL_FLOAT);
	BindArrayBufferAndAttribIPointer(rg.GetBuffer(5), 5, 4, GL_INT);
	BindArrayBufferAndAttribPointer(rg.GetBuffer(6), 6, 4, GL_FLOAT);
	BindAndDrawElement(rg.GetBuffer(7), 7, GL_TRIANGLES, rg.GetVertices().indices.size(), GL_UNSIGNED_SHORT);

	// Disable vertex attrib arrays.
	int n_buff = Max3DBuffer - 1;
	for (int i = 0; i < n_buff; i++)
		if (rg.GetBuffer(i))
			__GL_CALL(glDisableVertexAttribArray(i))
}

//-----------------------------------------------------------------------------
void			GLRenderer::DrawLines(RenderingGeometry* rg)
{
	BindArrayBufferAndAttribPointer(rg->GetBuffer(0), 0, 3, GL_FLOAT);
	__GL_CALL(glDrawArrays(GL_LINES, 0, rg->GetVertices().pos.size()))
}

//-----------------------------------------------------------------------------
void			GLRenderer::DrawText(Shader& shader, RenderingContext& context)
{
	__GL_CALL(glUseProgram(shader.GetProgram()))

	Text2D& text_2d = context.GetText2D();
 	OpenGL::BindTexture(text_2d.GetFont()->m_texture->GetGLTextureName(), GL_TEXTURE0);
	text_2d.Bind();

	context.SetTextureUnit(0);
	shader.SetUniforms(context);
 	
	BindArrayBufferAndAttribPointer(text_2d.GetBuffer(0), 0, 2, GL_FLOAT);
	BindArrayBufferAndAttribPointer(text_2d.GetBuffer(1), 1, 2, GL_FLOAT);

	__GL_CALL(glEnable(GL_BLEND))
	__GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
	__GL_CALL(glDrawArrays(GL_TRIANGLES, 0, text_2d.GetSize()))
	__GL_CALL(glDisable(GL_BLEND))

	for (int i = 0; i < Max2DBuffer; i++)
		__GL_CALL(glDisableVertexAttribArray(i))
}

//-----------------------------------------------------------------------------
void			GLRenderer::BindArrayBufferAndAttribPointer(uint buffer, uint index, int size, uint type, uchar normalized, int stride, const void* pointer)
{
	if (index >= 0 && index < Max3DBuffer)
	{
		__GL_CALL(glEnableVertexAttribArray(index))
		__GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer))
		// Specify the location and data format of the array of vertex attributes at index "index" to use when rendering.
		__GL_CALL(glVertexAttribPointer(index, size, type, normalized, stride, pointer))
	}
}

//-----------------------------------------------------------------------------
void			GLRenderer::BindArrayBufferAndAttribIPointer(uint buffer, uint index, int size, uint type, int stride, const void* pointer)
{
	if (index >= 0 && index < Max3DBuffer)
	{
		__GL_CALL(glEnableVertexAttribArray(index))
		__GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer))
		__GL_CALL(glVertexAttribIPointer(index, size, type, stride, pointer))
	}
}

//-----------------------------------------------------------------------------
void			GLRenderer::BindAndDrawElement(uint buffer, uint index, uint mode, int count, uint type, const void* indices)
{
	if (index >= 0 && index < Max3DBuffer)
	{
		__GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer))
		__GL_CALL(glDrawElements(mode, count, type, indices))
	}
}

//-----------------------------------------------------------------------------
RenderingGeometry* GLRenderer::ConvertToRenderingGeometry(Geometry* geom)
{
	uint size = geom->vertices.Size();
	if (size == 0)
		return null_ptr;

	RenderingGeometry* rdr_geom = new RenderingGeometry(geom->getGuid());
	RenderingVertices* rdr_vert = &rdr_geom->GetVertices();

	uint index = 0;
	for (uint i = 0; i < size; i++)
	{
		if (GetSimilarVertexIndex(geom->vertices[i], rdr_vert, index))
		{
			rdr_vert->indices.push_back(index);
			rdr_vert->tang[index] = geom->vertices[i]->tang;
			rdr_vert->btan[index] = geom->vertices[i]->btan;
		}
		else
		{
			rdr_vert->pos.push_back(geom->vertices[i]->pos);
			rdr_vert->uv.push_back(geom->vertices[i]->uv);
			rdr_vert->nor.push_back(geom->vertices[i]->nor);
			rdr_vert->tang.push_back(geom->vertices[i]->tang);
			rdr_vert->btan.push_back(geom->vertices[i]->btan);
			rdr_vert->joints.push_back(geom->vertices[i]->weights);
			rdr_vert->weights.push_back(geom->vertices[i]->weights);
			rdr_vert->indices.push_back(rdr_vert->pos.size() - 1);
		}
	}
	return rdr_geom;
}

//-----------------------------------------------------------------------------
bool			GLRenderer::GetSimilarVertexIndex(Vertex* vertex, RenderingVertices* rdr_vert, uint& index)
{
	for (uint i = 0; i < rdr_vert->pos.size(); i++)
	{	
		if	(	
				IsEqual(rdr_vert->pos[i].x, vertex->pos.x)
			&&	IsEqual(rdr_vert->pos[i].y, vertex->pos.y)
			&&	IsEqual(rdr_vert->pos[i].z, vertex->pos.z)
			&&	IsEqual(rdr_vert->uv[i].x, vertex->uv.x)
			&&	IsEqual(rdr_vert->uv[i].y, vertex->uv.y)
			&&	IsEqual(rdr_vert->nor[i].x, vertex->nor.x)
			&&	IsEqual(rdr_vert->nor[i].y, vertex->nor.y)
			&&	IsEqual(rdr_vert->nor[i].z, vertex->nor.z)
			&&	IsEqual((float)rdr_vert->joints[i].j[0], (float)vertex->weights[0].jid)
			&&	IsEqual((float)rdr_vert->joints[i].j[1], (float)vertex->weights[1].jid)
			&&	IsEqual((float)rdr_vert->joints[i].j[2], (float)vertex->weights[2].jid)
			&&	IsEqual((float)rdr_vert->joints[i].j[3], (float)vertex->weights[3].jid)
			&&	IsEqual(rdr_vert->weights[i].w[0], vertex->weights[0].w)
			&&	IsEqual(rdr_vert->weights[i].w[1], vertex->weights[1].w)
			&&	IsEqual(rdr_vert->weights[i].w[2], vertex->weights[2].w)
			&&	IsEqual(rdr_vert->weights[i].w[3], vertex->weights[3].w)
			)
		{	
			index = i;
			return true;
		}
	}
	return false;
}
