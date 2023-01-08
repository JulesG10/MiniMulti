#include "world.h"


World::World(std::weak_ptr<CNetClient> client,time_t id)
{
    this->m_client = client.lock();
    this->m_clientId = id;
    this->m_latency = 0;
    this->m_clientTime = 0;
    this->player_animations = new CharacterAnimationMap();
}

World::~World()
{
    for (auto& anim : *this->player_animations)
    {
        for (size_t i = 0; i < 4; i++)
        {
            UnloadTexture(anim.second[(Direction)i][0]);
            UnloadTexture(anim.second[(Direction)i][1]);
            UnloadTexture(anim.second[(Direction)i][2]);
            UnloadTexture(anim.second[(Direction)i][3]);
        }
    }
    
    delete this->player_animations;
}

void World::Init()
{
    Character::LoadAnimation(this->player_animations, "default\\idle", CharacterState::IDLE);
    Character::LoadAnimation(this->player_animations, "default\\walk", CharacterState::WALK);
    Character::LoadAnimation(this->player_animations, "default\\run", CharacterState::RUN);
    Character::LoadAnimation(this->player_animations, "default\\push", CharacterState::PUSH);

    this->player = std::make_unique<PlayerControl>(this->player_animations);
    this->player->Init();

    this->camera = {
       Vector2Subtract($util->screen_midle(), Vector2Divide(this->player->GetSize(), {2.f,2.f})),
       {0, 0},
       0.f,
       1.f
    };

    this->gsize = 64;
    this->gborder = this->gsize * 2;
    this->gmax_x = (int)((this->camera.offset.x + this->gborder) / this->gsize);
    this->gmax_y = (int)((this->camera.offset.x + this->gborder) / this->gsize);


    Wall* w1 = new Wall({ 100, 100, 20, 200 });
    this->m_collisionobjs.push_back(w1);

    Wall* w2 = new Wall({ 50, 0, 200, 20 });
    this->m_collisionobjs.push_back(w2);

}

void World::UpdateFrame()
{
    this->camera.target = this->player->GetPosition();
    DrawRectangle(0, 0, $util->render_size().x, $util->render_size().y, RAYWHITE);
    BeginMode2D(this->camera);
    this->DrawGrid({ 130, 130 , 130, 50 });

    this->player->UpdateFrame(this->m_collisionobjs);

    auto it = this->m_players.begin();
    while (it != this->m_players.end())
    {
        if (it->second->Expired())
        {
            it = this->m_players.erase(it);
        }
        else {
            it->second->UpdateFrame(this->m_collisionobjs);
            ++it;
        }
    }

    for (Collision* obj : this->m_collisionobjs)
    {
        obj->UpdateFrame();
    }

    this->UpdateNetDate();

    EndMode2D();

    DrawFPS(10, 10);
    DrawText((std::to_string(this->m_latency) + "ms [IN]").c_str(), 10, 30, 20, BLACK);

}

void World::DrawGrid(Color c)
{
    for (int x = -this->gmax_x; x < this->gmax_x; x++)
    {
        int px = (int)((this->camera.target.x / this->gsize) + x) * this->gsize;
        for (int y = -this->gmax_y; y < this->gmax_y; y++)
        {
            int py = (int)((this->camera.target.y / this->gsize) + y) * this->gsize;
            DrawRectangleLines(px, py, this->gsize, this->gsize, c);
        }
    }
}

void World::StartQueueThread()
{
    std::thread([&]() {
        while (this->m_client->is_alive())
        {
            if (this->m_client->get_queue_size() > 0)
            { 
                CNetBuffer buffer;
                this->m_client->recieve(buffer);
                this->HandleNetData((NetData*)buffer);
                buffer.clear();
            }
        }
    }).detach();
}

void World::HandleNetData(NetData* data)
{
    auto dtime = std::chrono::time_point<std::chrono::high_resolution_clock>() + std::chrono::milliseconds(data->time);
    auto now = std::chrono::high_resolution_clock::now();
    
    this->m_latency = std::chrono::duration_cast<std::chrono::milliseconds>(now - dtime).count();

    this->m_mutex.lock();
    if (this->m_players.count(data->id) > 0)
    {
        this->m_players.at(data->id)->Set(data->object);
    }
    else {
        auto newplayer = std::pair<time_t, std::shared_ptr<PlayerNet>>(data->id, std::make_shared<PlayerNet>(this->player_animations, data->object));
        this->m_players.insert(newplayer);
    }
    this->m_mutex.unlock();
}

void World::UpdateNetDate()
{
    NetData data = {};
    data.id = this->m_clientId;

    auto now = std::chrono::high_resolution_clock::now();
    data.time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    data.object = this->player->GetObject();

    CNetBuffer buffer(&data);
    int w = 0;
    this->m_client->send(buffer, &w);
    buffer.clear();
}
