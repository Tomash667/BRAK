#include "Pch.h"
#include "Core.h"
#include "Mesh.h"
#include <d3d11.h>

Mesh::Mesh() : vb(nullptr), ib(nullptr)
{

}

Mesh::~Mesh()
{
	if(vb)
		vb->Release();
	if(ib)
		ib->Release();
}
