#ifndef APP_HPP_
#define APP_HPP_

#include "renderer.hpp"
#include "texture.hpp"
#include <GLFW/glfw3.h>

class Application {
private:
	GLFWwindow *m_window;
	Texture m_tex;

public:
	Application();
	~Application();

	void run();
};

#endif