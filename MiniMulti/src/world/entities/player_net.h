#pragma once
#include "character.h"

class PlayerNet : public Character {
public:
    PlayerNet(CharacterAnimationMap*);
    PlayerNet(CharacterAnimationMap*, NetCharacterObject);
    ~PlayerNet();

    void Init() override;
    void UpdateFrame(std::vector<Collision*>&) override;
    void Set(NetCharacterObject);

    bool Expired();
private:
    std::chrono::steady_clock::time_point last_update;
};

