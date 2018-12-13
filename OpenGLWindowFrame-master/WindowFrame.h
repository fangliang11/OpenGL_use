
#ifndef WINDOWFRAME_H
#define WINDOWFRAME_H
#include <windows.h>
#include "glew.h"
#include "structs.h"

#define WM_TOGGLEFULLSCREEN (WM_USER+1)	                                                             // 自定义消息                                             

class WindowFrame
{
public:
	WindowFrame(void);
	~WindowFrame(void);
	BOOL Initialize();                                                                               // 初始化
	void ReshapeGL(int width, int height);								                             // 根据窗口大小，调整OpenGL视口大小
	void ToggleFullscreen(GL_Window* window);		                                                 // 切换窗口模式和全屏模式		
	void TerminateApplication (GL_Window* window);				                                     // 关闭程序
	BOOL DestroyWindowGL(GL_Window* window);							                             // 删除窗口	
	BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel);                            // 改变屏幕的大小和位深
	BOOL RegisterWindowClass(Application* application);						                         // 注册一个窗口		
	BOOL CreateWindowGL(GL_Window* window);		                                                     // 创建窗口
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg,	WPARAM wParam, LPARAM lParam);	             // 窗口函数
	static LRESULT CALLBACK MsgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);           // 消息路由	
	int MsgLoop(GL_Window* window);	                                                                 // 消息循环	
};

#endif