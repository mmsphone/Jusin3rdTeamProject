#pragma once
class InputSystem {
public:
    InputSystem();
    void OnMouseDown();
    void OnMouseUp();
    void Tick();

    bool IsKeyDown(WPARAM key) const;
    bool IsKeyPressed(WPARAM key) const;
    bool IsKeyReleased(WPARAM key) const;

    bool IsMouseDown() const;
    bool IsMousePressed() const;
    bool IsMouseReleased() const;
    D3DXVECTOR3 GetMousePos();

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

    D3DXVECTOR3 mousePos;
};