#include "photorealistic.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "config.hpp"

Photorealistic::Photorealistic():
	m_canvas(Config::WIDTH, Config::HEIGHT)
{
	stbi_flip_vertically_on_write(true);
}

Photorealistic::~Photorealistic() {

}

void Photorealistic::finish(Scene* scene) {
	// Do not use jpg, write everything in png
	stbi_write_jpg("output.jpg", Config::WIDTH, Config::HEIGHT, 3, m_canvas.getData(), 90);
}