#pragma once

#include "ray.h"
#include "hitable.h"
#include "glm/glm.hpp"

glm::vec3 RandomInUnitSphere();

class Material {
public:
	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec,
		glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const glm::vec3& a) : albedo(a) {}
	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec,
		glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	glm::vec3 albedo;
};

glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n)
{
	return v - 2 * glm::dot(v, n) * n;
}

class Metal : public Material {
public:
	Metal(const glm::vec3& a, float f) : albedo(a)
	{
		if (f < 1)
			fuzz = f;
		else
			fuzz = 1;
	}
	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec,
		glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 reflected = Reflect(glm::normalize(rayIn.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	glm::vec3 albedo;
	float fuzz;
};