#pragma once
#include <thread>

namespace Config {
	const int WIDTH = 640;
	const int HEIGHT = 360;
	const int RAYS_PER_PIXEL = 256;
	const int MAX_DEPTH = 50;
	//const int THREADS = std::thread::hardware_concurrency();
	const int THREADS = 12;
	const int PATCH_SIZE = 128;
	const int SAVE_INTERVAL = 100;
}