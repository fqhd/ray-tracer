#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <glad.h>
#include <GLFW/glfw3.h>
#include "canvas.hpp"
#include "subscription.hpp"

struct RendererSubscription {
	Subscription tick;
};

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual Canvas& canvas() = 0;

	virtual void add_subscription(RendererSubscription& subscription)
	{
	}

	// This function "finishes" the rendering, in the case of a png renderer it would write it to a file, in the openGL renderer it polls the window in a loop
	virtual void finish() = 0;
};

#endif
