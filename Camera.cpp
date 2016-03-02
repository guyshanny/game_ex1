#include "Camera.h"

Camera* Camera::_instance = NULL;

Camera::Camera() : _dir(0.0f, 0.0f, 1.0f),
				   _pos(0.0f, -3.0f, -10.0f),
				   _up(0.0f, 1.0f, 0.0f),
				   _world(1.0f),
				   _movingSpeed(0.1f),
				   _turningSpeed(0.1f)
{
	_projection = perspective(5.0f, 1.0f, 0.1f, 100.0f);
}

void Camera::init(const float & angle, const float & windowBoundary)
{
	_angle = angle;
	_windowBoundary = windowBoundary-0.2f;
	calculateWVP();
}

void Camera::calculateWVP()
{
	_view = lookAt(_pos, _pos + _dir, _up);
	_wvp = _projection * _view * _world;
}

void Camera::moveForward()
{
	vec3 tmpPos = _pos + _dir*_movingSpeed;
	if (glm::abs(tmpPos.x) >= _windowBoundary || glm::abs(tmpPos.z) >= _windowBoundary)
	{
		return;
	}

	_pos += _dir * _movingSpeed; 
	calculateWVP();
}

void Camera::moveBackward()
{
	vec3 tmpPos = _pos - _dir*_movingSpeed;
	if (glm::abs(tmpPos.x) >= _windowBoundary || glm::abs(tmpPos.z) >= _windowBoundary)
	{
		return;
	}

	_pos -= _dir * _movingSpeed;
	calculateWVP();
}

void Camera::turnLeft()
{
	_angle += _turningSpeed;
	_dir.x = glm::sin(_angle);
	_dir.z = -glm::cos(_angle);
	calculateWVP();
}

void Camera::turnRight()
{
	_angle -= _turningSpeed;
	_dir.x = glm::sin(_angle);
	_dir.z = -glm::cos(_angle);
	calculateWVP();
}




