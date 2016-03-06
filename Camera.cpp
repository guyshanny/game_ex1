#include "Camera.h"
#include "Globals.h"

Camera* Camera::_instance = NULL;

Camera::Camera() : _direction(0.0f, 0.0f, 1.0f),
				   _position(0.0f, -3.0f, -10.0f),
				   _up(0.0f, 1.0f, 0.0f),
				   _initialPosition(_position),
				   _initialDirection(_direction),
				   _movingSpeed(0.1f),
				   _turningSpeed(0.1f),
				   _crouchSpeed(0.1f)
{
}

void Camera::init(const float & angle, const float & windowBoundary)
{
	_angle = angle;
// 	_windowBoundary = vec3(windowBoundary-0.2f, _position.y+0.1f, windowBoundary-0.2f);
	_calculateViewMatrix();
}

void Camera::_calculateViewMatrix()
{
	_view = lookAt(_position, _position + _direction, _up);
}

#pragma region KeysHandling
mat4 Camera::moveForwardKeyPressed()
{
	vec3 tmpPos = _position + _direction*_movingSpeed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return EYE_MATRIX;
	}

	_position += _direction * _movingSpeed;
	_calculateViewMatrix();

	return _view;
}

mat4 Camera::moveBackwardKeyPressed()
{
	vec3 tmpPos = _position - _direction*_movingSpeed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return EYE_MATRIX;
	}

	_position -= _direction * _movingSpeed;
	_calculateViewMatrix();

	return _view;
}

mat4 Camera::turnLeftKeyPressed()
{
	_angle += _turningSpeed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	_calculateViewMatrix();

	return _view;
}

mat4 Camera::turnRightKeyPressed()
{
	_angle -= _turningSpeed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	_calculateViewMatrix();

	return _view;
}

mat4 Camera::crouchKeyPressed(const bool& isCrouch = true)
{
	// Inverse crouch
	if (false == isCrouch)
	{
		while (abs(_position.y) < abs(_initialPosition.y))
		{
			_position -= vec3(0.0f, _crouchSpeed, 0.0f);
			_calculateViewMatrix();
			glutPostRedisplay();
		}
	}
	// Crouch
	else
	{
		if (abs(_position.y) <= (WINDOW_BOUNDARY.y + 2.0f))
		{
			return EYE_MATRIX;
		}
		_position += vec3(0.0f, _crouchSpeed, 0.0f);
	}
	_calculateViewMatrix();
	return _view;
}

#pragma endregion
