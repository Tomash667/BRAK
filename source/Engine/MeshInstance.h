#pragma once

#include "Mesh.h"

struct MeshInstance
{
	enum FLAGS
	{
		FLAG_PLAYING = 1 << 0,
		FLAG_ONCE = 1 << 1,
		FLAG_BACK = 1 << 2,
		FLAG_GROUP_ACTIVE = 1 << 3,
		FLAG_BLENDING = 1 << 4,
		FLAG_STOP_AT_END = 1 << 5,
		FLAG_BLEND_WAIT = 1 << 6,
		FLAG_RESTORE = 1 << 7,
		FLAG_UPDATED = 1 << 8
	};

	struct Group
	{
		Group() : anim(nullptr), state(0), speed(1.f), prio(0), blend_max(0.33f)
		{
		}

		Mesh::Animation* anim;
		float time, speed, blend_time, blend_max;
		int state, prio, used_group;
	};

	MeshInstance(Mesh* mesh);
	void Update(float dt);
	void Play(Mesh::Animation* anim, int flags, int group);
	void Play(cstring name, int flags, int group)
	{
		Play(mesh->GetAnimation(name), flags, group);
	}
	void SetupBones();

	Mesh* mesh;
	vector<Matrix> mat_bones;
	vector<Mesh::KeyframeBone> blendb;
	vector<Group> groups;
	bool need_update;
};
