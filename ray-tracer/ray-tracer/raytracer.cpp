#include "raytracer.hpp"
#include "time.hpp"

vec3 ray_col(const ray& r, int depth, const hittable_list* world)
{
	if (depth >= Config::MAX_DEPTH) return vec3();
	hit_record rec;
	if (world->hit(r, interval(0.00001, INFINITY), rec)) {
		ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return ray_col(scattered, depth + 1, world) * attenuation;
		return vec3();
	}
	double height = r.direction().z();
	height = (height + 1.0) / 2.0;
	vec3 col = vec3(1.0, 1.0, 1.0) * (1 - height) + vec3(0.5, 0.7, 1.0) * height;
	return col;
	return vec3();
}

Pixel ray_data(const ray& r, const hittable_list* world)
{
	hit_record rec;
	if (world->hit(r, interval(0.00001, INFINITY), rec)) {
		ray scattered;
		vec3 attenuation;
		rec.mat_ptr->scatter(r, rec, attenuation, scattered);
		Pixel p;
		p.r = attenuation[0];
		p.g = attenuation[1];
		p.b = attenuation[2];
		p.nx = rec.normal.x();
		p.ny = rec.normal.y();
		p.nz = rec.normal.z();
		p.x = rec.p.x();
		p.y = rec.p.y();
		p.z = rec.p.z();
		p.depth = rec.t;
		return p;
	}
	double height = r.direction().z();
	height = (height + 1.0) / 2.0;
	vec3 col = vec3(1.0, 1.0, 1.0) * (1 - height) + vec3(0.5, 0.7, 1.0) * height;
	Pixel p;
	p.r = col[0];
	p.g = col[1];
	p.b = col[2];
	return p;
}

struct RenderJob {
	int x;
	int y;
	int width;
	int height;
};

void render_worker(Scene* scene, Canvas* canvas, std::vector<RenderJob>* jobs, std::mutex* mutex) {
	RenderJob job;

	while (true) {
		// Lock the mutex and find a job to do
		mutex->lock();
		if (jobs->size() > 0) {
			job = jobs->back();
			jobs->pop_back();
		}
		else {
			// If there are no more jobs to be done just leave
			std::cout << "Finished rendering" << std::endl;
			mutex->unlock();
			return;
		}
		mutex->unlock();

		// Do the job
		double dX = 2 / (double)Config::WIDTH;
		double dY = 2 / (double)Config::HEIGHT;

		for (int y = job.y; y < job.y + job.height; y++) {
			for (int x = job.x; x < job.x + job.width; x++) {
				double u = (double)x / (double)Config::WIDTH * 2.0 - 1.0;
				double v = (double)y / (double)Config::HEIGHT * 2.0 - 1.0;
				vec3 col;
				for (int i = 0; i < Config::RAYS_PER_PIXEL; i++) {
					double dx = (random_double() - 0.5);
					double dy = (random_double() - 0.5);
					double du = dx * dX;
					double dv = dy * dY;
					ray r = scene->camera.get_ray(u + du, v + dv);
					col += ray_col(r, 0, &scene->world);
				}
				col /= Config::RAYS_PER_PIXEL;
				canvas->setPixel(x, y, col);
			}
		}
	}
}

void gbuffer_worker(Scene* scene, GBuffer* gbuffer, std::vector<RenderJob>* jobs, std::mutex* mutex) {
	RenderJob job;

	while (true) {
		// Lock the mutex and find a job to do
		mutex->lock();
		if (jobs->size() > 0) {
			job = jobs->back();
			jobs->pop_back();
		}
		else {
			// If there are no more jobs to be done just leave
			mutex->unlock();
			return;
		}
		mutex->unlock();

		for (int y = job.y; y < job.y + job.height; y++) {
			for (int x = job.x; x < job.x + job.width; x++) {
				double u = (double)x / (double)Config::WIDTH * 2.0 - 1.0;
				double v = (double)y / (double)Config::HEIGHT * 2.0 - 1.0;
				ray r = scene->camera.get_ray(u, v);
				Pixel p = ray_data(r, &scene->world);
				gbuffer->setPixel(p, x, y);
			}
		}
	}
}

void render(Scene* scene, Canvas* canvas) {
	std::mutex mutex;
	std::thread** threads = new std::thread * [Config::THREADS];
	std::vector<RenderJob> jobs;

	for (int y = 0; y < Config::HEIGHT; y += Config::PATCH_SIZE) {
		for (int x = 0; x < Config::WIDTH; x += Config::PATCH_SIZE) {
			int width = std::min(Config::PATCH_SIZE, Config::WIDTH - x);
			int height = std::min(Config::PATCH_SIZE, Config::HEIGHT - y);
			jobs.push_back({ x, y, width, height });
		}
	}

	for (int i = 0; i < Config::THREADS; i++) {
		threads[i] = new std::thread(render_worker, scene, canvas, &jobs, &mutex);
	}
	for (int i = 0; i < Config::THREADS; i++) {
		threads[i]->join();
	}
	for (int i = 0; i < Config::THREADS; i++) {
		delete threads[i];
	}
	delete[] threads;
}

void render_gbuffer(Scene* scene, GBuffer* canvas) {
	std::mutex mutex;
	std::thread** threads = new std::thread * [Config::THREADS];
	std::vector<RenderJob> jobs;

	for (int y = 0; y < Config::HEIGHT; y += Config::PATCH_SIZE) {
		for (int x = 0; x < Config::WIDTH; x += Config::PATCH_SIZE) {
			int width = std::min(Config::PATCH_SIZE, Config::WIDTH - x);
			int height = std::min(Config::PATCH_SIZE, Config::HEIGHT - y);
			jobs.push_back({ x, y, width, height });
		}
	}

	for (int i = 0; i < Config::THREADS; i++) {
		threads[i] = new std::thread(gbuffer_worker, scene, canvas, &jobs, &mutex);
	}
	for (int i = 0; i < Config::THREADS; i++) {
		threads[i]->join();
	}
	for (int i = 0; i < Config::THREADS; i++) {
		delete threads[i];
	}
	delete[] threads;
}
