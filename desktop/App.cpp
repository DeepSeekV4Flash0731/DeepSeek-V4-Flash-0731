// rev-a7b31e-20260827 App.cpp
#include "App.h"
#include "ModelLoader.h"
#include "ChatEngine.h"
#include "Config.h"
#include <fstream>

App& App::GetInstance() { static App i; return i; }

bool App::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;
    Config::GetInstance().Load("config/settings.json");

    if (!RegisterWindowClass()) return false;
    if (!CreateMainWindow()) return false;

    m_chatHistory.clear();
    m_isModelLoaded = false;
    return true;
}

bool App::RegisterWindowClass() {
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = App::WndProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "DeepSeekV4Flash";
    wc.lpszMenuName = NULL;
    return RegisterClassExA(&wc) != 0;
}

bool App::CreateMainWindow() {
    m_hwnd = CreateWindowExA(
        0, "DeepSeekV4Flash", "DeepSeek V4 Flash Local 0731",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        NULL, NULL, m_hInstance, NULL
    );
    if (!m_hwnd) return false;

    // Create chat display
    m_chatDisplay = CreateWindowExA(
        WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        10, 10, 960, 500,
        m_hwnd, (HMENU)1, m_hInstance, NULL
    );

    // Create input box
    m_inputBox = CreateWindowExA(
        WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
        10, 520, 960, 80,
        m_hwnd, (HMENU)2, m_hInstance, NULL
    );

    // Create send button
    m_sendButton = CreateWindowExA(
        0, "BUTTON", "Send",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        850, 610, 120, 30,
        m_hwnd, (HMENU)3, m_hInstance, NULL
    );

    // Create load model button
    m_loadButton = CreateWindowExA(
        0, "BUTTON", "Load Model",
        WS_CHILD | WS_VISIBLE,
        10, 610, 120, 30,
        m_hwnd, (HMENU)4, m_hInstance, NULL
    );

    // Create status bar
    m_statusBar = CreateWindowExA(
        0, "STATIC", "Ready. Load a GGUF model to start.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        140, 615, 700, 20,
        m_hwnd, (HMENU)5, m_hInstance, NULL
    );

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    return true;
}

void App::Run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 3) App::GetInstance().SendMessage();
            if (LOWORD(wParam) == 4) App::GetInstance().LoadModel();
            return 0;
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void App::SendMessage() {
    if (!m_isModelLoaded) {
        SetWindowTextA(m_statusBar, "Error: No model loaded. Click 'Load Model' first.");
        return;
    }

    char text[8192];
    GetWindowTextA(m_inputBox, text, sizeof(text));
    if (strlen(text) == 0) return;

    AppendToChat(std::string("You: ") + text + "\r\n");
    SetWindowTextA(m_inputBox, "");

    std::string response = ChatEngine::GetInstance().Generate(text);
    AppendToChat(std::string("DeepSeek: ") + response + "\r\n");
}

void App::LoadModel() {
    OPENFILENAMEA ofn{};
    char filePath[MAX_PATH] = "";
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hwnd;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "GGUF Models\0*.gguf\0All Files\0*.*\0";
    ofn.lpstrTitle = "Select DeepSeek V4 Flash Local Model";

    if (GetOpenFileNameA(&ofn)) {
        SetWindowTextA(m_statusBar, "Loading model...");
        if (ModelLoader::GetInstance().LoadModel(filePath)) {
            m_isModelLoaded = true;
            std::string msg = "Model loaded: " + std::string(filePath);
            SetWindowTextA(m_statusBar, msg.c_str());
            AppendToChat("System: Model loaded successfully. You can start chatting.\r\n");
        } else {
            SetWindowTextA(m_statusBar, "Error: Failed to load model.");
        }
    }
}

void App::AppendToChat(const std::string& text) {
    int len = GetWindowTextLengthA(m_chatDisplay);
    SendMessageA(m_chatDisplay, EM_SETSEL, len, len);
    SendMessageA(m_chatDisplay, EM_REPLACESEL, FALSE, (LPARAM)text.c_str());
    SendMessageA(m_chatDisplay, EM_SCROLLCARET, 0, 0);
}
