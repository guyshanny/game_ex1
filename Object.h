#ifndef __OBJECT_H
#define __OBJECT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Globals.h"

using namespace glm;

class Object
{
protected:
	GLuint _vao, _vbo;

	vec3 _position;
	vec3 _direction;
	vec3 _initialPosition;
	vec3 _initialDirection;
	mat4 _toWordMatrix;

	Object();
	Object(const vec3 & position, const vec3 & direction, const mat4 & toWorldMatrix, const GLuint& vao, const GLint& vbo);
	
public:
	virtual ~Object() {};

	// Updates the object's params if needed (does in each render frame)
	virtual void update() = 0;

	// Draws the object
	virtual void draw(const mat4& projection, const mat4& view) = 0;
	virtual void init() = 0;

	vec3 getPosition() { return _position; }
	vec3 getDirection() { return _direction; }
};

#endif
