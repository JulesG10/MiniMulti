#include "player_net.h"


PlayerNet::PlayerNet(CharacterAnimationMap* anim, NetCharacterObject obj) : Character(anim)
{
    this->Set(obj);
}

PlayerNet::PlayerNet(CharacterAnimationMap* anim) : Character(anim)
{
}

PlayerNet::~PlayerNet()
{
}

void PlayerNet::Init()
{
}

void PlayerNet::UpdateFrame(std::vector<Collision*>& col)
{
    Character::UpdateFrame(col);
#ifdef _DEBUG
        DrawRectangleLines(this->position.x, this->position.y, this->size.x, this->size.y, GREEN);
#endif

    switch (this->direction)
    {
    case Direction::LEFT:
        this->position.x -= this->speed * GetFrameTime();
        break;
    case Direction::RIGHT:
        this->position.x += this->speed * GetFrameTime();
        break;
    case Direction::UP:
        this->position.y -= this->speed * GetFrameTime();
        break;
    case Direction::DOWN:
        this->position.y += this->speed * GetFrameTime();
        break;
    }
}

void PlayerNet::Set(NetCharacterObject obj)
{
    this->speed = obj.speed;
    this->direction = (Direction)obj.direction;
    this->state = (CharacterState)obj.state;
    this->frame = obj.frame;
    this->position.x = obj.x;
    this->position.y = obj.y;

    this->last_update = std::chrono::high_resolution_clock::now();
}

bool PlayerNet::Expired()
{
    std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
    int diff = std::chrono::duration_cast<std::chrono::seconds>(now - this->last_update).count();
    return diff >= 10;
}
