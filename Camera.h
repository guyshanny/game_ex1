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
	vec3 _dir;
	vec3 _pos;
	vec3 _up;

	mat4 _world;
	mat4 _view;
	mat4 _projection;
	mat4 _wvp;

	// Moving camera properties
	float _movingSpeed;
	float _turningSpeed;
	float _angle;
	float _windowBoundary;

	Camera();
	static Camera* _instance;

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

	void Camera::calculateWVP();
	mat4 getWVP() { return _wvp; }

	// Update camera functions
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();


};

#endif