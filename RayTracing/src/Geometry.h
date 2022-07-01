#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 origin;
	glm::vec3 normalized_direction;
};

struct Sphere
{
	glm::vec3 center;
	float radius;
};

bool is_sphere_hit(const Ray& ray, const Sphere& ball);

#endif