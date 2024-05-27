#pragma once
#include <string>
#include "canvas.hpp"
#include "raytracer.hpp"

class Photorealistic {
private:
	Canvas m_canvas;

public:

	Photorealistic();
	~Photorealistic();

	void finish(Scene* scene);

};