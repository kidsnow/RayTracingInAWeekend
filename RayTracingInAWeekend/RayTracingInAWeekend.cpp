#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "ray.h"

float HitSphere(const glm::vec3 center, float radius, const Ray& r)
{
	glm::vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.0f * a*c;

	if (discriminant < 0)
	{
		return -1.0f;
	}
	
	return (-b - sqrt(discriminant)) / (2.0f * a);
}

glm::vec3 Color(const Ray& r)
{
	float t = HitSphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, r);
	if (t > 0.0f)
	{
		glm::vec3 N = normalize(r.PointAtParameter(t) - glm::vec3(0.0f, 0.0f, -1.0f));
		return 0.5f * glm::vec3(N.x + 1, N.y + 1, N.z + 1);
	}

	glm::vec3 unitDirection = normalize(r.direction());
	t = 0.5*(unitDirection.y + 1.0f);
	return (1.0f - t)*glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	std::ofstream outImage("raytraced.ppm");

	int nx = 200;
	int ny = 100;
	outImage << "P3\n" << nx << " " << ny << "\n255\n";
	glm::vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
	glm::vec3 vertical(0.0f, 2.0f, 0.0f);
	glm::vec3 origin(0.0f, 0.0f, 0.0f);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			glm::vec3 col = Color(r);
			int ir = int(255.99f*col.r);
			int ig = int(255.99f*col.g);
			int ib = int(255.99f*col.b);

			outImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	outImage.close();

	return 0;
}