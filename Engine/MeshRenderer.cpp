#include "pch.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : Renderer(eComponentType::Renderer)
{

}

MeshRenderer::~MeshRenderer()
{
	
}

void MeshRenderer::Render()
{
	Renderer::Render();

}
