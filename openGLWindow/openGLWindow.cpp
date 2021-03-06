// openGLWindow.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <thread>
#include "openGLWindow.h"
#include "GLContext.h"
#include<windows.h>
#include<atlstr.h>
#include<GL/freeglut.h>

//打开保存文件对话框
#include<Commdlg.h>

//选择文件夹对话框
#include<Shlobj.h>
#pragma comment(lib,"Shell32.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前置声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    OpenFile(HWND, UINT, WPARAM, LPARAM);  //fang
void                SelectFile(); //fang
CString             SetTargetFileName();

void                 runThread();
void                 InitGL();
void                 DrawGLScene1(HDC hdc);
void                 DrawGLScene2(HDC hdc);
bool                 DRAWFLAG = 0;
std::thread          glThread;

GLContext			glContext;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。


	// 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENGLWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLWINDOW)); //快捷键

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg); //转换按键信息
            DispatchMessage(&msg);  //将消息传递给WinProc
        }
    }
    return (int) msg.wParam;
}

/////////////////////////////////////////
// openGL 代码
////////////////////////////////////////

void InitGL()                              // 此处开始对OpenGL进行所有设置
{
	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
}

void DrawGLScene1(HDC hdc)                             // 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (DRAWFLAG) {
		glRectf(-0.1f, -0.1f, 0.1f, 0.1f);

	}
	else glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

	SwapBuffers(hdc);
}

void DrawGLScene2(HDC hdc)
{

}

void runThread() {

	HWND hwnd = FindWindow(L"OpenGLWindow", NULL);
	HDC  hdc = GetDC(hwnd);
	std::thread glThread(&DrawGLScene1, hdc);

	//InvalidateRect(hwnd, NULL, true);  //使用InvalidateRect函数触发WM_PAINT消息

}
//void PaintWindow(HDC hdc)
//{
//	if (DRAWFLAG) {
//
//		DrawGLScene1;
//	}
//	else DrawGLScene2();
//
//	SwapBuffers(hdc);
//}


//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGLWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENGLWINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(
		szWindowClass,           //注册的窗口类的名称
		szTitle,                 //窗口的标题文本
		WS_OVERLAPPEDWINDOW,     //窗口的外观样式：重叠
		CW_USEDEFAULT,           //窗口相对于父窗口的X，Y坐标
		0, 
		CW_USEDEFAULT,           //窗口的 宽，高
		0, 
		nullptr,                 //有无父窗口
		nullptr,                 //有没有菜单
		hInstance,               //当前应用程序的实例句柄
		nullptr);                //有无附加数据

	if (!hWnd)      //检查窗口是否创建成功
	{
		return FALSE;
	}
	
	ShowWindow(hWnd, nCmdShow);    //显示窗口

	glContext.init(hWnd);      //初始化openGL
	InitGL();

	UpdateWindow(hWnd);     //更新窗口

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//6+93+-
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndButton;  //fang
	HDC hdc;


	switch (message)   //消息处理
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:

			glThread.join();
			//ReleaseDC(hWnd, hdc);

			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:   //fang
			//MessageBox("123");
			//SetTargetFileName();
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FILE), hWnd, OpenFile);
			break;
		case IDM_REDRAW:  //重绘选项
			HINSTANCE hInst;

			DRAWFLAG = 1;
			InvalidateRect(hWnd, NULL, true);  //使用InvalidateRect函数触发WM_PAINT消息

			MessageBox(NULL, TEXT("DRAW"), TEXT("messagebox"), 0);

			break;
			//return (0);

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_CREATE:    //FANG
	{
		hwndButton = CreateWindow(TEXT("button"),//必须为：button  
			TEXT("anniu"),//按钮上显示的字符  
			WS_CHILD | WS_VISIBLE,
			100, 100, 80, 40,
			hWnd, (HMENU)0,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);

	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);  // 利用BeginPaint获取绘图设备的句柄---一个HDC对象，然后在改绘图设备上进行绘制
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		DrawGLScene1(hdc);

		//if (DRAWFLAG) {
		//	//DrawGLScene1(hdc);
		//}
		//else //DrawGLScene2(hdc);


		EndPaint(hWnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:  //鼠标左键点击消息
	{
		DRAWFLAG = 1;
		InvalidateRect(hWnd, NULL, true);  //使用InvalidateRect函数触发WM_PAINT消息

	}
	break;

	case WM_RBUTTONDOWN:  //鼠标右键点击消息
	{
		DRAWFLAG = 0;
		InvalidateRect(hWnd, NULL, true);  //使用InvalidateRect函数触发WM_PAINT消息

	}
	break;


	case WM_DESTROY:
		PostQuitMessage(0); //退出消息循环
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);  //将消息控制权交回给系统
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	}

    return (INT_PTR)FALSE;
}


// “打开文件”框的消息处理程序。
INT_PTR CALLBACK OpenFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_OPENFILE)
		{
			SelectFile();
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

void SelectFile() {

	//HWND hDlg;
	//hDlg = GetDlgItem(IDC_EDITFILENAME)->GetSafeHwnd();


	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	ofn.lpstrFilter = TEXT("所有文件\0*.*\0.txt/.dat Flie\0*.txt;*.dat\0\0");//设置过滤
	ofn.nFilterIndex = 1;//过滤器索引
	ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择数据文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
	if (GetOpenFileName(&ofn))
	{
		//SetWindowText(hWnd, strFilename);
		MessageBox(NULL, strFilename, TEXT("选择的文件"), 0);
	}

	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//目录必须存在，覆盖文件前发出警告
	//ofn.lpstrTitle = TEXT("保存到");//使用系统默认标题留空即可
	//ofn.lpstrDefExt = TEXT("cpp");//默认追加的扩展名
	//if (GetSaveFileName(&ofn))
	//{
	//	MessageBox(NULL, strFilename, TEXT("保存到"), 0);
	//}
	//else {
	//	MessageBox(NULL, TEXT("请输入一个文件名"), NULL, MB_ICONERROR);
	//}

	//TCHAR szBuffer[MAX_PATH] = { 0 };
	//BROWSEINFO bi = { 0 };
	//bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	//bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区
	//bi.lpszTitle = TEXT("选择一个文件夹");//标题
	//bi.ulFlags = BIF_NEWDIALOGSTYLE;
	//LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	//if (SHGetPathFromIDList(idl, szBuffer)) {
	//	MessageBox(NULL, szBuffer, TEXT("你选择的文件夹"), 0);
	//}
	//else {
	//	MessageBox(NULL, TEXT("请选择一个文件夹"), NULL, MB_ICONERROR);
	//}


}

//使用BROWSEINFO选择文件夹
CString SetTargetFileName()
{
	CString strPath(_T(""));
	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;//显示位于对话框左上部的提示信息
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//有新建文件夹按钮
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//调用选择对话框
	if (pidl == NULL)
	{
		return strPath;
	}
	TCHAR strFolder[1024];
	SHGetPathFromIDList(pidl, strFolder);
	strPath = strFolder;
	MessageBox(NULL, strPath, TEXT("选择的文件夹"), 0);

	return strPath;
}

