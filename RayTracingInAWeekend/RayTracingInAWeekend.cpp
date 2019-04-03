#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "ray.h"

bool hitSphere(const glm::vec3 center, float radius, const Ray& r)
{
	glm::vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.0f * a*c;
	return (discriminant > 0.0f);
}

glm::vec3 color(const Ray& r)
{
	if (hitSphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, r))
		return glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 unitDirection = normalize(r.direction());
	float t = 0.5*(unitDirection.y + 1.0f);
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
			glm::vec3 col = color(r);
			int ir = int(255.99f*col.r);
			int ig = int(255.99f*col.g);
			int ib = int(255.99f*col.b);

			outImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	outImage.close();

	return 0;
}