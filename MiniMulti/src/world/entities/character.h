#pragma once

#include "../../stdafx.h"
#include "../collision/collision.h"

#define ANIMATION_MAX_FRAMES 4
enum CharacterState {
    IDLE,
    WALK,
    RUN,
    PUSH
};

typedef std::unordered_map<CharacterState, std::unordered_map<Direction, Texture[ANIMATION_MAX_FRAMES]>> CharacterAnimationMap;

class Character {
public:
    Character(CharacterAnimationMap*);
    ~Character();

    virtual void Init();
    virtual void UpdateFrame(std::vector<Collision*>&);

    Vector2 GetPosition();
    Vector2 GetSize();

    NetCharacterObject GetObject();

    static void LoadAnimation(CharacterAnimationMap* map, std::string name, CharacterState state);
protected:
    CharacterState state;
    float frame;

    float speed;
    Direction direction;

    Vector2 position;
    Vector2 size;

private:
    CharacterAnimationMap* animations;
};