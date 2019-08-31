#ifndef JG_CAMERA_H
#define JG_CAMERA_H

#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera {
private:
	/*       Camera  
	*      Direction   y
	*           \      ^
	*            \     |     / Front Face
	*           / \   _|____/ 
	* Theta -> |  _\-- |   / 
	*          |/   \  |  / <- Phi
	*          /__   \ | /
	*             ---_\|/
	*    --------------+-------------> x
	*                 /|                   / - cos(theta) * sin(phi) \
	*                / |   	       ->      |                         |
	*               /  |	       r = r * |        sin(theta)       |
	*              /   |                   |                         |
	*             /    |                   \ - cos(theta) * cos(phi) /
	*          z L     |
	*/
	float phi;
	float theta;

	float posX;
	float posPhi;
	// Window from which to read the cursor location
	GLFWwindow* window;
	// Cursor for captured mouse
	bool cursorIsCaptured;
	GLFWcursor* capturedCursor;
	// Last positon the cursor was at
	double cursorX;
	double cursorY;
	// Set cursorX/Y to current position without rotation
	void readCursorNoMovement();
public:
	Camera(GLFWwindow* w);
	~Camera();
	void resetCamera();
	// Update Camera and reset cursor to center
	void updateFirstPersonCamera(float dt);
	// World to Eye space transformation matrix
	//glm::mat4x4 getCameraTransform();
	void setVectors(float ratio);
	// Capture and free mouse
	void captureMouse();
	void freeMouse();
	inline bool isMouseCaptured() { return cursorIsCaptured; };
	// Get position of the mouse on the screen
	void findMouse(float &ratio, float &x, float &y);

	static Camera * C;

	int findCentralHexagon();
};

#endif