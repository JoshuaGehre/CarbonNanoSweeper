#include "Texture.h"

//#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "RenderData.h"
#include <iostream>

Texture::Texture()
{
	height = 0;
	width = 0;
	pixels = NULL;
	onGraphicsCard = false;
}

Texture::Texture(std::string file) : Texture()
{
	load(file);
}

Texture::~Texture()
{
	clear();
}

void Texture::load(std::string file)
{
	clear();
	pixels = stbi_load(file.c_str(), &width, &height, &channels, 0);
	if (pixels == NULL) {
		std::cout << file << " could not be loaded" << std::endl;
	}
	// Flip on X Axis
	int h = height / 2;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channels; k++) {
				std::swap(
					pixels[k + channels * (j + width * i)],
					pixels[k + channels * (j + width * (height - i - 1))]);
			}
		}
	}
}

void Texture::createEmpty(unsigned int w, unsigned h)
{
	clear();
	width = w;
	height = h;
	channels = 4;
	pixels = new unsigned char[width * height * channels];
}

void Texture::clear()
{
	height = 0;
	width = 0;
	channels = 0;
	if (pixels != NULL) {
		delete[] pixels;
		pixels = NULL;
		if (onGraphicsCard) {
			glDeleteTextures(1, &binding);
		}
	}
	onGraphicsCard = false;
	binding = 0;
}

void Texture::toGraphicsCard()
{
	if (onGraphicsCard) {
		glDeleteTextures(1, &binding);
	}
	glGenTextures(1, &binding);
	glBindTexture(GL_TEXTURE_2D, binding);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	RenderData::setTexParam(GL_NEAREST, GL_CLAMP_TO_EDGE);
	onGraphicsCard = true;
}

void Texture::setAs(int a)
{
	glActiveTexture(a);
	if (!onGraphicsCard) {
		toGraphicsCard();
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, binding);
}

void Texture::generateMipMap()
{
	if (pixels == NULL) return;
	if (!onGraphicsCard) {
		toGraphicsCard();
	}
	int maxLevel = 0;
	int mSize = (height < width) ? height : width;
	while (mSize != 0) {
		maxLevel++;
		mSize >>= 1;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, binding);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

float Texture::getRatio()
{
	return static_cast<float>(width) / static_cast<float>(height);
}

void Texture::setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if ((x >= (unsigned int) width) || (y >= (unsigned int) height)) return;
	int pos = x + y * width;
	pos *= channels;
	pixels[pos] = r;
	pixels[pos + 1] = g;
	pixels[pos + 2] = b;
	if (channels == 3) return;
	pixels[pos + 3] = a;

}

void Texture::setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	setPixel(x, y, r, g, b, 255);
}

void Texture::setPixel(unsigned int x, unsigned int y, unsigned int argb)
{
	setPixel(x,y, (argb >> 16) & 0xff, (argb >> 8) & 0xff, argb & 0xff, (argb >> 24) & 0xff);
}
