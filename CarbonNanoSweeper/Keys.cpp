#include "Keys.h"
#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

Keys * Keys::A = NULL;

Keys::Keys()
{
	length = 0;
	bindings = NULL;
	state = false;
}

Keys::~Keys()
{
	clear();
}

void Keys::clear()
{
	if (length == 0) return;
	delete[] bindings;
	bindings = NULL;
	length = 0;
	state = false;
}

bool Keys::addKey(int k)
{
	if (length == 0) {
		// Add as a the only key
		bindings = new int[1];
		bindings[0] = k;
		length = 1;
		return true;
	}
	else {
		int i;
		for (i = 0; i < length; i++) {
			// Exit if key was allready added
			if (bindings[i] == k) return false;
		}
		// Copy previous bindings to new array
		int * H = new int[length + 1];
		for (i = 0; i < length; i++) {
			H[i] = bindings[i];
		}
		// Add new key
		H[length] = k;
		delete[] bindings;
		bindings = H;
		length++;
	}
}

bool Keys::removeKey(int k)
{
	int i;
	int loc = -1;
	//Search key
	for (i = 0; i < length; i++) {
		if (bindings[i] == k) {
			loc = i;
			i = length;
		}
	}
	// Exit if key wasn't found
	if (loc == -1) return false;
	state = false;
	if (length == 1) {
		// Clear the enitre array
		clear();
		return true;
	}
	// Copy to new array except for the key to removes
	int * H = new int[length - 1];
	for (i = 0; i < length; i++) {
		if (i < loc) {
			H[i] = bindings[i];
		}
		else if (i > loc) {
			H[i - 1] = bindings[i];
		}
	}
	delete[] bindings;
	bindings = H;
	length--;
	return true;
}

void Keys::init()
{
	// Create Key array if it does not exist
	if (A != NULL) {
		std::cout << "Keys have allready been constructed!\n";
		return;
	}
	A = new Keys[_KEY_COUNT];
	// Assign default key bindings
	// Left
	A[KEY_LEFT].addKey(GLFW_KEY_A);
	A[KEY_LEFT].addKey(GLFW_KEY_LEFT);
	// Right
	A[KEY_RIGHT].addKey(GLFW_KEY_D);
	A[KEY_RIGHT].addKey(GLFW_KEY_RIGHT);
	// Up
	A[KEY_UP].addKey(GLFW_KEY_W);
	A[KEY_UP].addKey(GLFW_KEY_UP);
	// Down
	A[KEY_DOWN].addKey(GLFW_KEY_S);
	A[KEY_DOWN].addKey(GLFW_KEY_DOWN);
	// Space
	A[KEY_SPACE].addKey(GLFW_KEY_SPACE);
	// Look Left
	A[KEY_LOOK_LEFT].addKey(GLFW_KEY_Q);
	// look Right
	A[KEY_LOOK_RIGHT].addKey(GLFW_KEY_E);
	// Alternatively load custom key bidings or save if there is no key binding file
	//...
}

void Keys::end()
{
	//Destroy key array if it still exists
	if (A == NULL) {
		std::cout << "Keys have allready been destroyed or were never created!\n";
		return;
	}
	delete[] A;
	A = NULL;
}

bool Keys::get(int i)
{
	// Return state of the key or false if there is no such key
	if ((i < 0) || (i >= _KEY_COUNT)) return false;
	return A[i].state;
}

void Keys::onCallback(int key, int scancode, int action, int mods)
{
	// Exit if keys have not yet been initialized
	if (A == NULL) {
		std::cout << "Keys have not been intialized or been destroyed!\n\tIgnoring input!\n";
		return;
	}
	// actions:
	//   0 - Released
	//   1 - Pressed
	//   2 - Held down
	bool newState = (action > 0);
	int i;
	int j;
	// Check all keys
	for (i = 0; i < _KEY_COUNT; i++) {
		// Check all bindings
		for (j = 0; j < A[i].length; j++) {
			if (A[i].bindings[j] == key) {
				// Update key state and skip to next
				A[i].state = newState;
				j = A[i].length;
			}
		}
	}
}
