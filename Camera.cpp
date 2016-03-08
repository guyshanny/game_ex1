#include "Camera.h"
#include "Globals.h"

Camera* Camera::_instance = NULL;

Camera::Camera() : _direction(0.0f, 0.0f, 1.0f),
				   _position(0.0f, -3.0f, -10.0f),
				   _up(0.0f, 1.0f, 0.0f),
				   _initialPosition(_position),
				   _initialDirection(_direction),
				   _speed(0.1f),
				   _zoomSavedPosition(0.0f),
				   _isZoomSavedPosition(false),
				   _initialSpeed(_speed)
{
}

void Camera::init(const float & angle, const float & windowBoundary)
{
	_angle = angle;
// 	_windowBoundary = vec3(windowBoundary-0.2f, _position.y+0.1f, windowBoundary-0.2f);
	_calculateViewMatrix();
}

void Camera::update()
{
	/************************************************************************/
	/* All regular controls                                                 */
	/************************************************************************/
	if (_commands[Commands::MOVE_FORWARD]) { _moveForwardLogic(); }
	if (_commands[Commands::MOVE_BACKWARD]) { _moveBackwardLogic(); }
	if (_commands[Commands::TURN_RIGHT]) { _turnRightLogic(); }
	if (_commands[Commands::TURN_LEFT]) { _turnLeftLogic(); }
	if (_commands[Commands::STRAFE_RIGHT]) { _strafeRightLogic(); }
	if (_commands[Commands::STRAFE_LEFT]) { _strafeLeftLogic(); }
	if (_commands[Commands::RESET]) { _resetLogic(); }
	if (_commands[Commands::TELEPORT]) { _teleportLogic(); }

	/************************************************************************/
	/* Smooth controls                                                      */
	/************************************************************************/
	// Forward crouch
	if (_commands[Commands::CROUCH] && false == _commands[Commands::CROUCH_RETURN])
	{
		_crouchLogic(true);
	}
	// Backward crouch
	else if (false == _commands[Commands::CROUCH] && _commands[Commands::CROUCH_RETURN])
	{
		_crouchLogic(false);
		glutPostRedisplay();
	}
	else
	{
		_commands[Commands::CROUCH] = false;
		_commands[Commands::CROUCH_RETURN] = false;
	}

	// Forward jump
	if (_commands[Commands::JUMP] && false == _commands[Commands::JUMP_RETURN])
	{
		_jumpLogic(true);
		glutPostRedisplay();
	}
	// Backward jump
	else if (false == _commands[Commands::JUMP] && _commands[Commands::JUMP_RETURN])
	{
		_jumpLogic(false);
		glutPostRedisplay();
	}
	else
	{
		_commands[Commands::JUMP] = false;
		_commands[Commands::JUMP_RETURN] = false;
	}

	// Zoom in
	if (_commands[Commands::ZOOM_IN] && false == _commands[Commands::ZOOM_OUT])
	{
		_zoomLogic(true);
		glutPostRedisplay();
	}
	// Zoom out
	else if (false == _commands[Commands::ZOOM_IN] && _commands[Commands::ZOOM_OUT])
	{
		_zoomLogic(false);
		glutPostRedisplay();
	}
	else
	{
		_commands[Commands::ZOOM_IN] = false;
		_commands[Commands::ZOOM_OUT] = false;
	}
}

mat4 Camera::_calculateViewMatrix()
{
	_view = lookAt(_position, _position + _direction, _up);
	return _view;
}

#pragma region KeysHandling

void Camera::moveForward() { _commands[Commands::MOVE_FORWARD] = true; }
void Camera::moveBackward() { _commands[Commands::MOVE_BACKWARD] = true; }
void Camera::turnLeft() { _commands[Commands::TURN_LEFT] = true; }
void Camera::turnRight() { _commands[Commands::TURN_RIGHT] = true; }
void Camera::strafeRight() { _commands[Commands::STRAFE_RIGHT] = true; }
void Camera::strafeLeft() { _commands[Commands::STRAFE_LEFT] = true; }
void Camera::reset() { _commands[Commands::RESET] = true; }

void Camera::crouch(const bool& isCrouch)
{
	if (isCrouch)
	{
		_commands[Commands::CROUCH] = true;
	}
	else
	{
		_commands[Commands::CROUCH_RETURN] = true;
	}
}

void Camera::jump(const bool& isJump)
{
	if (isJump)
	{
		_commands[Commands::JUMP] = true;
	}
	else
	{
		_commands[Commands::JUMP_RETURN] = true;
	}
}

void Camera::zoom(const bool & isZoomIn)
{
	if (isZoomIn)
	{
		_commands[Commands::ZOOM_IN] = true;
	}
	else
	{
		_commands[Commands::ZOOM_OUT] = true;
	}
}

void Camera::teleport()
{
	_commands[Commands::TELEPORT] = true;
}

void Camera::fastMove(const bool & isShiftPressed)
{
	if (isShiftPressed)
	{
		if (_speed <= _initialSpeed)
		{
			_speed *= 4;
		}
	}
	else
	{
		_speed = _initialSpeed;
	}
}

void Camera::_moveForwardLogic()
{
	_commands[Commands::MOVE_FORWARD] = false;

	vec3 tmpPos = _position + _direction*_speed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return;
	}

	_position += _direction * _speed;
	_calculateViewMatrix();
}

