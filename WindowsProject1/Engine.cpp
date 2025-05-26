#include "pch.h"
#include "Engine.h"
#include "SceneManager.h"
#include "InputSystem.h"
#include "SoundManager.h"
Engine& Engine::GetInstance()
{
	static Engine instance;
	return instance;
}

bool Engine::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    this->hInstance = hInstance;
    msg.message = WM_NULL;

    if (!InitWindow(hInstance, nCmdShow))
        return false;

    sceneManager = std::make_unique<SceneManager>();
    sceneManager->Initialize();
    inputSystem = std::make_unique<InputSystem>();

    soundManager = std::make_unique<SoundManager>();
    soundManager->Initialize();

    return true;
}

void Engine::Run() {
    while (isRunning) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return;

            if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        auto now = std::chrono::system_clock::now();
        double dt = std::chrono::duration<double>(now - lastTick).count();
        if (dt > FramePurpose * 2.0) dt = FramePurpose;

        if (dt >= FramePurpose) {
            lastTick = now;
            inputSystem->Tick();
            sceneManager->Update(dt);
            Render();
        }
    }
}

void Engine::Shutdown()
{
    DestroyWindow(hWnd);
}

InputSystem* Engine::GetInputSystem()
{
    return inputSystem.get();
}

SoundManager* Engine::GetSoundManager()
{
    return soundManager.get();
}

HWND Engine::GethWnd()
{
    return hWnd;
}

RECT Engine::GetWindowSize()
{
    return windowSize;
}

bool Engine::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wcex;
    const wchar_t CLASS_NAME[] = L"WindowClass";
    
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) {
        DWORD err = GetLastError();
        wchar_t buffer[256];
        wsprintf(buffer, L"RegisterClassEx failed! Error code: %lu\n", err);
        OutputDebugStringW(buffer);
        return false;
    }

    windowSize = { 0, 0, 800, 600 };
    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowW(CLASS_NAME, L"My Game", WS_OVERLAPPEDWINDOW,
        100, 100, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, nullptr, nullptr, hInstance, nullptr);
    
    if (!hWnd) {
        DWORD errorCode = GetLastError();
        wchar_t buffer[256];
        wsprintf(buffer, L"CreateWindow failed! Error code: %lu\n", errorCode);
        OutputDebugStringW(buffer);
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return true;
}

LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto input = Engine::GetInstance().GetInputSystem();
    switch (message)
    {
    case WM_KEYDOWN:
        input->OnKeyDown(wParam);
        break;

    case WM_KEYUP:
        input->OnKeyUp(wParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
        input->OnMouseDown();
        break;

    case WM_LBUTTONUP:
        input->OnMouseUp();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Engine::Render()
{
    HDC hdc = GetDC(hWnd);

    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right;
    int height = rect.bottom;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

    sceneManager->Render(memDC);

    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    ReleaseDC(hWnd, hdc);
    DeleteDC(memDC);
}
