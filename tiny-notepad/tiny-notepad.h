#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100
#define IDC_MAIN_EDIT 200
#define IDC_MAIN_TOOL 201
#define IDC_MAIN_STATUS 202

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

                                                // Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                DoFileOpen(HWND hWnd);
void                DoFileSave(HWND hWnd);
void                DoFileNew(HWND hWnd);
BOOL                LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);
BOOL                SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);
