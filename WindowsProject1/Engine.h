#pragma once
class SceneManager;
class InputSystem;
class Engine
{
public:
	static Engine& GetInstance();

	bool Initialize(HINSTANCE hInstance, int nCmdShow);
	void Run();
	void Shutdown();
	InputSystem* GetInputSystem();
	HWND GethWnd();

	RECT GetWindowSize();
private:
	Engine() = default;
	bool InitWindow(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Render();

private:
	HINSTANCE hInstance;
	HWND hWnd;
	HACCEL hAccel;
	MSG msg;
	RECT windowSize;
private:
	bool isRunning = true;

private:
	std::chrono::system_clock::time_point lastTick;
	static constexpr double FramePurpose = 1. / 144.;

public:
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<InputSystem> inputSystem;
};

