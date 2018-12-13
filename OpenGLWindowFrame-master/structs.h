
#ifndef STRUCTS_H
#define STRUCTS_H
#include <windows.h>

typedef struct {									// 保存按键信息
	BOOL keyDown [256];								// TRUE表示键按下
} Keys;												

typedef struct {
	int x;                                          // x 坐标
    int y;                                          // y 坐标
} Mouse;

typedef struct {									// 保存程序信息
	HINSTANCE		hInstance;						// 程序实例
	const char*		className;						// 程序窗口的类名    
} Application;										

typedef struct {									// 创建初始化结构
	Application*		application;				// 程序结构
	char*				title;						// 窗口标题
	int					width;						// 窗口宽度
	int					height;						// 窗口高度
	int					bitsPerPixel;				// 像素位深	
} GL_WindowInit;									

typedef struct {									// 保存窗口信息
	Keys*				keys;						// 按键结构
	Mouse              mouse;                       // 鼠标结构
	HWND				hWnd;						// 窗口句柄
	HDC					hDC;						// 窗口设备描述表句柄 
	HGLRC				hRC;						// OpenGL渲染描述表句柄
	GL_WindowInit		init;						// 初始化窗口结构
	BOOL				isVisible;					// 窗口是否可见	
	BOOL                isProgramLooping;			// 是否进行程序循环	
	BOOL				isFullScreen;				// 是否全屏显示
} GL_Window;										

#endif