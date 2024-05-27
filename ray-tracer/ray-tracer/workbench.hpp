#pragma once
#include "glad.h"
#include "canvas.hpp"
#include "raytracer.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fstream>

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
	GBuffer m_gbuffer;
	Scene* m_scene;
	std::ofstream m_camOutFile;

	void set_texture(GLuint texID);
	void handleKeyboardInput(double dt);
	void handleMouseInput();
	void updateCinematicCamera(double dt);
	void copyGBufferIntoCanvas();
	void recordCameraPosition();

public:
	Workbench(Scene* scene);
	~Workbench();

	void refresh();
	void poll_events();
	bool should_continue();
	double m_currX, m_currY, m_prevX, m_prevY;
	vec3 m_targetDirection;
	vec3 m_targetOrigin;

	void finish();
};
