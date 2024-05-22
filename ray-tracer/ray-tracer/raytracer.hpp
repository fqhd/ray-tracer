#pragma once
#include "scene.hpp"
#include "canvas.hpp"
#include <vector>
#include <mutex>
#include <thread>

struct RenderJob {
	int x;
	int y;
	int width;
	int height;
};

class Raytracer {
public:

	Raytracer(Scene* scene, Canvas* canvas);
	void render();

private:
	
	Canvas* m_canvas;
	Scene* m_scene;

};