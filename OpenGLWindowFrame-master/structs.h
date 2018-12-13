
#ifndef STRUCTS_H
#define STRUCTS_H
#include <windows.h>

typedef struct {									// ���水����Ϣ
	BOOL keyDown [256];								// TRUE��ʾ������
} Keys;												

typedef struct {
	int x;                                          // x ����
    int y;                                          // y ����
} Mouse;

typedef struct {									// ���������Ϣ
	HINSTANCE		hInstance;						// ����ʵ��
	const char*		className;						// ���򴰿ڵ�����    
} Application;										

typedef struct {									// ������ʼ���ṹ
	Application*		application;				// ����ṹ
	char*				title;						// ���ڱ���
	int					width;						// ���ڿ��
	int					height;						// ���ڸ߶�
	int					bitsPerPixel;				// ����λ��	
} GL_WindowInit;									

typedef struct {									// ���洰����Ϣ
	Keys*				keys;						// �����ṹ
	Mouse              mouse;                       // ���ṹ
	HWND				hWnd;						// ���ھ��
	HDC					hDC;						// �����豸�������� 
	HGLRC				hRC;						// OpenGL��Ⱦ��������
	GL_WindowInit		init;						// ��ʼ�����ڽṹ
	BOOL				isVisible;					// �����Ƿ�ɼ�	
	BOOL                isProgramLooping;			// �Ƿ���г���ѭ��	
	BOOL				isFullScreen;				// �Ƿ�ȫ����ʾ
} GL_Window;										

#endif