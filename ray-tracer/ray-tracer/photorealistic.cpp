#include "photorealistic.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "config.hpp"

Photorealistic::Photorealistic():
	m_canvas(Config::WIDTH, Config::HEIGHT)
{
	stbi_flip_vertically_on_write(true);
}

Photorealistic::~Photorealistic() {

}

Canvas& Photorealistic::canvas() {
	return m_canvas;
}

void Photorealistic::finish() {
	stbi_write_jpg("output.jpg", Config::WIDTH, Config::HEIGHT, 3, m_canvas.getData(), 90);
}