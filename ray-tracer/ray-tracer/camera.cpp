#include "camera.hpp"

#include <cmath>

constexpr double PI = 3.14159265358979323846;

Camera::Camera(
	vec3 origin,
	vec3 dir,
	double vofv,
	double aspect_ratio
) : origin(origin),
	direction(dir.normalize()),
	m_vfov(vofv),
	m_aspect_ratio(aspect_ratio)
{
}

ray Camera::get_ray(double u, double v) const
{
	vec3 right = cross(direction, UP).normalize();
	vec3 up = cross(right, direction);
	double focal_length = 1.0f / std::tan(m_vfov * PI / 2.0f / 180.0f);

	return ray(
		origin,
		(
			direction * focal_length
			+ right * (u) * m_aspect_ratio
			+ up * (v)
			).normalize()
	);
}
