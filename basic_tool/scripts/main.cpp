#include <Windows.h>
#include <memory>
#include "application.h"

// エントリポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    std::unique_ptr<Application> pApplication = std::make_unique<Application>(hInstance);

    pApplication->Initialize();
    pApplication->Loop();
    pApplication->Finalize();

    return 0;
}