#ifndef JG_PERSPECTIVE_H
#define JG_PERSPECTIVE_H

#include <glm/gtc/type_ptr.hpp>

class Perspective {
protected:
	float _ratio;
	float _fovX;
	float _fovY;
	//glm::mat4x4 _mvp;
public:
	Perspective(float r);
	virtual ~Perspective();

	inline float ratio() { return _ratio; };
	inline float fovX() { return _fovX; };
	inline float fovY() { return _fovY; };
	//inline glm::mat4x4 mvp() { return _mvp; };
	inline float tanFovX_2() { return tan(0.5f * _fovX); };
	inline float tanFovY_2() { return tan(0.5f * _fovY); };
};

#endif
