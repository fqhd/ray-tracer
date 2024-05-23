#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include "vec.hpp"
#include "ray.hpp"

class Camera {
private:
	double m_vfov;
	double m_aspect_ratio;
	
public:
	vec3 origin, direction;

	Camera(vec3 origin, vec3 dir, double vfov, double aspect_ratio);

	ray get_ray(double u, double v) const;
};

#endif