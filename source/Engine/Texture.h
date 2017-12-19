#pragma once

#include "Resource.h"

struct ID3D11Resource;

struct Texture : Resource
{
	ID3D11Resource* tex;
};
