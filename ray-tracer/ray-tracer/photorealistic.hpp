#pragma once
#include <string>
#include "canvas.hpp"

class Photorealistic {
private:
	Canvas m_canvas;

public:

	Photorealistic();
	~Photorealistic();

	Canvas& canvas();
	void finish();

};