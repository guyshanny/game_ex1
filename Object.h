#ifndef __OBJECT_H
#define __OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Globals.h"

using namespace glm;

class Object
{
protected:
	vec3 _position;
	vec3 _direction;
	vec3 _initialPosition;
	vec3 _initialDirection;
	mat4 _toWordMatrix;

public:
	Object();
	Object(const vec3& position, const vec3& direction, const mat4& toWorldMatrix);
	virtual ~Object() {};
	
	virtual void draw(const mat4& projection, const mat4& view) = 0;
	virtual void init() = 0;

	vec3 getPosition() { return _position; }
	vec3 getDirection() { return _direction; }
};

#endif
