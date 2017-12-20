#pragma once

#include "Resource.h"

struct ID3D11ShaderResourceView;

struct Texture : Resource
{
	~Texture();

	ID3D11ShaderResourceView* tex;
};
