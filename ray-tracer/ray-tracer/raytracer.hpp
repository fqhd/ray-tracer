#pragma once
#include "scene.hpp"
#include "canvas.hpp"
#include "GBuffer.hpp"
#include <vector>
#include <mutex>
#include <thread>

void render(Scene* scene, Canvas* canvas);
void render_gbuffer(Scene* scene, GBuffer* canvas);