#pragma once
#include <string>
#include <vector>

struct Pixel {
	Pixel() {
		x = 0;
		y = 0;
		z = 0;
		nx = 0;
		ny = 0;
		nz = 0;
		r = 0;
		g = 0;
		b = 0;
		depth = 0;
	}
	float x, y, z;
	float nx, ny, nz;
	float r, g, b;
	float depth;
};

class GBuffer
{
public:


	GBuffer();
	void setPixel(const Pixel& pixel, int i, int j);
	void save(const std::string& path);
	const Pixel& getPixel(int i, int j) const;


private:

	std::vector<Pixel> m_buffer;

};
