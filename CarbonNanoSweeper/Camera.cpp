#include "Camera.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>
#include <math.h>

#include <glm/gtx/transform.hpp>

#include "RenderData.h"
#include "Perspective.h"
#include "DTGlobal.h"
#include "Keys.h"

Camera * Camera::C = nullptr;


Camera::Camera(GLFWwindow* w)
{
	cursorIsCaptured = false;
	phi = 0;
	theta = 0;
	posX = 0;
	posPhi = 0;
	window = w;
	readCursorNoMovement();

	// Create cursor for captured mouse
	unsigned char pixels[4] = { 0,0,0,0x01 };
	GLFWimage image;
	image.width = 1;
	image.height = 1;
	image.pixels = pixels;
	capturedCursor = glfwCreateCursor(&image, 0, 0);
}

Camera::~Camera()
{
	//delete capturedCursor;
}

void Camera::resetCamera()
{
	phi = 0;
	theta = 0;
	posX = 0;
	posPhi = 0;
}

void Camera::readCursorNoMovement()
{
	glfwGetCursorPos(window, &cursorX, &cursorY);
}

void Camera::updateFirstPersonCamera(float dt)
{
	double cX, cY, dX, dY;
	glfwGetCursorPos(window, &cX, &cY);
	dX = cX - cursorX;
	dY = cY - cursorY;
	// Change Angles
	if (Keys::get(KEY_LOOK_LEFT)) { 
		phi += dt * 90; 
	}
	else if (Keys::get(KEY_LOOK_RIGHT)) {
		phi -= dt * 90;
	}
	else {
		phi *= exp(-3 * dt);
		if (abs(phi) < 0.1) phi = 0;
	}
	// Limit Phi
	if (phi < -60) phi = -60;
	if (phi > 60) phi = 60;

	if (cursorIsCaptured) {
		// Recenter Mouse
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glfwSetCursorPos(window, width >> 1, height >> 1);
		cursorX = width >> 1;
		cursorY = height >> 1;
	}
	else {
		// Use new position as old one for next step
		cursorX = cX;
		cursorY = cY;
	}
	// Movement
	double speed = dt * 10;
	double forward = (Keys::get(KEY_RIGHT) ? 1 : 0) + (Keys::get(KEY_LEFT) ? -1 : 0);
	double right = (Keys::get(KEY_DOWN) ? 1 : 0) + (Keys::get(KEY_UP) ? -1 : 0);

	posX += speed * forward * 0.8;
	posPhi -= speed * right / CAM_RADIUS * 1.2;
}

void Camera::setVectors(float ratio)
{
	glm::mat4x4 T = 
		glm::translate(glm::vec3(posX, 0, 0)) 
		* glm::rotate((float)(-posPhi), glm::vec3(1, 0, 0))
		* glm::translate(glm::vec3(0, 0, CAM_RADIUS))
		* glm::rotate((float)(M_PI / 180.0f * phi), glm::vec3(0, 1, 0))
		* glm::rotate((float)(M_PI / 180.0f * theta), glm::vec3(1, 0, 0));

	Perspective P(ratio);

	RenderData::UBO.Vectors.CameraPosition = T * glm::vec4(0, 0, 0, 1);
	RenderData::UBO.Vectors.CameraDirection = T * glm::vec4(0, 0, -1, 0);
	RenderData::UBO.Vectors.CameraRight = T * glm::vec4(P.tanFovX_2(), 0, 0, 0);
	RenderData::UBO.Vectors.CameraUp = T * glm::vec4(0, P.tanFovY_2(), 0, 0);

	RenderData::updateUBO(UBOB_VECTORS);
}

void Camera::captureMouse()
{
	readCursorNoMovement();
	glfwSetCursor(window, capturedCursor);
	cursorIsCaptured = true;
}

void Camera::freeMouse()
{
	glfwSetCursor(window, NULL);
	cursorIsCaptured = false;
}

//-----------------------------------------------------//

static const float radius = sin(M_PI / 3) / sin(M_PI / 16);
static const float rSin16 = radius * sin(M_PI / 16);
static const float rCos16 = radius * cos(M_PI / 16);
static const float rSin8 = radius * sin(M_PI / 8);
static const float rCos8 = radius * cos(M_PI / 8);
static const float rSin4 = radius * sin(M_PI / 4);
static const float rCos4 = radius * cos(M_PI / 4);

