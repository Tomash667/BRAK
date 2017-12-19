#pragma once

struct Resource
{
	enum Type
	{
		Mesh,
		Texture
	};

	Type type;
	string name;

	virtual ~Resource() {}
};
