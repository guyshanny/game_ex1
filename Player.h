#ifndef __PLAYER_H
#define __PLAYER_H

#include "Object.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Player : Object
{
private:
	// Movements
	float _movingSpeed;
	float _turnningSpeed;
	float _angle;
	mat4 _moveForward();
	mat4 _moveBackward();
	mat4 _turnLeft();
	mat4 _turnRight();
	mat4 _crouch();

public:
	enum Movements
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		CROUCH,
	};

	Player() : Object(), _movingSpeed(0.1f), _angle(M_PI) {}
	Player(const float& movingSpeed, 
		   const float& initialAngle) : Object(), _movingSpeed(movingSpeed), _angle(initialAngle) {}

	mat4 update(const Movements& movement);
	void draw(const mat4& projection, const mat4& view);
};

#endif