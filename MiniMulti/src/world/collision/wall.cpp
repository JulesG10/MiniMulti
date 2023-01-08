#include "wall.h"

Wall::Wall(Rectangle rectwall)
{
    this->wall = rectwall;
}

Wall::~Wall()
{
}

void Wall::UpdateFrame()
{
    DrawRectangleRec(this->wall, RAYWHITE);
    DrawRectangleLinesEx(this->wall, 1, GRAY);
}

Vector2 Wall::FixCollision(Vector2 p, Vector2 s)
{
    Vector2 fix = p;
    float dx = std::abs(p.x + s.x / 2 - (this->wall.x + this->wall.width / 2));
    float dy = std::abs(p.y + s.y / 2 - (this->wall.y + this->wall.height / 2));

    if (dx > dy)
    {
        if (p.y < this->wall.y)
        {
            fix.y = this->wall.y - s.y;
        }
        else
        {
            fix.y = this->wall.y + this->wall.height;
        }
    }
    else
    {
        if (p.x < this->wall.x)
        {
            fix.x = this->wall.x - s.x;
        }
        else
        {
            fix.x = this->wall.x + this->wall.width;
        }
    }

    return fix;
}

bool Wall::CheckCollision(Vector2 pos, Vector2 size)
{
    return CheckCollisionRecs(this->wall, { pos.x, pos.y, size.x, size.y });
}
