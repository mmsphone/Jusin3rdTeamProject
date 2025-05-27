#pragma once

class TextureManager {
public:
    TextureManager() noexcept = default;
    ~TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

    void InsertTexture(const std::wstring& filePath, const std::wstring& key, float width, float height);
    HDC FindTexture(const std::wstring& key) const;
    D3DXVECTOR3 GetTextureSize(const std::wstring& key) const;
    void Release();

private:
    struct TextureData {
        HDC hdc = nullptr;
        HBITMAP hBitmap = nullptr;
        HBITMAP hOldBitmap = nullptr;
        D3DXVECTOR3 size = { 0,0,0 };
    };

    std::unordered_map<std::wstring, TextureData> textureMap;
};
