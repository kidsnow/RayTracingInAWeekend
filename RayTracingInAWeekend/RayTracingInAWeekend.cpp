#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "material.h"

// For random numbers
#include <stdlib.h>
#include <time.h>

float random()
{
	return float(rand()) / float(RAND_MAX + 1);
}

glm::vec3 RandomInUnitSphere()
{
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(random(), random(), random()) - glm::vec3(1.0, 1.0, 1.0);
	} while (glm::length(p) >= 1.0);
	return p;
}

glm::vec3 Color(const Ray& r, Hitable *world, int depth)
{
	HitRecord rec;
	if (world->Hit(r, 0.001, FLT_MAX, rec))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.matPtr->Scatter(r, rec, attenuation, scattered))
		{
			return attenuation * Color(scattered, world, depth + 1);
		}
		else
		{
			return glm::vec3(0, 0, 0);
		}
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize(r.direction());
		float t = 0.5f*(unitDirection.y + 1.0f);
		return (1.0f - t)*glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	srand(time(NULL));
	std::ofstream outImage("raytraced.ppm");
	int nx = 200;
	int ny = 100;
	int ns = 100;
	outImage << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *list[4];
	list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(glm::vec3(0.8, 0.3, 0.3)));
	list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(glm::vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8, 0.6, 0.2), 1.0f));
	list[3] = new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8, 0.8, 0.8), 0.0f));
	Hitable *world = new HitableList(list, 4);
	Camera cam;

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			glm::vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				Ray r = cam.GetRay(u, v);
				col += Color(r, world, 0);
			}
			col /= float(ns);
			col = glm::vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99f*col.r);
			int ig = int(255.99f*col.g);
			int ib = int(255.99f*col.b);
			outImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	outImage.close();

	return 0;
}