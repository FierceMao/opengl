#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Addbuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);

	void Bind() const;
	void UnBind() const;
};