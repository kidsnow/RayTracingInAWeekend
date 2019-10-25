#pragma once

#include "ray.h"
#include "hitable.h"
#include "glm/glm.hpp"
#include "material.h"

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(glm::vec3 cen, float r, Material* mPtr) : center(cen), radius(r), matPtr(mPtr) {};
	virtual bool Hit(const Ray &r, float tMin,
		float tMax, HitRecord& rec) const;

public:
	glm::vec3 center;
	float radius;
	Material* matPtr;
};

bool Sphere::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	glm::vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a*c;

	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}

	return false;
}