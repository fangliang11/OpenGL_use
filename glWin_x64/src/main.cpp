///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// main driver
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-06-28
// UPDATED: 2017-11-30
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN             // exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <commctrl.h>                   // common controls
#include "Window.h"
#include "DialogWindow.h"
#include "ControllerMain.h"
#include "ControllerGL.h"
#include "ControllerFormGL.h"
#include "ModelGL.h"
#include "ViewGL.h"
#include "ViewFormGL.h"
#include "resource.h"
#include "Log.h"


// function declarations
int mainMessageLoop(HACCEL hAccelTable=0);




///////////////////////////////////////////////////////////////////////////////
// main function of a windows application
///////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdArgs, int cmdShow)
{
    // set log mode: FILE, DIALOG or BOTH
    //Win::logMode(Win::LOG_MODE_BOTH);
    Win::logMode(Win::LOG_MODE_FILE); // LOG_MODE_FILE is default

    // register slider(trackbar) from comctl32.dll brfore creating windows
    INITCOMMONCONTROLSEX commonCtrls;
    commonCtrls.dwSize = sizeof(commonCtrls);
    commonCtrls.dwICC = ICC_BAR_CLASSES;        // trackbar is in this class
    ::InitCommonControlsEx(&commonCtrls);

    // get app name from resource file
    wchar_t name[256];
    ::LoadString(hInst, IDS_APP_NAME, name, 256);

    Win::ControllerMain mainCtrl;
    Win::Window mainWin(hInst, name, 0, &mainCtrl);

    // add menu to window class
    mainWin.setMenuName(MAKEINTRESOURCE(IDR_MAIN_MENU));
    mainWin.setWidth(300);
    mainWin.setHeight(460);
    mainWin.setWindowStyleEx(WS_EX_WINDOWEDGE);

    // create a window and show
    if(mainWin.create())
        Win::log("Main window is created.");
    else
        Win::log("[ERROR] Failed to create main window.");

    // create model and view components for controller
    ModelGL modelGL;
    Win::ViewGL viewGL;
    Win::ViewFormGL viewFormGL;

    // create OpenGL rendering window as a child
    Win::ControllerGL glCtrl(&modelGL, &viewGL);
    Win::Window glWin(hInst, L"WindowGL", mainWin.getHandle(), &glCtrl);
    glWin.setClassStyle(CS_OWNDC);
    glWin.setWindowStyle(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    glWin.setWindowStyleEx(!WS_EX_CLIENTEDGE);   // remove edge
    glWin.setWidth(300);
    glWin.setHeight(300);
    if(glWin.create())
        Win::log("OpenGL window is created.");
    else
        Win::log("[ERROR] Failed to create OpenGL window.");

    // create a child dialog box contains controls
    Win::ControllerFormGL formCtrl(&modelGL, &viewFormGL);
    Win::DialogWindow glDialog(hInst, IDD_CONTROLS, mainWin.getHandle(), &formCtrl);
    glDialog.setWidth(300);
    glDialog.setHeight(160);
    if(glDialog.create())
        Win::log("OpenGL form dialog is created.");
    else
        Win::log("[ERROR] Failed to create OpenGL form dialog.");

    // create status bar window using CreateWindowEx()
    // mainWin must pass WM_SIZE message to the status bar
    // So, mainWin accesses the status bar with GetDlgItem(handle, IDC_STATUSBAR).
    HWND statusHandle = ::CreateWindowEx(0, STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                                         0, 0, 0, 0, mainWin.getHandle(), (HMENU)IDC_STATUSBAR, ::GetModuleHandle(0), 0);
    if(statusHandle)
        Win::log("Status bar window is created.");
    else
        Win::log("[ERROR] Failed to create status bar window.");
    ::SendMessage(statusHandle, SB_SETTEXT, 0, (LPARAM)L"Ready");

    // send window handles to mainCtrl, they are used for resizing window
    mainCtrl.setGLHandle(glWin.getHandle());
    mainCtrl.setFormHandle(glDialog.getHandle());

    // re-compute dim of dialog window (for higher DPI display) ==========
    RECT rect;
    ::GetWindowRect(glDialog.getHandle(), &rect);   // get size of glDialog
    int dialogWidth = rect.right - rect.left;
    int dialogHeight = rect.bottom - rect.top;
    //Win::log("Dialog Dimension: %dx%d", dialogWidth, dialogHeight);

    // get height of statusbar
    ::GetWindowRect(statusHandle, &rect);
    int statusHeight = rect.bottom - rect.top;

    // reset dim of opengl window (square)
    int glWidth = dialogWidth;
    int glHeight = glWidth;
    ::SetWindowPos(glWin.getHandle(), 0, 0, 0, glWidth, glHeight, SWP_NOZORDER);

    // place the opengl form dialog at the bottom of the opengl rendering window
    ::SetWindowPos(glDialog.getHandle(), 0, 0, glHeight, dialogWidth, dialogHeight, SWP_NOZORDER);

    // reset dim of main window including menu
    rect.left = 0;
    rect.right = dialogWidth;
    rect.top = 0;
    rect.bottom = dialogHeight + glHeight + statusHeight;
    DWORD style = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_STYLE);
    DWORD styleEx = (DWORD)::GetWindowLongPtr(mainWin.getHandle(), GWL_EXSTYLE);
    ::AdjustWindowRectEx(&rect, style, TRUE, styleEx);
    ::SetWindowPos(mainWin.getHandle(), 0, 0, 0, (rect.right-rect.left), (rect.bottom-rect.top), SWP_NOZORDER);

    // show all windows
    glWin.show();
    glDialog.show();
    mainWin.show();
    //::SendMessage(mainWin.getHandle(), WM_NCPAINT, 1, 0);   // repaint window frame

    // main message loop //////////////////////////////////////////////////////
    int exitCode;
    HACCEL hAccelTable = 0;
    //hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACCEL));
    exitCode = mainMessageLoop(hAccelTable);

    Win::log("Application terminated.");
    return exitCode;
}



///////////////////////////////////////////////////////////////////////////////
// main message loop
///////////////////////////////////////////////////////////////////////////////
int mainMessageLoop(HACCEL hAccelTable)
{
    HWND activeHandle;
    MSG msg;

    while(::GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
    {
        // determine the activated window is dialog box
        // skip if messages are for the dialog windows
        activeHandle = GetActiveWindow();
        if(::GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
        {
            if(::IsDialogMessage(activeHandle, &msg))
                continue;   // message handled, back to while-loop
        }

        // now, handle window messages
        if(!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}
