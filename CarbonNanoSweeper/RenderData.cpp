#include "RenderData.h"

//#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

PostProcessingStruct RenderData::POSTPROCESSING;
UBOStruct RenderData::UBO;
string RenderData::shaderLocation = "resources/shaders/";
Texture * RenderData::TILES[4] = { nullptr, nullptr, nullptr, nullptr };

void RenderData::loadShader(unsigned int sh, std::string File)
{
	string S = loadShaderFile(File) + "";
	//std::cout << File << ":\n" << S << "\n";
	const char * C = S.c_str();
	glShaderSource(sh, 1, &C, NULL);
	glCompileShader(sh);
	//Error Stuff
	GLint isCompiled = 0;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		cout << "Error for: " << File << endl;
		cout << "Text: " << endl;
		cout << C << endl;
		GLint maxLength = 0;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(sh, maxLength, &maxLength, &errorLog[0]);
		int i;
		for (i = 0; i < maxLength; i++) {
			cout << errorLog[i];
		}
		cout << endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(sh); // Don't leak the shader.
		return;
	}
}

std::string RenderData::loadShaderFile(std::string File)
{
	ifstream In;
	In.open(shaderLocation + File);
	if (!In) {
		cout << "ERROR: " << (shaderLocation + File.c_str()) << " does not exist\n";
		return "";
	}
	std::string S = "";
	std::string R = "";
	char T, U;
	while (!In.eof()) {
		T = In.get();
		if ((int)T == -1) {
			S += "\0";
		}
		else if (T == '@') {
			T = In.get();
			U = 1;
			R = "";
			while (!In.eof() && (U != '>') && (U != ')') && (U != '}') && (U != ']')) {
				U = In.get();
				if ((U != '>') && (U != ')') && (U != '}') && (U != ']')) R += U;
			}
			//cout << R.c_str() << endl;
			switch (T) {
			case '<':
				//Load and insert specified file here
				S += loadShaderFile(R);
				break;
			case '{':
				//Currently no specified meaning
				break;
			case '(':
				//Currently no specified meaning
				break;
			case '[':
				//Currently no specified meaning
				break;
			}
		}
		else {
			S += T;
		}
	}
	In.close();
	return S;
}

void RenderData::setTexParam(int Filter, int Wrap)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap);
}

void RenderData::updateUBO(unsigned int U)
{
#define doUboUpdate(Name, Data, Struct) glBindBuffer(GL_UNIFORM, RenderData::UBO.Name);\
	glBindBufferBase(GL_UNIFORM_BUFFER, U, RenderData::UBO.Name);\
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Struct), &RenderData::UBO.Data);

	switch (U) {
	case UBOB_SCREEN_SIZES:
		doUboUpdate(Sizes_UBO, Sizes, UBOSizeStruct);
		break;
	case UBOB_VECTORS:
		doUboUpdate(Vectors_UBO, Vectors, UBOVectorStruct);
		break;
	case UBOB_TILES:
		doUboUpdate(Tiles_UBO, Tiles, UBOTileStruct);
		break;
	case UBOB_HIGHLIGHT:
		doUboUpdate(Highlight_UBO, Highlight, UBOHighlightStruct);
		break;
	case UBOB_COLORS:
		doUboUpdate(Color_UBO, Colors, UBOColorStruct);
		break;
	}

#undef doUboUpdate
}

void RenderData::setColorTheme(int i)
{
	switch (i) {
	case 0:
		RenderData::UBO.Colors.AtomColor = glm::vec4(1, 1, 1, 1);
		RenderData::UBO.Colors.BondColor = glm::vec4(1, 1, 1, 1);
		RenderData::UBO.Colors.BackgroundColor = glm::vec4(0, 0, 0, 1);
		break;
	case 1:
		RenderData::UBO.Colors.AtomColor = glm::vec4(0.1, 0.1, 0.1, 1);
		RenderData::UBO.Colors.BondColor = glm::vec4(0.25, 0.25, 0.25, 1);
		RenderData::UBO.Colors.BackgroundColor = glm::vec4(0.75, 0.9, 0.95, 1);
		break;
	}
	RenderData::updateUBO(UBOB_COLORS);
}

