#include "pch.h"
#include <windows.h>
#include <string>

double text = 0.0;
HWND hEdit;

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
    HWND hwnd = CreateWindowExA(
        0,                              // Дополнительные стили окна
        CLASS_NAME,                     // Имя класса окна
        "Input Dialog",                 // Заголовок окна
        WS_OVERLAPPEDWINDOW,            // Стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 150, // Положение и размер окна
        NULL,                           // Родительское окно
        NULL,                           // Меню
        GetModuleHandle(NULL),          // Экземпляр приложения
        NULL                            // Дополнительные параметры создания
    );

    if (hwnd == NULL)
    {
        return;
    }

    // Создаем поле ввода
    hEdit = CreateWindowExA(
        0, "EDIT",                      // Имя класса поля ввода
        NULL,                           // Текст по умолчанию
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, // Стиль поля ввода
        50, 30, 200, 25,                // Положение и размер
        hwnd,                           // Родительское окно
        NULL,                           // Идентификатор поля ввода
        GetModuleHandle(NULL),          // Экземпляр приложения
        NULL                            // Дополнительные параметры
    );

    // Создаем кнопку "OK"
    CreateWindowExA(
        0, "BUTTON",                    // Имя класса кнопки
        "OK",                           // Текст на кнопке
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, // Стиль кнопки
        110, 70, 75, 25,                // Положение и размер
        hwnd,                           // Родительское окно
        (HMENU)1,                       // Идентификатор кнопки
        GetModuleHandle(NULL),          // Экземпляр приложения
        NULL                            // Дополнительные параметры
    );

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // Запускаем цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
