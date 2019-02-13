#include "stdafx.h"
#include "dlg.h"

#include <string>

#define MAX_LOADSTRING 100

HWND hWndDlg;
HINSTANCE hInstance;

typedef struct
{
    std::wstring Name;
    std::wstring Url;
    std::wstring License;
} LicenseInfo;

LicenseInfo Licenses[] =
{
    {L"ImGui", L"https://github.com/ocornut/imgui", 
LR"(The MIT License (MIT)

Copyright(c) 2014 - 2019 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense,and /or sell
copies of the Software,and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)" }
};


INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
        {
            // Set version text
            WCHAR szVersion[MAX_LOADSTRING];
            LoadString(hInstance, IDS_VERSION, szVersion, MAX_LOADSTRING);

            std::wstring version = L"Cube version: " + std::wstring(szVersion);
            SetDlgItemText(hDlg, IDC_TEXT_VERSION, version.c_str());

            // Add items to license listbox
            HWND hwndList = GetDlgItem(hDlg, IDC_LISTBOX);

            for (int i = 0; i < ARRAYSIZE(Licenses); i++)
            {
                int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)Licenses[i].Name.c_str());
                SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
            }

            return TRUE;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:
                {
                    EndDialog(hDlg, LOWORD(wParam));
                    return TRUE;
                }
                case IDC_LISTBOX:
                {
                    switch (HIWORD(wParam))
                    {
                        case LBN_SELCHANGE:
                        {
                            HWND hwndList = GetDlgItem(hDlg, IDC_LISTBOX);
                            int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                            int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

                            std::wstring text = Licenses[i].Name + L"\n" + Licenses[i].Url + L"\n" + Licenses[i].License;
                            SetDlgItemText(hDlg, IDC_TEXT_LICENSE, text.c_str());

                            return TRUE;
                        }
                    }
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDM_ABOUT:
                {
                    ShowWindow(hWndDlg, SW_SHOW);
                    break;
                }
                case IDM_EXIT:
                {
                    DestroyWindow(hWnd);
                    break;
                }
                default:
                {
                    return DefWindowProc(hWnd, msg, wParam, lParam);
                }
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }

    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    hInstance = hInst;

    // Load resource strings
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_DLG, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLG));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DLG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Register window class
    RegisterClassExW(&wcex);

    // Create window
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    // Create dialog
    hWndDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);

    // Show main window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Load accelerators
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DLG));

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
