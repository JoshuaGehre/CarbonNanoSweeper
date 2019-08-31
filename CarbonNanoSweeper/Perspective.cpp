#include "Perspective.h"

#include "glm/gtx/transform.hpp"

#include "DTGlobal.h"

static const float idealRatio = 1.75;
static const float idealFOVY = 3.1415926 / 180.0 * 80; //80 degrees to radians
static const float idealDistance = 1 / (2.0 * tan(0.5 * idealFOVY));
static const float idealFOVX = 2 * atan(idealRatio / (2 * idealDistance));

Perspective::Perspective(float r)
{
	_ratio = r;
	if (_ratio <= idealRatio) {
		//_mvp = glm::perspective(idealFOVY, _ratio, PROJ_NEAR, PROJ_FAR);
		_fovY = idealFOVY;
		_fovX = 2 * atan(tan(0.5 * idealFOVY) * _ratio);
	}
	else {
		_fovX = idealFOVX;
		_fovY = 2 * atan(tan(0.5 * idealFOVX) / _ratio);
		//_mvp = glm::perspective(_fovY, _ratio, PROJ_NEAR, PROJ_FAR);
	}
}

Perspective::~Perspective()
{
}