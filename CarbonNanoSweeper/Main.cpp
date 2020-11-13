#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

//#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DTGlobal.h"
#include "RenderData.h"
#include "Camera.h"
#include "Keys.h"
#include "Loop.h"
#include "CNS.h"

using namespace std;

void error_callback(int error, const char* description)
{
	(void) error;
	cout << description << endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Keys::onCallback(key, scancode, action, mods);

	if (action == 0) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_ENTER:
			CNS::Game->initGame();
			break;
		case GLFW_KEY_1:
			RenderData::setColorTheme(0);
			break;
		case GLFW_KEY_2:
			RenderData::setColorTheme(1);
			break;
		case GLFW_KEY_3:
			CNS::Game->setTileSet(0);
			break;
		case GLFW_KEY_4:
			CNS::Game->setTileSet(1);
			break;
		case GLFW_KEY_5:
			CNS::Game->setTileSet(2);
			break;
		case GLFW_KEY_6:
			CNS::Game->setTileSet(3);
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	(void) mods;
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//float x = cursorX / width;
	//float y = cursorY / height;
	//float ratio = width / (float)height;
	if (action == 0) {
		int hex = Camera::C->findCentralHexagon();
		if(hex >= 0){
			if (button == 0) {
				CNS::Game->leftClick(hex);
			}
			else if (button == 1) {
				CNS::Game->rightClick(hex);
			}
		}
	}
}

void resize_callback(GLFWwindow* window, int width, int height) {
	(void) window;
	cout << "Height: " << height << "\tWidth: " << width << endl;
}

/*static void * wrap_getprocaddress(const char *name, void *user_ptr)
{
	(void)user_ptr;
	return (void*)glfwGetProcAddress(name);
}*/

//Main function
int main()
{
	Keys::init();

	glfwSetErrorCallback(error_callback);


	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Carbon Nano Sweeper", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*if (!gladLoadGL(wrap_getprocaddress, NULL)) {
		return -1;
	}*/

	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	glfwSetFramebufferSizeCallback(window, resize_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Creates Shaders
	RenderData::init();

	// Create camera object
	Camera::C = new Camera(window);

	// Loop until the user closes the window or exits the game
	Loop::L = new Loop(window);
	Loop::L->doLoop();

	// Delete
	Keys::end();
	delete Loop::L;
	delete Camera::C;

	// End
	// Causes a memory access violation on some machines
	//...
	glfwTerminate();

	return 0;
}
