#include "stdafx.h"
#include "gui.h"
#include "world/world.h"


class Game
{
public:
    Game(int argc, char **argv);
    ~Game();

    int Start();

private:
    void CreateConsole();
    void CreateUI();

    void UpdateFrame();
    void LoadingFrame();
    void StartConnectionThread(int delay);

    CNetStatus m_clientStatus;
    CNetStatus m_serverStatus;
    
    time_t m_clientId;
    std::shared_ptr<CNetClient> m_client;
    std::shared_ptr<CNetServer> m_server;
    std::thread m_connThread;
    CNetProps m_props;

    std::shared_ptr<World> m_world;

    std::unordered_map<std::string, std::string> m_args;
    RenderTexture m_render;

    std::shared_ptr<Loader> m_connAnim;
    std::shared_ptr<Text> m_connMsg;
    std::shared_ptr<Button> m_connNext;

    bool m_connNextClick;
    bool m_connChange;
    bool m_loadingPage;
    Color m_background;

    const Color m_defbg = {50, 100, 220, 255};
    const Color m_errbg = {220, 100, 50, 255};
    const Color m_valbg = {50, 220, 100, 255};
};
