#pragma once

#include "../../stdafx.h"

class Collision
{
public:
	Collision();
	~Collision();

	virtual void UpdateFrame();
	virtual Vector2 FixCollision(Vector2, Vector2);
	virtual bool CheckCollision(Vector2, Vector2);
};

