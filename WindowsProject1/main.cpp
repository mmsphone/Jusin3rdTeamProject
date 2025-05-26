#include "pch.h"
#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Engine& Engine = Engine::GetInstance();
    if (!Engine.Initialize(hInstance, nCmdShow)) return -1;
    Engine.Run();
    return 0;
}