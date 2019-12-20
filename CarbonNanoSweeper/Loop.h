#ifndef JG_LOOP_H
#define JG_LOOP_H

//#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

// Maybe split into seperate loops for rendering and physics later
// For now just do one loop reasponsible for everything
class Loop {
private:
	// Window (Exit Loop if window should close)
	GLFWwindow * window;
	// Set to false to quit loop and end everything
	bool shouldRun;
	// Set time before loopEvent and determine how
	// much time has passed during the last loopEvent
	double time;
public:
	Loop(GLFWwindow * W);
	virtual ~Loop();
	// This will only be left when the loop is over
	void doLoop();
	// Actual Content of the loop
	virtual void loopEvent(float dt);
	// Event after the loop ends
	virtual void endOfLoop();
	// The one loop doing everything
	static Loop * L;
};

#endif