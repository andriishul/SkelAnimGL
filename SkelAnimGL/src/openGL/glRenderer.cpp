#include "glRenderer.hpp"
#include <glad/glad.h>
#include <utils/Logger.hpp>

void glRenderer::DrawIndexed(VertexArray& vao)
{
	vao.Bind();
	if (!vao.GetIndexBuffer()) LOG_ERROR("IndexBuffer isnot set.");
	int count = vao.GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	vao.Unbind();
}
