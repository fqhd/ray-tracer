#include "canvas.hpp"
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "config.hpp"

Canvas::Canvas(size_t width, size_t height)
	: m_width(width), m_height(height), m_buf(width * height * 3)
{
	stbi_flip_vertically_on_write(true);
}

size_t Canvas::width() const {
	return m_width;
}

size_t Canvas::height() const {
	return m_height;
}

uint8_t* Canvas::getData() const {
	return (uint8_t*)m_buf.data();
}

const uint8_t& Canvas::operator[](size_t i) const {
	return m_buf[i];
}

void Canvas::setPixel(size_t x, size_t y, float r, float g, float b) {
	int idx = (x + y * m_width) * 3;
	m_buf[idx] = (uint8_t)(r * 255);
	m_buf[idx + 1] = (uint8_t)(g * 255);
	m_buf[idx + 2] = (uint8_t)(b * 255);
}

void Canvas::setPixel(size_t x, size_t y, vec3 col) {
	setPixel(x, y, col.x(), col.y(), col.z());
}

void Canvas::save(const std::string& path) const {
	stbi_write_png(path.c_str(), m_width, m_height, 3, m_buf.data(), m_width * 3);
}