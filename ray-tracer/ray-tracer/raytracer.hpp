#pragma once
#include "scene.hpp"
#include "canvas.hpp"
#include <vector>
#include <mutex>
#include <thread>

void render(Scene* scene, Canvas* canvas);