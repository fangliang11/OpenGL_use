#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <windows.h>
#include <GL/freeglut.h>

class GLContext
{
public:

	GLContext()
	{
		reset();
	}

	~GLContext()
	{
		purge();
	}

	void init(HWND hWnd)
	{
		// remember the window handle (HWND)
		mhWnd = hWnd;

		// get the device context (DC)��ȡ�豸�������
		mhDC = GetDC(mhWnd);

		// set the pixel format for the DC�������ظ�ʽ
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int format = ChoosePixelFormat(mhDC, &pfd);
		SetPixelFormat(mhDC, format, &pfd);

		// create the render context (RC)������ɫ������
		mhRC = wglCreateContext(mhDC);

		// make it the current render context����Ϊ��ǰ��ɫ������
		wglMakeCurrent(mhDC, mhRC);
	}

	void Stop() { purge(); }

	void SwapBuffer() { SwapBuffers(mhDC); }

private:

	void purge()
	{
		if (mhRC)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(mhRC);
		}
		if (mhWnd && mhDC)
		{
			ReleaseDC(mhWnd, mhDC);
		}
		reset();
	}

	void reset()
	{
		mhWnd = NULL;
		mhDC = NULL;
		mhRC = NULL;
	}

	HWND mhWnd;
	HDC mhDC;
	HGLRC mhRC;

};

#endif