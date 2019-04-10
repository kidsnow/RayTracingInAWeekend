#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "float.h"
#include "sphere.h"
#include "hitable_list.h"

glm::vec3 Color(const Ray& r, Hitable *world)
{
	HitRecord rec;
	if (world->Hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5f * glm::vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
	}
	else
	{
		glm::vec3 unitDirection = normalize(r.direction());
		float t = 0.5*(unitDirection.y + 1.0f);
		return (1.0f - t)*glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
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
	Hitable *list[2];
	list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);
	Hitable *world = new HitableList(list, 2);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			
			glm::vec3 p = r.PointAtParameter(2.0);
			glm::vec3 col = Color(r, world);
			int ir = int(255.99f*col.r);
			int ig = int(255.99f*col.g);
			int ib = int(255.99f*col.b);

			outImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	outImage.close();

	return 0;
}