#pragma once

#include "ray.h"

struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
};

class Hitable
{
public:
	virtual bool Hit(const Ray& r, float tMin,
		float tMax, HitRecord& rec) const = 0;
};