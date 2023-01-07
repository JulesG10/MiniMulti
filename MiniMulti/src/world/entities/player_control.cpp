#include "player_control.h"


PlayerControl::PlayerControl(CharacterAnimationMap* anim) : Character(anim)
{
    this->keytime = 0;
    this->size = { 16 * 2, 24 * 2 };
}

PlayerControl::~PlayerControl()
{
}

void PlayerControl::Init()
{

}

void PlayerControl::UpdateFrame(std::vector<Collision*>& colobjs)
{
    Character::UpdateFrame(colobjs);
#ifdef _DEBUG
    DrawRectangleLines(this->position.x, this->position.y, this->size.x, this->size.y, RED);
#endif
    if (this->OnKeyDown(KEY_UP, Direction::UP, &this->position.y, -1))
    {
    }
    else if (this->OnKeyDown(KEY_DOWN, Direction::DOWN, &this->position.y, 1))
    {
    }
    else if (this->OnKeyDown(KEY_LEFT, Direction::LEFT, &this->position.x, -1))
    {
    }
    else if (this->OnKeyDown(KEY_RIGHT, Direction::RIGHT, &this->position.x, 1))
    {
    }

    for (Collision* col : colobjs)
    {
        if (col->CheckCollision(this->position, this->size))
        {
            this->position = col->FixCollision(this->position, this->size);

            this->keytime = 0;
            this->speed = 0;
            this->state = CharacterState::IDLE;
            break;
        }
    }
}

bool PlayerControl::OnKeyDown(int key, Direction dir, float* move, float factor)
{
    if (IsKeyDown(key))
    {
        this->keytime += 100 * GetFrameTime();
        if (this->direction != dir)
        {
            if (this->state == CharacterState::IDLE)
            {
                this->speed = 0;
                this->keytime = 0;
            }
            this->direction = dir;
        }
        else if (this->keytime > 0)
        {
            if (IsKeyDown(KEY_RIGHT_CONTROL))
            {
                this->state = CharacterState::RUN;
                this->speed = 300;
            }
            else if (IsKeyDown(KEY_RIGHT_SHIFT))
            {
                this->state = CharacterState::PUSH;
                this->speed = 150;
            }
            else if (this->state != CharacterState::WALK) {

                this->state = CharacterState::WALK;
                this->speed = 200;
            }


            *move += factor * this->speed * GetFrameTime();
        }
        return true;
    }
    else if (this->direction == dir && IsKeyUp(key))
    {
        this->keytime = 0;
        this->speed = 0;
        this->state = CharacterState::IDLE;
    }
    return false;
}
