#pragma once

#include "../stdafx.h"
#include "../gui.h"

#include "collision/collision.h"
#include "collision/wall.h"

#include "entities/character.h"
#include "entities/player_control.h"
#include "entities/player_net.h"

class World {
public:
    World(std::weak_ptr<CNetClient>, time_t);
    ~World();

    void Init();
    void StartQueueThread();
    void UpdateFrame();
private:
    void HandleNetData(NetData*);
    void UpdateNetDate();

    void DrawGrid(Color);

    int gsize;
    int gborder;
    int gmax_x;
    int gmax_y;

    CharacterAnimationMap* player_animations;
    std::unique_ptr<PlayerControl> player;
    Camera2D camera;

    std::unordered_map<time_t, std::shared_ptr<PlayerNet>> m_players;
    std::vector<Collision*> m_collisionobjs;
    std::shared_ptr<CNetClient> m_client;
    
    std::mutex m_mutex;

    time_t m_clientId;
    int m_clientTime;
    long long m_latency;
};


