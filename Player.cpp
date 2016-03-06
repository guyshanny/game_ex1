#include "Player.h"

mat4 Player::update(const Movements & movement)
{
	switch (movement)
	{
		case FORWARD:
			return _moveForward();
			break;
		case BACKWARD:
			return _moveBackward();
			break;
		case LEFT:
			return _turnLeft();
			break;
		case RIGHT:
			return _turnRight();
			break;
		case CROUCH:
			return _crouch();
			break;
		default:
			return _toWordMatrix;
			break;
	}
}

mat4 Player::_moveForward()
{
	vec3 tmpPos = _position + _direction*_movingSpeed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return EYE_MATRIX;
	}
	_position += _direction * _movingSpeed;

	return glm::translate(_toWordMatrix, _position);
}

mat4 Player::_moveBackward()
{
	vec3 tmpPos = _position - _direction*_movingSpeed;
	if (glm::abs(tmpPos.x) >= WINDOW_BOUNDARY.x || glm::abs(tmpPos.z) >= WINDOW_BOUNDARY.z)
	{
		return EYE_MATRIX;
	}

	_position -= _direction * _movingSpeed;
	return glm::translate(_toWordMatrix, _position);
}

mat4 Player::_turnLeft()
{
	_angle += _turnningSpeed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	return glm::mat4(1.0);
}

mat4 Player::_turnRight()
{
	_angle -= _turnningSpeed;
	_direction.x = glm::sin(_angle);
	_direction.z = -glm::cos(_angle);
	return glm::mat4(1.0);
}

mat4 Player::_crouch()
{
	return glm::mat4(1.0);
}

void Player::draw(const mat4& projection, const mat4& view)
{
}