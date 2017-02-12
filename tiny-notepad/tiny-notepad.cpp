// tiny-notepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "tiny-notepad.h"
#include <commdlg.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TINYNOTEPAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TINYNOTEPAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TINYNOTEPAD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL initEditInstance(HWND hWnd)
{
    HFONT hfDefault;
    HWND hEdit;

    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        0, 0, 100, 100, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
    if (hEdit == NULL)
    {
        MessageBox(hWnd, _T("Could not create main edit."), _T("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }
    hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
    return TRUE;
}

void initToolBar(HWND hWnd)
{
    HWND hTool;
    TBBUTTON tbb[3];
    TBADDBITMAP tbab;

    // Create Toolbar
    hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
        hWnd, (HMENU)IDC_MAIN_TOOL, GetModuleHandle(NULL), NULL);
    if (hTool == NULL)
        MessageBox(hWnd, _T("Could not create tool bar."), _T("Error"), MB_OK | MB_ICONERROR);

    // Send the TB_BUTTONSTRUCTSIZE message, which is required for
    // backward compatibility.
    SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    tbab.hInst = HINST_COMMCTRL;
    tbab.nID = IDB_STD_SMALL_COLOR;
    SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tbab);

    ZeroMemory(tbb, sizeof(tbb));
    tbb[0].iBitmap = STD_FILENEW;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON;
    tbb[0].idCommand = ID_FILE_NEW;

    tbb[1].iBitmap = STD_FILEOPEN;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[1].idCommand = ID_FILE_OPEN;

    tbb[2].iBitmap = STD_FILESAVE;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = TBSTYLE_BUTTON;
    tbb[2].idCommand = ID_FILE_SAVE;

    SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);
}

void initStatusBar(HWND hWnd)
{
    HWND hStatus;
    int statwidths[] = { 100, -1 };

    // Create Status bar
    hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
        hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

    SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
    SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)_T("Hi there :)"));
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            initEditInstance(hWnd);
            initStatusBar(hWnd);
            initToolBar(hWnd);
        }
        break;
    case WM_SIZE:
        {
            HWND hTool;
            RECT rcTool;
            int iToolHeight;

            HWND hStatus;
            RECT rcStatus;
            int iStatusHeight;

            HWND hEdit;
            int iEditHeight;
            RECT rcClient;

            // Size toolbar and get height

            hTool = GetDlgItem(hWnd, IDC_MAIN_TOOL);
            SendMessage(hTool, TB_AUTOSIZE, 0, 0);

            GetWindowRect(hTool, &rcTool);
            iToolHeight = rcTool.bottom - rcTool.top;

            // Size status bar and get height

            hStatus = GetDlgItem(hWnd, IDC_MAIN_STATUS);
            SendMessage(hStatus, WM_SIZE, 0, 0);

            GetWindowRect(hStatus, &rcStatus);
            iStatusHeight = rcStatus.bottom - rcStatus.top;

            // Calculate remaining height and size edit

            GetClientRect(hWnd, &rcClient);

            iEditHeight = rcClient.bottom - iToolHeight - iStatusHeight;

            hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
            SetWindowPos(hEdit, NULL, 0, iToolHeight, rcClient.right, iEditHeight, SWP_NOZORDER);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_FILE_NEW:
                DoFileNew(hWnd);
                break;
            case ID_FILE_OPEN:
                DoFileOpen(hWnd);
                break;
            case ID_FILE_SAVE:
                DoFileSave(hWnd);
                break;
            case ID_EDIT_CUT:
                SendDlgItemMessage(hWnd, IDC_MAIN_EDIT, WM_CUT, 0, 0);
                break;
            case ID_EDIT_COPY:
                SendDlgItemMessage(hWnd, IDC_MAIN_EDIT, WM_COPY, 0, 0);
                break;
            case ID_EDIT_PASTE:
                SendDlgItemMessage(hWnd, IDC_MAIN_EDIT, WM_PASTE, 0, 0);
                break;
            case ID_FILE_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DoFileNew(HWND hWnd)
{
        HWND hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
        if (SetWindowTextA(hEdit, (LPCSTR)""))
        {
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 0, (LPARAM)_T("Opened..."));
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 1, (LPARAM)_T(""));
        }
}

void DoFileOpen(HWND hWnd)
{
    OPENFILENAME ofn;
    TCHAR szFileName[MAX_PATH] = _T("");

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = _T("txt");

    if (GetOpenFileName(&ofn))
    {
        HWND hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
        if (LoadTextFileToEdit(hEdit, szFileName))
        {
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 0, (LPARAM)_T("Opened..."));
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 1, (LPARAM)szFileName);
        }
    }
}

void DoFileSave(HWND hWnd)
{
    OPENFILENAME ofn;
    TCHAR szFileName[MAX_PATH] = _T("");

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = _T("txt");
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn))
    {
        HWND hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
        if (SaveTextFileFromEdit(hEdit, szFileName))
        {
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 0, (LPARAM)_T("Saved..."));
            SendDlgItemMessage(hWnd, IDC_MAIN_STATUS, SB_SETTEXT, 1, (LPARAM)szFileName);
        }
    }
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize;

        dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize != INVALID_FILE_SIZE)
        {
            LPVOID pszFileText;

            HANDLE heapHandle = GetProcessHeap();
            pszFileText = HeapAlloc(heapHandle, HEAP_GENERATE_EXCEPTIONS, dwFileSize + 1);
            if (pszFileText != NULL)
            {
                DWORD dwRead;

                if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
                {
                    ((char*)pszFileText)[dwFileSize] = 0;// Add null terminator
                    if (SetWindowTextA(hEdit, (LPCSTR)pszFileText))
                        bSuccess = TRUE;
                }
                HeapFree(heapHandle, 0, pszFileText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwTextLength;

        dwTextLength = GetWindowTextLength(hEdit);
        if (dwTextLength > 0)
        {
            LPVOID pszText;
            DWORD dwBufferSize = dwTextLength + 1;

            HANDLE heapHandle = GetProcessHeap();
            pszText = HeapAlloc(heapHandle, HEAP_GENERATE_EXCEPTIONS, dwBufferSize);
            if (pszText != NULL)
            {
                if (GetWindowTextA(hEdit, (LPSTR)pszText, dwBufferSize))
                {
                    DWORD dwWritten;

                    if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
                        bSuccess = TRUE;
                }
                HeapFree(heapHandle, 0, pszText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}