void RenderData::adaptFBOsToSize(int width, int height)
{
	if ((width == POSTPROCESSING.width) && (height == POSTPROCESSING.height)) return;
	POSTPROCESSING.height = height;
	POSTPROCESSING.width = width;
	UBO.Sizes.ScreenSize_E_E = glm::ivec4(width, height, 0, 0);
	UBO.Sizes.PixelSize_E_E = glm::vec4(1.0 / (float) width, 1.0 / (float) height, 0, 0);
	cout << "New width " << width << ", New height" << height << endl;
	int i;
	if (POSTPROCESSING.hasBeenCreated) {
		// Initial Game
		glDeleteTextures(1, &POSTPROCESSING.Initial.tex_color);
		//<||>glDeleteTextures(1, &POSTPROCESSING.Initial.tex_depth);
		glDeleteFramebuffers(1, &POSTPROCESSING.Initial.fbo);
		//
		for (i = 0; i < PP_TEMP_SINGLE_TEX; i++) {
			glDeleteTextures(1, &POSTPROCESSING.Temp[i].tex_color);
			glDeleteTextures(1, &POSTPROCESSING.Temp[i].tex_depth);
			glDeleteFramebuffers(1, &POSTPROCESSING.Temp[i].fbo);
		}

		glDeleteTextures(1, &POSTPROCESSING.Temp2.tex_color);
		glDeleteTextures(1, &POSTPROCESSING.Temp2.tex_color_back);
		glDeleteTextures(1, &POSTPROCESSING.Temp2.tex_depth);
		glDeleteFramebuffers(1, &POSTPROCESSING.Temp2.fbo);
	}
	glBindBuffer(GL_UNIFORM, UBO.Sizes_UBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBOB_SCREEN_SIZES, UBO.Sizes_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOSizeStruct), &UBO.Sizes);

	// Initial FBO (game)
	glGenTextures(1, &POSTPROCESSING.Initial.tex_color);
	glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Initial.tex_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &POSTPROCESSING.Initial.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, POSTPROCESSING.Initial.fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, POSTPROCESSING.Initial.tex_color, 0);

	for (i = 0; i < PP_TEMP_SINGLE_TEX; i++) {
		glGenTextures(1, &POSTPROCESSING.Temp[i].tex_color);
		glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Temp[i].tex_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

		glGenTextures(1, &POSTPROCESSING.Temp[i].tex_depth);
		glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Temp[i].tex_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
		setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &POSTPROCESSING.Temp[i].fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, POSTPROCESSING.Temp[i].fbo);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, POSTPROCESSING.Temp[i].tex_color, 0);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, POSTPROCESSING.Temp[i].tex_depth, 0);
		POSTPROCESSING.Temp[i].width = width;
		POSTPROCESSING.Temp[i].height = height;
	}

	glGenTextures(1, &POSTPROCESSING.Temp2.tex_color);
	glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Temp2.tex_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &POSTPROCESSING.Temp2.tex_color_back);
	glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Temp2.tex_color_back);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &POSTPROCESSING.Temp2.tex_depth);
	glBindTexture(GL_TEXTURE_2D, POSTPROCESSING.Temp2.tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
	setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &POSTPROCESSING.Temp2.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, POSTPROCESSING.Temp2.fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, POSTPROCESSING.Temp2.tex_color, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, POSTPROCESSING.Temp2.tex_color_back, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, POSTPROCESSING.Temp2.tex_depth, 0);
	POSTPROCESSING.Temp2.width = width;
	POSTPROCESSING.Temp2.height = height;

	POSTPROCESSING.hasBeenCreated = true;
}

