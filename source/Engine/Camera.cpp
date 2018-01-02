#include "Pch.h"
#include "Core.h"
#include "Camera.h"
#include "SceneNode.h"

void Camera::Update(float dt)
{
	if(!target)
		return;

	float d;
	Vec3 to = target->pos;
	to.y += h;

	if(reset)
	{
		d = 1.f;
		vto = to;
		vrot = rot;
		vdist = dist;
	}
	else
	{
		d = 1.0f - exp(log(0.5f) * springiness * dt);
		vrot = Vec2::Slerp(vrot, rot, d).Clip();
		vdist += (dist - vdist) * d;
		vto += (to - vto) * d;
	}

	Vec3 ray(0, -vdist, 0);
	Matrix mat = Matrix::Rotation(vrot.x, vrot.y, 0);
	ray = Vec3::Transform(ray, mat);
	Vec3 from = to + ray;
	vfrom += (from - vfrom) * d;

	reset = false;
}

Matrix Camera::GetViewMatrix()
{
	return Matrix::CreateLookAt(vfrom, vto, Vec3(0, 1, 0));
}