static const glm::vec3 atomMirrors[7][2] =
{
	{ glm::vec3(0.75, -0.5 * rSin16, 0.5 * rCos16 + 0.5 * radius), glm::vec3(-0.5, rSin16, radius - rCos16) },
	{ glm::vec3(0, -rSin16, rCos16), glm::vec3(0, cos(M_PI / 16), sin(M_PI / 16)) },
	{ glm::vec3(0, -rSin8, rCos8), glm::vec3(0, cos(M_PI / 8), sin(M_PI/ 8)) },
	{ glm::vec3(0, -rSin4, rCos4), glm::vec3(0, cos(M_PI / 4), sin(M_PI / 4)) },
	{ glm::vec3(0, -radius, 0), glm::vec3(0, 0, 1) },
	{ glm::vec3(0, 0, 0), glm::vec3(0, -1, 0) },
	{ glm::vec3(0, 0, 0), glm::vec3(1, 0, 0) }
};

static const glm::vec3 bondMirrors[7][2] =
{
	{ glm::vec3(0.5, 0, radius), glm::vec3(-1.5, rSin16, radius - rCos16) / glm::length(glm::vec3(-1.5, rSin16, radius - rCos16)) },
	{ glm::vec3(0, -rSin16, rCos16), glm::vec3(0, cos(M_PI / 16), sin(M_PI / 16)) },
	{ glm::vec3(0, -rSin8, rCos8), glm::vec3(0, cos(M_PI / 8), sin(M_PI / 8)) },
	{ glm::vec3(0, -rSin4, rCos4), glm::vec3(0, cos(M_PI / 4), sin(M_PI / 4)) },
	{ glm::vec3(0, -radius, 0), glm::vec3(0, 0, 1) },
	{ glm::vec3(0, 0, 0), glm::vec3(0, -1, 0) },
	{ glm::vec3(0, 0, 0), glm::vec3(1, 0, 0) }
};

float atomDistanceEstimator(glm::vec3 p)
{
	p.x += 1.5;
	p.x -= 3 * floor(p.x / 3);
	p.x -= 1.5;
	for (int j = 6; j >= 0; j--) {
		glm::vec3 md = p - atomMirrors[j][0];
		float l = glm::dot(md, atomMirrors[j][1]);
		if (l < 0) {
			p -= 2 * l * atomMirrors[j][1];
		}
	}
	return std::max(0.0, glm::length(p - glm::vec3(0.5, 0, radius)) - 0.25);
}

float bondDistanceEstimator(glm::vec3 p)
{
	p.x += 1.5;
	p.x -= 3 * floor(p.x / 3);
	p.x -= 1.5;
	for (int j = 6; j >= 1; j--) {
		glm::vec3 md = p - bondMirrors[j][0];
		float l = glm::dot(md, bondMirrors[j][1]);
		if (l < 0) {
			p -= 2 * l * bondMirrors[j][1];
		}
	}

	glm::vec3 c = p - glm::vec3(1.5, -rSin16, rCos16);
	float dAlt = std::max(length(c) - 0.5, glm::length(glm::vec2(c.y, c.z)) - 0.15);

	glm::vec3 md = p - bondMirrors[0][0];
	float l = glm::dot(md, bondMirrors[0][1]);
	if (l < 0) {
		p -= 2 * l * bondMirrors[0][1];
	}

	c = p - glm::vec3(0, 0, radius);
	return std::max(0.0f, std::min(dAlt, (float)std::max(length(c) - 0.5, glm::length(glm::vec2(c.y, c.z)) - 0.15)));
}

