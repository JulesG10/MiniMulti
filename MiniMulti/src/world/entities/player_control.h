#pragma once

#include "character.h"
class PlayerControl : public Character {
public:
    PlayerControl(CharacterAnimationMap*);
    ~PlayerControl();

    void Init() override;
    void UpdateFrame(std::vector<Collision*>&) override;
private:
    float keytime;
    bool OnKeyDown(int key, Direction dir, float* move, float factor);
};

