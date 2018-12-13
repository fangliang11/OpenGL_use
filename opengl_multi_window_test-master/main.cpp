#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <map>

#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

HINSTANCE hInst;

std::map<HWND, HDC>   map_hdc;
std::map<HWND, HGLRC> map_rendering_context;
std::map<HWND, int>   map_window_id;
int count = 0;
DWORD st = 0;

int window_w = 200;
int window_h = 200;

PIXELFORMATDESCRIPTOR pfd = { 
	sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
	1,                     // version number 
	PFD_DRAW_TO_WINDOW |   // support window 
	PFD_SUPPORT_OPENGL |   // support OpenGL 
	PFD_DOUBLEBUFFER,      // double buffered 
	PFD_TYPE_RGBA,         // RGBA type 
	24,                    // 24-bit color depth 
	0, 0, 0, 0, 0, 0,      // color bits ignored 
	0,                     // no alpha buffer 
	0,                     // shift bit ignored 
	0,                     // no accumulation buffer 
	0, 0, 0, 0,            // accum bits ignored 
	32,                    // 32-bit z-buffer 
	0,                     // no stencil buffer 
	0,                     // no auxiliary buffer 
	PFD_MAIN_PLANE,        // main layer 
	0,                     // reserved 
	0, 0, 0                // layer masks ignored 
}; 

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, char *str, int x, int y);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void                DrawOpenGL(int window_id);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;

	MyRegisterClass(hInstance);

	st = ::GetTickCount();

	InitInstance (hInstance, "window_id=0", 0, 0);
	InitInstance (hInstance, "window_id=1", window_w, 0);
	InitInstance (hInstance, "window_id=2", window_w * 2, 0);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "opengl_multi_window_test class";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, char *szTitle, int x, int y)
{
	HWND hWnd;

	hInst = hInstance;

	hWnd = CreateWindow("opengl_multi_window_test class", szTitle, WS_EX_WINDOWEDGE, x, y, window_w, window_h, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int pixel_format;
	HGLRC hrc;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		map_hdc[hWnd] = hdc;

		pixel_format = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pixel_format, &pfd);
		hrc = wglCreateContext(hdc);

		map_rendering_context[hWnd] = hrc;
		map_window_id[hWnd] = count;
		count ++;

		::SetTimer(hWnd, count, 15, NULL);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = map_hdc[hWnd];
		hrc = map_rendering_context[hWnd];
		wglMakeCurrent(hdc, hrc);
		DrawOpenGL(map_window_id[hWnd]);
		SwapBuffers(hdc);
		wglMakeCurrent(hdc, 0);
		return DefWindowProc(hWnd, message, wParam, lParam); 
			// see also... http://social.msdn.microsoft.com/Forums/vstudio/en-US/179ea901-dac4-4202-90b8-87c91b581932/

	case WM_DESTROY:
		hrc = map_rendering_context[hWnd];
		wglDeleteContext(hrc);
		hdc = map_hdc[hWnd];
		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == 27) {
			::PostMessageA(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

float diff()
{
	return (::GetTickCount() - st) / 1000.0f;
}

void DrawCube()
{
	GLdouble vertex[][3] = {
		{ -1.0, -1.0, -1.0 },
		{  1.0, -1.0, -1.0 },
		{  1.0,  1.0, -1.0 },
		{ -1.0,  1.0, -1.0 },
		{ -1.0, -1.0,  1.0 },
		{  1.0, -1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{ -1.0,  1.0,  1.0 } 
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	GLdouble color[][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 1.0, 0.0, 1.0 },
		{ 0.0, 1.0, 1.0 }
	};

	glBegin(GL_QUADS);
	for (int j = 0; j < 6; ++j) {
		glColor3dv(color[j]);
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();
};

void DrawOpenGL(int window_id)
{
	glEnable(GL_DEPTH_TEST);

	// clear 
	switch(window_id) {
	case 0:
		glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
		break;
	case 1:
		glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
		break;
	case 2:
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		break;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set viewport & camera
	glViewport(0, 0, window_w, window_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)window_w / (double)window_h, 1.0, 100.0);
	gluLookAt(0.0f, 0.0f, -7.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// draw cube
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float t = diff();

	glRotatef(t * 180, 1.0f, 0.0f, 0.0f);
	glRotatef(t *  90, 0.0f, 1.0f, 0.0f);
	glRotatef(t *  45, 0.0f, 0.0f, 1.0f);
	DrawCube();
}
