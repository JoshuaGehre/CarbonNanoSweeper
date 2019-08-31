#ifndef JG_RENDER_DATA_H
#define JG_RENDER_DATA_H

#include "DTGlobal.h"
#include <string>
#include "Texture.h"
#include "glm/gtc/type_ptr.hpp"

class GLFWcursor;

struct InitialFBOStruct {
	unsigned int fbo;
	unsigned int tex_color;
	//unsigned int tex_depth;
};

struct OneTextureFBOStruct {
	unsigned int fbo;
	unsigned int tex_color;
	unsigned int tex_depth;
	void setAsSource();
	void setAsSource(unsigned int T1, unsigned int T2);
	void setAsTarget();
	void generateMipMap();
	float getRatio();
	unsigned int width = 0;
	unsigned int height = 0;
};

struct TwoTextureFBOStruct {
	unsigned int fbo;
	unsigned int tex_color;
	unsigned int tex_color_back;
	unsigned int tex_depth;
	void setAsSource();
	void setAsSource(unsigned int T1, unsigned int T2, unsigned int T3);
	void setAsTarget();
	float getRatio();
	unsigned int width = 0;
	unsigned int height = 0;
};

struct RayMarchShaderStruct {
	unsigned int atoms;
	unsigned int bonds;
	unsigned int cylinder;
};

struct ShaderStruct{
	unsigned int copy;
	unsigned int combine;
};

struct PostProcessingStruct {
	InitialFBOStruct Initial;
	OneTextureFBOStruct Temp[PP_TEMP_SINGLE_TEX];
	TwoTextureFBOStruct Temp2;
	// Ray Marching
	RayMarchShaderStruct RayMarchShaders;
	// Post processing effects
	ShaderStruct Shaders;
	bool hasBeenCreated = false;
	int height = -1;
	int width = -1;
};

struct UBOSizeStruct {
	glm::ivec4 ScreenSize_E_E;
	glm::vec4 PixelSize_E_E;
};

struct UBOVectorStruct {
	glm::vec4 CameraPosition;
	glm::vec4 CameraDirection;
	glm::vec4 CameraUp;
	glm::vec4 CameraRight;
};

struct UBOTileStruct {
	int ids[16 * ROW_NUM];
};

struct UBOHighlightStruct {
	glm::ivec4 HighlightHex_E_E_E;
};

struct UBOColorStruct {
	glm::vec4 BackgroundColor;
	glm::vec4 AtomColor;
	glm::vec4 BondColor;
};

struct UBOStruct {
	UBOSizeStruct Sizes;
	unsigned int Sizes_UBO;
	UBOVectorStruct Vectors;
	unsigned int Vectors_UBO;
	UBOTileStruct Tiles;
	unsigned int Tiles_UBO;
	UBOHighlightStruct Highlight;
	unsigned int Highlight_UBO;
	UBOColorStruct Colors;
	unsigned int Color_UBO;
};

class RenderData {
private:
	static std::string shaderLocation;
	static void loadShader(unsigned int sh, std::string File);
	static std::string loadShaderFile(std::string File);

public:
	static PostProcessingStruct POSTPROCESSING;
	static UBOStruct UBO;
	static Texture * TILES[4];

	static void adaptFBOsToSize(int width, int heigth);
	static void init();
	static void createSimplePPShader(unsigned int &i, std::string Frag);
	static void loadSimpleShader(unsigned int &A, std::string vert, std::string frag);
	static void setTexParam(int Filter, int Wrap);
	static void updateUBO(unsigned int U);
	static void setColorTheme(int i);
};

#endif