int Camera::findCentralHexagon()
{
	float screenRatio;
	float mouseX;
	float mouseY;
	findMouse(screenRatio, mouseX, mouseY);
	mouseX = mouseX * 2 - 1;
	mouseY = mouseY * 2 - 1;
	/// Ray and starting point
	glm::vec4 camPos = RenderData::UBO.Vectors.CameraPosition;
	glm::vec4 camDir = RenderData::UBO.Vectors.CameraDirection
		+ mouseX * RenderData::UBO.Vectors.CameraRight
		- mouseY * RenderData::UBO.Vectors.CameraUp;
	/// First check for collision with the cylinder
	
	// a * y + b * z = c
	// y * y + z * z = r * r
	float a = -camDir.z;
	float b = camDir.y;
	float c = camPos.y * a + camPos.z * b;

	bool flipYZ = false;

	if (std::abs(a) < 0.001) {
		if (std::abs(a) < 0.001) {
			return - 1;
		}
		else {
			flipYZ = true;
			b = -camDir.z;
			a = camDir.y;
		}
	}

	// y = c / a - b / a * z

	float d = c / a;
	float e = -b / a;

	// y = d + e * z
	// (d + e * z) * (d + e * z) + z * z = r * r
	// d * d + 2 * d * e + e * e * z * z + z * z = r * r 
	// z * z * (1 + e * e) + z * (2 * d * e) + d * d - r * r = 0

	float f = 1 + e * e;
	float g = 2 * d * e;
	float h = d * d - radius * radius;

	// z * z * f + z * g + h = 0

	g /= f;
	h /= f;

	// z * z + z * g + h = 0

	float i = 0.25 * g * g - h;

	if (i < 0) return -1;

	i = sqrt(i);
	float j = -g * 0.5;

	// z1/2 = j +- i

	float z1 = j + i;
	float z2 = j - i;

	float z0 = flipYZ ? -camPos.y : camPos.z;

	float l1 = (z0 - z1) / a;
	float l2 = (z0 - z2) / a;

	if (l1 < 0) return -1;

	glm::vec3 posFront = camPos + camDir * std::min(l1, l2);
	float depthCylinder = glm::length(posFront - glm::vec3(camPos));
	// Raymarch atoms
	if (STRUCTURE_BLOCKS_SELECTION) {
		glm::vec3 pos(camPos);
		glm::vec3 dir(camDir);
		for (int i = 0; i < 64; i++) {
			float dist = atomDistanceEstimator(pos);
			pos += dir * dist;
			if (dist > 10000) break;
		}
		if (atomDistanceEstimator(pos) < 0.5) {
			float depthAtoms = glm::length(pos - glm::vec3(camPos));
			if (depthAtoms < depthCylinder) return -1;
		}
		// Raymarch bonds
		pos = glm::vec3(camPos);
		dir = glm::vec3(camDir);
		for (int i = 0; i < 64; i++) {
			float dist = bondDistanceEstimator(pos);
			pos += dir * dist;
			if (dist > 10000) break;
		}
		if (bondDistanceEstimator(pos) < 0.5) {
			float depthBonds = glm::length(pos - glm::vec3(camPos));
			if (depthBonds < depthCylinder) return -1;
		}
	}
	// Figure out which hexagon was clicked
	//pos = posFront;
	float phi0 = (atan2(posFront.z, posFront.y) / M_PI) * 0.5;
	if (phi0 < 0) phi0 += 1;
	phi0 *= 16;
	float phi = phi0;
	float xSeg = 0.5 + posFront.x - 3 * floor((posFront.x + 0.5) / 3);

	int row = int(floor((posFront.x + 0.5) / 1.5));

	int num = 0;

	if (xSeg > 1.5) {
		phi += 0.5;
		xSeg -= 1.5;
		if (phi >= 16) phi -= 16;
	}
	if (xSeg < 1) {
		num = int(phi);
	}
	else {
		xSeg = 2 * (xSeg - 1);
		float h = 1 - 2 * abs(phi - floor(phi) - 0.5);
		if (xSeg < h) {
			num = int(phi);
		}
		else {
			num = int(floor(phi + 0.5));
			if ((row & 1) == 1) num -= 1;
			row += 1;
			if (num == -1) num = 15;
			if (num == 16) num = 0;
		}
	}
	int repeat = 16 * ROW_NUM;
	int hexId = (row * 16 + num)% repeat;
	if (hexId < 0) {
		hexId += repeat;
	}
	return hexId;
}

void Camera::findMouse(float &ratio, float &x, float &y)
{
	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	x = posX / width;
	y = posY / height;
}
