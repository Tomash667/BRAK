#pragma once

struct SceneNode;

struct Camera
{
	SceneNode* target;
	Vec3 vfrom, vto;
	Vec2 rot, vrot;
	float dist, vdist, h, springiness;
	bool reset;

	Camera() : target(nullptr), vfrom(-5, 5, -5), vto(0, 0, 0), reset(true), springiness(40.f), dist(3.f)
	{

	}

	void Update(float dt);
	Matrix GetViewMatrix();
};
