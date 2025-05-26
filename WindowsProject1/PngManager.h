#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dxgiformat.h>
#include <dxgi.h>
#include <wincodec.h>

class PngManager {
public:
    static PngManager& GetInstance() {
        static PngManager instance;
        return instance;
    }

    PngManager(const PngManager&) = delete;
    PngManager& operator=(const PngManager&) = delete;
    PngManager(PngManager&&) = delete;
    PngManager& operator=(PngManager&&) = delete;

    void InsertPng(const std::wstring& filePath, const std::wstring& key);
    HDC FindPng(const std::wstring& key) const;
    void Release();

private:
    PngManager() noexcept = default;
    ~PngManager();

    struct PngData {
        HDC hdc = nullptr;
        HBITMAP hBitmap = nullptr;
        HBITMAP oldBitmap = nullptr;
    };

    std::unordered_map<std::wstring, PngData> mapPng;

    HBITMAP LoadPngWIC(const std::wstring& filePath);
};
