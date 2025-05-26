#pragma once
class InputSystem {
public:
    InputSystem();
    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);
    void OnMouseMove(int x, int y);
    void OnMouseDown();
    void OnMouseUp();
    void Tick();

    bool IsKeyDown(WPARAM key) const;
    bool IsKeyPressed(WPARAM key) const;
    bool IsKeyReleased(WPARAM key) const;

    bool IsMouseDown() const;
    bool IsMousePressed() const;
    bool IsMouseReleased() const;
    math::vec2 GetMousePos();

    void SetKeyWork(bool keyWork);
    void SetMouseWork(bool mouseWork);

private:
    static constexpr int KEY_COUNT = 256;
    std::vector<bool> keyDown;
    std::vector<bool> wasKeyDown;
    bool mouseDown;
    bool wasMouseDown;
    bool isKeyWork = true;
    bool isMouseWork = true;

    math::vec2 mousePos;
};