#pragma once
#include <string>
#include "canvas.hpp"
#include "raytracer.hpp"

class Photorealistic {
private:
	Canvas m_canvas;
	int m_frame = 0;

public:

	Photorealistic();
	~Photorealistic();

	void finish(Scene* scene);

};