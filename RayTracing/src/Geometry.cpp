#include "Geometry.h"

bool is_sphere_hit(const Ray& ray, const Sphere& ball)
{
	// See episode 2 for a derivation of the 2D case
	auto b = 2.0f * glm::dot(ray.normalized_direction, ray.origin - ball.center);
	auto c = glm::dot(ray.origin, ray.origin) - 2.0f * glm::dot(ray.origin, ball.center) + glm::dot(ball.center, ball.center) - ball.radius * ball.radius;
	auto discriminant = b * b - 4.0f * c;
	return discriminant >= 0.0f;
}