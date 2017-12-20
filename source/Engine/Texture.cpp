#include "Pch.h"
#include "Core.h"
#include "Texture.h"
#include <d3d11.h>

Texture::~Texture()
{
	if(tex)
		tex->Release();
}
