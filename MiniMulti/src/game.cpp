#include "game.h"

Game::Game(int argc, char **argv)
{
    if (argc >= 1 && argv != nullptr)
    {
        this->m_args = $util->parse_args(argc, argv);
    }
    else {
        this->m_args = {};
    }
}

Game::~Game()
{
}

int Game::Start()
{
    SetTraceLogLevel(LOG_NONE);
    std::string host = "127.0.0.1";
    if(this->m_args.count("--host") > 0)
    {
        host = this->m_args["--host"];
    }

    short port = 5530;
    if(this->m_args.count("--port") > 0)
    {
        port = std::atoi(this->m_args["--port"].c_str());
    }

    this->m_props = cnet_create_props((char*)host.c_str(), port);

    if (this->m_args.count("--console") > 0)
    {
        this->CreateConsole();
    }

    this->m_serverStatus = CNetStatus::CNET_NONE;
    if(this->m_args.count("--server") > 0)
    {
        this->m_server = std::make_shared<CNetServer>(this->m_props);
        this->m_serverStatus = this->m_server->start();
    
        if (this->m_args.count("--nogui") > 0)
        {
            this->CreateConsole();
            while (this->m_server->is_alive())
            {
                std::string line;
                std::cout << ">";
                std::getline(std::cin, line);
            
                if (line == "exit")
                {
                    break;
                }
                else if (line == "clients")
                {
                    std::cout << this->m_server->count_clients() << '\n';
                }
            }
            this->m_server->terminate();
            return 0;
        }
    }

#ifdef _DEBUG
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "MiniMulti");
#else
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "MiniMulti");
#endif
    SetTargetFPS(0);

    this->m_clientId = time(0);
    this->m_client = std::make_shared<CNetClient>(this->m_props);
    this->m_client->set_queue_max(10);

    this->m_render = LoadRenderTexture(800, 600);
    Rectangle renderRect = {0, 0, (float)this->m_render.texture.width, -(float)this->m_render.texture.height};
    Rectangle screenRect = {0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()};

    this->CreateUI();

    this->m_world = std::make_shared<World>(this->m_client, this->m_clientId);
    this->m_world->Init();

#ifdef _DEBUG
    this->StartConnectionThread(0);
#else
    this->StartConnectionThread(3);
#endif // _DEBUG
    while (!WindowShouldClose())
    {
        BeginTextureMode(this->m_render);
        ClearBackground(BLACK);
        // BeginShaderMode(shader);
        this->UpdateFrame();
        // EndShaderMode();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLANK);
        DrawTexturePro(this->m_render.texture, renderRect, screenRect, {0, 0}, 0, WHITE);
        EndDrawing();
    }

    if(this->m_server != NULL)
    {
        this->m_server->terminate();
    }
    this->m_client->terminate();

    return EXIT_SUCCESS;
}

void Game::CreateConsole()
{
    if (GetConsoleWindow() == NULL)
    {
        AllocConsole();
        SetConsoleTitleA("MiniMulti");
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
}

void Game::CreateUI()
{
    $util->set_rendersize({ (float)this->m_render.texture.width, (float)this->m_render.texture.height });

    this->m_connMsg = std::make_shared<Text>("Waiting for connection...",
        24,
        $util->percent({ 50, 35 }));
    this->m_connMsg->set_center(true, true);
    this->m_background = this->m_defbg;

    this->m_connNext = std::make_shared<Button>();
    this->m_connNext
        ->set_position($util->percent({ 50, 45 }))
        ->set_size(20)
        ->set_text("")
        ->set_margin(10)
        ->set_center(true, false);

    Vector2 animPos = $util->percent({ 50, 45 });
    Vector2 animSize = $util->percent({ 20, 3 });
    animPos.x -= animSize.x / 2.f;
    animPos.y -= animSize.y / 2.f;
    Rectangle animRect = { animPos.x, animPos.y,
                          animSize.x, animSize.y };
    this->m_connAnim = std::make_shared<Loader>(0.f, animRect);
}

void Game::UpdateFrame()
{
    if (this->m_loadingPage)
    {
        this->LoadingFrame();
    }
    else
    {
        this->m_world->UpdateFrame();
        if (!this->m_client->is_alive())
        {
            this->m_connMsg->set_text("Server Closed");
            this->m_connNext->set_text("Try to reconnect");
            this->m_background = this->m_errbg;
            this->m_loadingPage = true;
            this->m_clientStatus = CNetStatus::CNET_ERROR;
        }
    }
}

void Game::LoadingFrame()
{
    DrawRectangle(0, 0, this->m_render.texture.width, this->m_render.texture.height, this->m_background);
    this->m_connMsg->draw(WHITE);
    if (this->m_clientStatus == CNetStatus::CNET_NONE)
    {
        this->m_connAnim->update(60 * GetFrameTime());
        if (this->m_connAnim->m_value >= 100)
        {
            this->m_connAnim->m_value = 0;
        }
        this->m_connAnim->draw(WHITE, WHITE);
    }
    else if (this->m_clientStatus != CNetStatus::CNET_NONE && !this->m_connChange)
    {
        if (this->m_clientStatus == CNetStatus::CNET_ERROR)
        {
            this->m_connMsg->set_text("Connection failed");
            this->m_connNext->set_text("Try to reconnect");
            this->m_background = this->m_errbg;
        }
        else if (this->m_clientStatus == CNetStatus::CNET_OK)
        {
            this->m_connMsg->set_text("Connection success");
            this->m_connNext->set_text("Start game");
            this->m_background = this->m_valbg;
        }
        this->m_connChange = true;
    }
    else if (this->m_connChange)
    {
        if (this->m_connNext->is_hover($util->mouse()))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                this->m_connNext->set_margin(8);
                this->m_connNextClick = true;
            }
            else
            {
                if (this->m_connNextClick && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
                {
                    this->m_connNextClick = false;
                    if (this->m_clientStatus == CNetStatus::CNET_OK)
                    {
                        this->m_loadingPage = false;
                    }
                    else
                    {

                        this->m_connMsg->set_text("Waiting for connection...");
                        this->m_background = this->m_defbg;
                        this->m_connAnim->m_value = 0.f;
                        this->StartConnectionThread(5);
                    }
                }
            }
            this->m_connNext->draw({150, 150, 150, 255}, BLANK);
        }
        else
        {
            this->m_connNext->set_margin(10);
            this->m_connNext->draw(WHITE, BLANK);
        }
    }
}

void Game::StartConnectionThread(int delay)
{
    this->m_connChange = false;
    this->m_loadingPage = true;
    this->m_clientStatus = CNetStatus::CNET_NONE;

    this->m_connThread = std::thread([&, delay]()
                                     { 
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        this->m_clientStatus = this->m_client->start(); });
    this->m_connThread.detach();
}
