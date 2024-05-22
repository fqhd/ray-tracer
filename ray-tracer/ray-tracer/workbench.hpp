#pragma once
#include "glad.h"
#include "canvas.hpp"
#include "raytracer.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class GLCanvas : public Canvas {
private:
	GLuint m_glTexture;

public:
	GLCanvas(size_t width, size_t height);
	void init();

	void bind();
	void unbind();
	void update();

	friend class Workbench;
};

class Workbench {
private:
	GLuint m_vertexbuffer, m_programID;
	GLFWwindow *m_window;
	GLCanvas m_canvas;
	Scene* m_scene;

	void set_texture(GLuint texID);

public:
	Workbench(Scene* scene);
	~Workbench();

	void refresh();
	void poll_events();
	bool should_continue();

	void finish();
};
