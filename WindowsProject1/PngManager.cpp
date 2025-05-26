#pragma once
#include "pch.h"
#include "PngManager.h"
#include <comdef.h>

PngManager::~PngManager() {
    Release();
}

void PngManager::InsertPng(const std::wstring& filePath, const std::wstring& key) {
    if (mapPng.contains(key)) {
        MessageBox(nullptr, L"키가 이미 존재합니다", L"Error", MB_OK);
        return;
    }

    PngData data;

    HBITMAP hBitmap = LoadPngWIC(filePath);
    if (!hBitmap) {
        MessageBox(nullptr, (L"PNG 로드 실패: " + filePath).c_str(), L"에러", MB_OK);
        return;
    }

    HDC screenDC = GetDC(nullptr);
    data.hdc = CreateCompatibleDC(screenDC);
    if (!data.hdc) {
        MessageBox(nullptr, L"CreateCompatibleDC 실패", L"에러", MB_OK);
        ReleaseDC(nullptr, screenDC);
        return;
    }

    data.hBitmap = hBitmap;
    data.oldBitmap = (HBITMAP)SelectObject(data.hdc, hBitmap);
    if (data.oldBitmap == nullptr) {
        MessageBox(nullptr, L"SelectObject 실패", L"에러", MB_OK);
        DeleteDC(data.hdc);
        ReleaseDC(nullptr, screenDC);
        return;
    }
    ReleaseDC(nullptr, screenDC);

    mapPng.insert({ key, data });
}

HDC PngManager::FindPng(const std::wstring& key) const {
    auto it = mapPng.find(key);
    if (it == mapPng.end())
        return nullptr;
    return it->second.hdc;
}

void PngManager::Release() {
    for (auto& [key, data] : mapPng) {
        SelectObject(data.hdc, data.oldBitmap);
        DeleteObject(data.hBitmap);
        DeleteDC(data.hdc);
    }
    mapPng.clear();
}

HBITMAP PngManager::LoadPngWIC(const std::wstring& filePath) {
    IWICImagingFactory* pFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    HBITMAP hBitmap = nullptr;

    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        MessageBox(nullptr, L"CoInitializeEx failed", L"Error", MB_OK);
        return nullptr;
    }

    do {
        if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory)))) {
            MessageBox(nullptr, L"CoCreateInstance failed", L"Error", MB_OK);
            break;
        }

        if (FAILED(pFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder))) {
            MessageBox(nullptr, L"CreateDecoderFromFilename failed", L"Error", MB_OK);
            break;
        }

        if (FAILED(pDecoder->GetFrame(0, &pFrame))) {
            MessageBox(nullptr, L"GetFrame failed", L"Error", MB_OK);
            break;
        }

        if (FAILED(pFactory->CreateFormatConverter(&pConverter))) {
            MessageBox(nullptr, L"CreateFormatConverter failed", L"Error", MB_OK);
            break;
        }

        if (FAILED(pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom))) {
            MessageBox(nullptr, L"Initialize failed", L"Error", MB_OK);
            break;
        }

        UINT width, height;
        pConverter->GetSize(&width, &height);

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -static_cast<LONG>(height); // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HDC screenDC = GetDC(nullptr);
        hBitmap = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
        ReleaseDC(nullptr, screenDC);

        if (!hBitmap) {
            MessageBox(nullptr, L"CreateDIBSection failed", L"Error", MB_OK);
            break;
        }

        if (FAILED(pConverter->CopyPixels(nullptr, width * 4, width * height * 4, (BYTE*)pBits))) {
            MessageBox(nullptr, L"CopyPixels failed", L"Error", MB_OK);
            DeleteObject(hBitmap);
            hBitmap = nullptr;
        }

    } while (false);

    if (pFactory) pFactory->Release();
    if (pDecoder) pDecoder->Release();
    if (pFrame) pFrame->Release();
    if (pConverter) pConverter->Release();

    CoUninitialize();

    if (hBitmap == nullptr) {
        MessageBox(nullptr, L"Failed to load PNG or create HBITMAP", L"Error", MB_OK);
    }

    return hBitmap;
}