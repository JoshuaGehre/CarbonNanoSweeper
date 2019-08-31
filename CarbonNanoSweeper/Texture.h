#ifndef JG_TEXTURE_H
#define JG_TEXTURE_H

#include <string>

class Texture {
private:
	int width;
	int height;
	int channels;
	unsigned char * pixels;
	bool onGraphicsCard;
	unsigned int binding;
public:
	Texture();
	Texture(std::string file);
	~Texture();
	void load(std::string file);
	void createEmpty(unsigned int w, unsigned h);
	void clear();
	void toGraphicsCard();
	void setAs(int a);
	void generateMipMap();
	float getRatio();
	void setPixel(unsigned int x, unsigned int y,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setPixel(unsigned int x, unsigned int y,
		unsigned char r, unsigned char g, unsigned char b);
	void setPixel(unsigned int x, unsigned int y,
		unsigned int argb);
};

#endif