void Camera::_moveBackwardLogic()
{
	_commands[Commands::MOVE_BACKWARD] = false;

	vec3 tmpPos = _position - _direction*_speed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return;
	}

	_position -= _direction * _speed;
	_calculateViewMatrix();
}

void Camera::_turnLeftLogic()
{
	_commands[Commands::TURN_LEFT] = false;

	_angle += _speed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	_calculateViewMatrix();
}

void Camera::_turnRightLogic()
{
	_commands[Commands::TURN_RIGHT] = false;

	_angle -= _speed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	_calculateViewMatrix();
}

void Camera::_crouchLogic(const bool& isCrouch)
{
	// Crouch
	if (isCrouch)
	{
		_commands[Commands::CROUCH] = false;
		vec3 tmpPosition = _position + vec3(0.0f, _speed, 0.0f);
		if (tmpPosition.y > -2.0f)
		{
			return;
		}
		_position = tmpPosition;
	}
	// Inverse crouch
	else
	{
		vec3 tmpPosition = _position - vec3(0.0f, _speed/200, 0.0f);
		if (tmpPosition.y < _initialPosition.y)
		{
			_commands[Commands::CROUCH_RETURN] = false;
			return;
		}
		_position = tmpPosition;
	}
	_calculateViewMatrix();
}

void Camera::_jumpLogic(const bool& isJump)
{
	// Jump
	if (isJump)
	{
		vec3 tmpPosition = _position - vec3(0.0f, _speed/10, 0.0f);
		if (tmpPosition.y < WINDOW_BOUNDARY.y)
		{
			_commands[Commands::JUMP] = false;
			_commands[Commands::JUMP_RETURN] = true;
			return;
		}
		_position = tmpPosition;
	}
	// Inverse jump
	else
	{
		vec3 tmpPosition = _position + vec3(0.0f, _speed/100, 0.0f);
		if (tmpPosition.y > _initialPosition.y)
		{
			_commands[Commands::JUMP_RETURN] = false;
			return;
		}
		_position = tmpPosition;
	}
	_calculateViewMatrix();
}

void Camera::_zoomLogic(const bool & isZoomIn)
{
	// Zoom in
	if (isZoomIn)
	{
		_commands[Commands::ZOOM_IN] = false;

		if (false == _isZoomSavedPosition)
		{
			_zoomSavedPosition = _position;
			_isZoomSavedPosition = true;
		}
		
		vec3 tmpPosition = _position + _direction * (_speed*6);
		float xDiff = glm::abs(glm::abs(tmpPosition.x) - glm::abs(_zoomSavedPosition.x));
		float zDiff = glm::abs(glm::abs(tmpPosition.z) - glm::abs(_zoomSavedPosition.z));
		if ((xDiff > 5.0f) || (zDiff > 5.0f) || 
			(glm::abs(tmpPosition.x) >= WINDOW_BOUNDARY.x) || (glm::abs(tmpPosition.z) >= WINDOW_BOUNDARY.z))
		{
			return;
		}
		_position = tmpPosition;
	}
	// Zoom out
	else
	{
		vec3 tmpPosition = _position - _direction * (_speed/10);
		float xDiff = glm::abs(glm::abs(tmpPosition.x) - glm::abs(_zoomSavedPosition.x));
		float zDiff = glm::abs(glm::abs(tmpPosition.z) - glm::abs(_zoomSavedPosition.z));
		if (xDiff < 0.1f && zDiff < 0.1f)
		{
			_isZoomSavedPosition = false;
			_commands[Commands::ZOOM_OUT] = false;
			return;
		}
		_position = tmpPosition;
	}
	_calculateViewMatrix();
}

void Camera::_teleportLogic()
{
	_commands[Commands::TELEPORT] = false;

	// Linear random
	//		((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	_position = vec3(((float(rand()) / float(RAND_MAX)) * (WINDOW_BOUNDARY.x + WINDOW_BOUNDARY.x)) - WINDOW_BOUNDARY.x,
					_position.y,
					((float(rand()) / float(RAND_MAX)) * (WINDOW_BOUNDARY.z + WINDOW_BOUNDARY.z)) - WINDOW_BOUNDARY.z);
	_calculateViewMatrix();
}

void Camera::_strafeRightLogic()
{
	_commands[Commands::STRAFE_RIGHT] = false;

	vec3 dirNormal = glm::normalize(glm::cross(_direction, _up));
	vec3 tmpPos = _position - dirNormal * _speed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return;
	}
	_position -= dirNormal * _speed;
	_calculateViewMatrix();
}

void Camera::_strafeLeftLogic()
{
	_commands[Commands::STRAFE_LEFT] = false;

	vec3 dirNormal = glm::normalize(glm::cross(_direction, _up));
	vec3 tmpPos = _position + dirNormal * _speed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return;
	}
	_position += dirNormal * _speed;
	_calculateViewMatrix();
}

void Camera::_resetLogic()
{
	_commands[Commands::RESET] = false;

	_direction = vec3(0.0f, 0.0f, 1.0f);
	_position = vec3(0.0f, -3.0f, -10.0f);
	_calculateViewMatrix();
}

#pragma endregion


