#ifndef __CAMERA_H
#define __CAMERA_H


#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

class Camera
{
private:
	vec3 _direction;
	vec3 _position;
	vec3 _up;
	vec3 _initialDirection;
	vec3 _initialPosition;

	mat4 _view;

	// Moving camera properties
	float _movingSpeed;
	float _turningSpeed;
	float _angle;
	float _crouchSpeed;

	Camera();
	static Camera* _instance;

	void Camera::_calculateViewMatrix();

public:
	static Camera* instance()
	{
		if (NULL == _instance)
		{
			_instance = new Camera();
		}
		return _instance;
	}
	void init(const float& angle, const float& windowBoundary);

	// Getters & setters
	mat4 getViewMatrix() { return _view; }

	// Key functions
	mat4 moveForwardKeyPressed();
	mat4 moveBackwardKeyPressed();
	mat4 turnLeftKeyPressed();
	mat4 turnRightKeyPressed();
	mat4 crouchKeyPressed(const bool& isCrouch);
};

#endif