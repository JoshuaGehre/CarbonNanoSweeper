#ifndef JG_KEYS_H
#define JG_KEYS_H

// Refer to the keys using these codes
#define KEY_LEFT 0
#define KEY_RIGHT 1
#define KEY_UP 2
#define KEY_DOWN 3
#define KEY_SPACE 4
#define KEY_LOOK_LEFT 5
#define KEY_LOOK_RIGHT 6
// 1 bigger then the last Key Id
#define _KEY_COUNT 7

class Keys final{
private:
	// Only create these objects within the static methods of this class
	Keys();
	static Keys * A;
	int * bindings;
	int length;
	bool state;
public:
	~Keys();
	// Remove all bindings
	void clear();
	// Add/remove binding return true if bindings were changes
	bool addKey(int k);
	bool removeKey(int k);
	// Create array of keys when the game starts
	static void init();
	// Destroy key array when game is quit
	static void end();
	// Return state of a key
	static bool get(int i);
	// Called when key is pressed / released / held down
	static void onCallback(int key, int scancode, int action, int mods);
};

#endif
