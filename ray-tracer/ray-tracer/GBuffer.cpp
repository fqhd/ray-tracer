#include "GBuffer.hpp"
#include "stb_image_write.h"
#include "config.hpp"
#include <fstream>
#include <iostream>

GBuffer::GBuffer()
    : m_buffer(Config::WIDTH * Config::HEIGHT)
{
}

void GBuffer::setPixel(const Pixel& pixel, int x, int y) {
	int idx = (x + y * Config::WIDTH);
	m_buffer[idx] = pixel;
}

void GBuffer::save(const std::string& path) {
    std::ofstream outFile(path.c_str(), std::ios::out | std::ios::binary);

    // Check if the file was successfully opened
    if (!outFile) {
        std::cerr << "Failed to open the file." << std::endl;
    }

    // Write the buffer to the file
    outFile.write((const char*)m_buffer.data(), m_buffer.size() * sizeof(m_buffer[0]));

    // Close the file
    outFile.close();

    if (outFile.fail()) {
        std::cerr << "Error occurred while writing to the file." << std::endl;
    }
}

const Pixel& GBuffer::getPixel(int i, int j) const {
    return m_buffer[j + i * Config::WIDTH];
}