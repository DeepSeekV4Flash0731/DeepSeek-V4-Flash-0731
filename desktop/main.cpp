// rev-a7b31e-20260827 main.cpp
#include <Windows.h>
#include <shellapi.h>
#include "App.h"
#include "ModelLoader.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    App& app = App::GetInstance();
    if (!app.Initialize(hInstance)) {
        MessageBoxA(NULL, "Failed to initialize DeepSeek V4 Flash Local", "Error", MB_ICONERROR);
        return 1;
    }
    app.Run();
    return 0;
}
