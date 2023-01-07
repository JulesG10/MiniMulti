#pragma once
#include "collision.h"

class Wall : public Collision
{
public:
	Wall(Rectangle);
	~Wall();

	void UpdateFrame() override;
	Vector2 FixCollision(Vector2, Vector2) override;
	bool CheckCollision(Vector2, Vector2) override;
private:
	Rectangle wall;
};

