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
    if (!this->CheckCollision(p, s))
    {
        return p;
    }

    Vector2 fix = p;

    if (p.x < this->wall.x && p.x + s.x >= this->wall.x)
    {
        fix.x = this->wall.x - s.x;

    }
    else if (p.x + s.x >= this->wall.x + this->wall.width && p.x <= this->wall.x + this->wall.width)
    {
        fix.x = this->wall.x + this->wall.width;
    }

    if (p.y < this->wall.y && p.y + s.y >= this->wall.y)
    {
        fix.y = this->wall.y - s.y;
    }
    else if (p.y + s.y >= this->wall.y + this->wall.height && p.y <= this->wall.y + this->wall.height)
    {
        fix.y = this->wall.y + this->wall.height;
    }

    return fix;
}

bool Wall::CheckCollision(Vector2 pos, Vector2 size)
{
    return CheckCollisionRecs(this->wall, { pos.x, pos.y, size.x, size.y });
}
