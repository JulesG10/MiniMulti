#include "src/game.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ char* pCmdLine, _In_ int nCmdShow)
{
    std::unique_ptr<Game> game = std::make_unique<Game>(__argc, __argv);
    return game->Start();
}