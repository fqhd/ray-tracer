#include "photorealistic.hpp"
#include "config.hpp"
#include <fstream>

Photorealistic::Photorealistic():
	m_canvas(Config::WIDTH, Config::HEIGHT)
{
}

Photorealistic::~Photorealistic() {

}

void Photorealistic::finish(Scene* scene) {
	// Do not use jpg, write everything in png
	std::ifstream is("camera.txt");

	while (is >> scene->camera.origin[0] >> scene->camera.origin[1] >> scene->camera.origin[2] >> scene->camera.direction[0] >> scene->camera.direction[1] >> scene->camera.direction[2]) {
		render(scene, &m_canvas, 10);
		m_canvas.save("raytraced/noisy/frame_" + std::to_string(m_frame) + ".png");
		render(scene, &m_canvas, 10000);
		m_canvas.save("raytraced/clean/frame_" + std::to_string(m_frame) + ".png");
		m_frame++;
		std::cout << "Saved " << m_frame << " frames" << std::endl;
	}

	is.close();
	// stbi_write_jpg("output.jpg", Config::WIDTH, Config::HEIGHT, 3, m_canvas.getData(), 90);
}