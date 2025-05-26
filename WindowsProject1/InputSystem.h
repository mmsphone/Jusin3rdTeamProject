#pragma once
class InputSystem {
public:
    InputSystem();
    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);
    void Tick();

    bool IsKeyDown(WPARAM key) const;
    bool IsKeyPressed(WPARAM key) const;
    bool IsKeyReleased(WPARAM key) const;

private:
    static constexpr int KEY_COUNT = 256;
    std::vector<bool> keyDown;
    std::vector<bool> wasKeyDown;
    bool isKeyWork = true;
};