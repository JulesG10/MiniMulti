#include "character.h"


Character::Character(CharacterAnimationMap* anim)
{
    this->animations = anim;

    this->size = { 0,0 };
    this->position = { 0,0 };
    this->state = CharacterState::IDLE;
    this->direction = Direction::DOWN;
    this->speed = 0;
    this->frame = 0;
}

Character::~Character()
{
}

void Character::Init()
{
}

void Character::UpdateFrame(std::vector<Collision*>&)
{
    this->frame += 5 * GetFrameTime();
    if (this->frame > ANIMATION_MAX_FRAMES)
    {
        this->frame = 0;
    }
    Texture& texture = (*this->animations)[this->state][this->direction][(int)this->frame];
    DrawTextureEx(texture, this->position, 0.f, 2.f, WHITE);
}

Vector2 Character::GetPosition()
{
    return this->position;
}

Vector2 Character::GetSize()
{
    return this->size;
}

NetCharacterObject Character::GetObject()
{
    return {
           (int)this->state,
           (int)this->direction,
           this->frame,
           this->speed,
           this->position.x, this->position.y
    };
}


void Character::LoadAnimation(CharacterAnimationMap* map, std::string name, CharacterState state)
{

    std::string apppath = std::string(GetApplicationDirectory()) + "\\assets\\" + name + "\\";
    std::string dirs[4] = { "up","down","left","right" };

    for (size_t i = 0; i < 4; i++)
    {
        Direction direction = (Direction)i;
        std::string dirpath = apppath + dirs[i];

        (*map)[state][direction][0] = LoadTexture((dirpath + "_0.png").c_str());
        (*map)[state][direction][1] = LoadTexture((dirpath + "_1.png").c_str());
        (*map)[state][direction][2] = LoadTexture((dirpath + "_2.png").c_str());
        (*map)[state][direction][3] = LoadTexture((dirpath + "_3.png").c_str());
    }
}