void RenderData::init()
{
	//UBO's
#define createUBO(Struct, U, Name) glGenBuffers(1, &UBO.Name);\
	glBindBufferBase(GL_UNIFORM_BUFFER, U, UBO.Name);\
	glBindBuffer(GL_UNIFORM, UBO.Name);\
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Struct), NULL, GL_STATIC_DRAW);

	createUBO(UBOSizeStruct, UBOB_SCREEN_SIZES, Sizes_UBO);
	createUBO(UBOVectorStruct, UBOB_VECTORS, Vectors_UBO);
	createUBO(UBOTileStruct, UBOB_TILES, Tiles_UBO);
	createUBO(UBOHighlightStruct, UBOB_HIGHLIGHT, Highlight_UBO);
	createUBO(UBOColorStruct, UBOB_COLORS, Color_UBO);

#undef createUBO
	//Default Size
	adaptFBOsToSize(640, 480);

	// Create Ray Marching Shaders
	createSimplePPShader(POSTPROCESSING.RayMarchShaders.atoms, "rm_atoms");
	createSimplePPShader(POSTPROCESSING.RayMarchShaders.bonds, "rm_bonds");
	createSimplePPShader(POSTPROCESSING.RayMarchShaders.cylinder, "rt_cylinder");
	// Create Post Processing Shaders
	createSimplePPShader(POSTPROCESSING.Shaders.copy, "pp_copy");
	createSimplePPShader(POSTPROCESSING.Shaders.combine, "pp_combine");
	// Tiles
	TILES[0] = new Texture("resources/textures/Semi_Transparent.png");
	TILES[1] = new Texture("resources/textures/Solid.png");
	TILES[2] = new Texture("resources/textures/Transparent.png");
	TILES[3] = new Texture("resources/textures/Colored.png");
	
}

void RenderData::createSimplePPShader(unsigned int & i, std::string Frag)
{
	loadSimpleShader(i, "pp.vert", Frag + ".frag");
}

void RenderData::loadSimpleShader(unsigned int & A, std::string vert, std::string frag)
{
	GLint vertex_shader, fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	loadShader(vertex_shader, vert);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	loadShader(fragment_shader, frag);
	A = glCreateProgram();
	glAttachShader(A, vertex_shader);
	glAttachShader(A, fragment_shader);
	glLinkProgram(A);
}

void OneTextureFBOStruct::setAsSource()
{
	setAsSource(GL_TEXTURE0, GL_TEXTURE1);
}

void OneTextureFBOStruct::setAsSource(unsigned int T1, unsigned int T2)
{
	glActiveTexture(T1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_color);
	glActiveTexture(T2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_depth);
}

void OneTextureFBOStruct::setAsTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	GLenum Buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, Buffers);
	glViewport(0, 0, RenderData::POSTPROCESSING.width, RenderData::POSTPROCESSING.height);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OneTextureFBOStruct::generateMipMap()
{
	int maxLevel = 0;
	int mSize = (height < width) ? height : width;
	while (mSize != 0) {
		maxLevel++;
		mSize >>= 1;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

float OneTextureFBOStruct::getRatio()
{
	return static_cast<float>(width) / static_cast<float>(height);
}

void TwoTextureFBOStruct::setAsSource()
{
	setAsSource(GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2);
}

void TwoTextureFBOStruct::setAsSource(unsigned int T1, unsigned int T2, unsigned int T3)
{
	glActiveTexture(T1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_color);
	glActiveTexture(T2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_color_back);
	glActiveTexture(T3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_depth);
}

void TwoTextureFBOStruct::setAsTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	GLenum Buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, Buffers);
	glViewport(0, 0, RenderData::POSTPROCESSING.width, RenderData::POSTPROCESSING.height);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
}

float TwoTextureFBOStruct::getRatio()
{
	return static_cast<float>(width) / static_cast<float>(height);
}
