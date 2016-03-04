#include "Object.h"

Object::Object() : _position(0.0f), _direction(0.0f), _initialPosition(0.0f), _initialDirection(0.0f), _toWordMatrix(0.0f)
{
}

Object::Object(const vec3 & position, const vec3 & direction, const mat4 & toWorldMatrix) : 
	_position(position), _direction(direction), _initialPosition(position), _initialDirection(direction), _toWordMatrix(toWorldMatrix)
{
}
