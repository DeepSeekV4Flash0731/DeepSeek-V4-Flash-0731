// rev-a7b31e-20260827 App.h
#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <utility>

class App {
private:
    HINSTANCE m_hInstance = nullptr;
    HWND m_hwnd = nullptr;
    HWND m_chatDisplay = nullptr;
    HWND m_inputBox = nullptr;
    HWND m_sendButton = nullptr;
    HWND m_loadButton = nullptr;
    HWND m_statusBar = nullptr;
    bool m_isModelLoaded = false;
    std::vector<std::pair<std::string, std::string>> m_chatHistory;
    App() = default;
    bool RegisterWindowClass();
    bool CreateMainWindow();
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void SendMessage();
    void LoadModel();
    void AppendToChat(const std::string& text);
public:
    static App& GetInstance();
    bool Initialize(HINSTANCE hInstance);
    void Run();
};
