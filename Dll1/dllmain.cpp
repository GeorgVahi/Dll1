#include "pch.h"
#include <windows.h>
#include <string>

double text = 0.0;
extern HWND hwnd = NULL;
HWND hEdit = NULL;

// Функция обратного вызова для обработки сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) // ID кнопки "OK"
        {
            char buffer[256];
            GetWindowTextA(hEdit, buffer, 256);
            text = atof(buffer); // Преобразуем строку в число с плавающей запятой
            DestroyWindow(hwnd); // Закрываем окно после нажатия "OK"
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

// Функция для активации и фокусировки окна
void ActivateWindow(HWND hwnd)
{
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

// Экспортируемая функция для отображения окна ввода
extern "C" __declspec(dllexport) void __stdcall ShowInputDialog()
{
    const char CLASS_NAME[] = "InputDialogClass";

    // Регистрируем класс окна
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    // Создаем окно
    hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Input Dialog",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

  

    if (hwnd == NULL)
    {
        return;
    }

    // Создаем поле ввода
    hEdit = CreateWindowExA(
        0, "EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        50, 30, 200, 25,
        hwnd,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    // Создаем кнопку "OK"
    CreateWindowExA(
        0, "BUTTON",
        "OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        110, 70, 75, 25,
        hwnd,
        (HMENU)1,
        GetModuleHandle(NULL),
        NULL
    );

    // Показать окно
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    ActivateWindow(hwnd);

    // Запускаем цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Экспортируемая функция для закрытия окна ввода
extern "C" __declspec(dllexport) void __stdcall CloseInputDialog()
{
    if (hwnd != NULL)
    {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    else {
        MessageBox(NULL, L"Window is already closed or not initialized.", L"Information", MB_OK | MB_ICONINFORMATION);
    }
}

// Экспортируемая функция для получения значения
extern "C" __declspec(dllexport) double __stdcall GetText()
{
    return text;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}