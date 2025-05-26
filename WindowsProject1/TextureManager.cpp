#include "pch.h"
#include "TextureManager.h"

void TextureManager::InsertTexture(const std::wstring& filePath, const std::wstring& key, int width, int height) {
    if (textureMap.find(key) != textureMap.end())
        return;

    HBITMAP hBitmap = (HBITMAP)LoadImageW(
        nullptr, filePath.c_str(), IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );

    if (!hBitmap) {
        OutputDebugStringW((L"텍스처 로드 실패: " + filePath + L"\n").c_str());
        return;
    }

    HDC hdc = CreateCompatibleDC(nullptr);
    HBITMAP hOld = (HBITMAP)SelectObject(hdc, hBitmap);

    TextureData data;
    data.hdc = hdc;
    data.hBitmap = hBitmap;
    data.hOldBitmap = hOld;
    data.size = { width, height, 0 };

    textureMap[key] = data;

#ifdef _DEBUG
    OutputDebugStringW((L"텍스처 로드 성공: " + key + L"\n").c_str());
#endif
}

HDC TextureManager::FindTexture(const std::wstring& key) const {
    auto it = textureMap.find(key);
    if (it != textureMap.end())
        return it->second.hdc;

    return nullptr;
}

D3DXVECTOR3 TextureManager::GetTextureSize(const std::wstring& key) const
{
    auto it = textureMap.find(key);
    if (it != textureMap.end())
        return it->second.size;

    return { 0, 0, 0 };
}

void TextureManager::Release() {
    for (auto& [key, data] : textureMap) {
        if (data.hdc && data.hOldBitmap)
            SelectObject(data.hdc, data.hOldBitmap);
        if (data.hBitmap)
            DeleteObject(data.hBitmap);
        if (data.hdc)
            DeleteDC(data.hdc);
    }
    textureMap.clear();
}

TextureManager::~TextureManager() {
    Release();
}