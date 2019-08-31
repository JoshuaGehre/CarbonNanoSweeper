#include "Loop.h"

#include <iostream>

#include "RenderData.h"
#include "Camera.h"
#include "Perspective.h"
#include "CNS.h"

using namespace std;

Loop * Loop::L = NULL;

Loop::Loop(GLFWwindow * W) :
	shouldRun(true),
	window(W)
{
}

Loop::~Loop()
{
}

void Loop::doLoop()
{
	unsigned int vbo = 0;
	unsigned int vao = 0;
	int datasize = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, datasize, NULL, GL_STATIC_DRAW);

	RenderData::setColorTheme(0);

	CNS::Game = new CNS();

	// Get first time before start of the loop
	time = glfwGetTime();
	while (shouldRun) {
		// Get new time
		double newTime = glfwGetTime();
		double dt = newTime - time;
		time = newTime;
		// Do Loop
		loopEvent(dt);
		// Check if window should close
		if (glfwWindowShouldClose(window)) {
			shouldRun = false;
		}
	}
	// End of loop
	endOfLoop();

	delete CNS::Game;
}

void Loop::loopEvent(float dt)
{
	// Update Title (Test to show time to render)
	/*if (floor(time * 5) > floor((time - dt) * 5)) {
		string title = "CarbonNanoSweeper " + std::to_string(1000 * dt) + "ms";
		glfwSetWindowTitle(window, title.c_str());
	}*/
	if (CNS::Game->newName) {
		CNS::Game->newName = false;
		glfwSetWindowTitle(window, ("CarbonNanoSweeper   " + CNS::Game->title).c_str());
	}
	
	// Poll for and process events
	glfwPollEvents();

	// ---- Update ----
	Camera::C->updateFirstPersonCamera(dt);

	// ---- Render ----

	// Get window size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	RenderData::adaptFBOsToSize(width, height);
	
	// Don't draw into minimized window
	if ((width > 0) && (height > 0)) {
		float ratio = width / (float)height;
		// Camera Position
		Camera::C->setVectors(ratio);

		CNS::Game->render();
	}

	glfwSwapBuffers(window);
}

void Loop::endOfLoop()
{
	// Do whatever needs to be done at the end of the loop
	//...
}
