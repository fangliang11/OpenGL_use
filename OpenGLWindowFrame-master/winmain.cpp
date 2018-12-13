
#include <windows.h>
#include "WindowFrame.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib" )		

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
	Application			application;				                    // 程序结构					
	GL_Window			window;					                    	// 窗口结构			
	Keys				keys;						                    // 按键结构		
			
	application.className = "OpenGLBookBar";		                    // 程序名为OpenGL					
	application.hInstance = hInstance;				                    // 保留窗口实例	

	ZeroMemory(&keys, sizeof(Keys));									// 清空保存按键的数组
	ZeroMemory(&window, sizeof(GL_Window));							    // 把这个结构清0
	window.keys					= &keys;								// 设置保存按键的数组
	window.init.application		= &application;							// 保存程序实例
	window.init.title			= "BaseWindowFrame";					// 设置标题
	window.init.width			= 640;									// 设置窗口宽度
	window.init.height			= 480;									// 设置窗口高度
	window.init.bitsPerPixel	= 32;									// 设置颜色深度为32									
	window.isFullScreen     	= TRUE;									// 设置去全屏幕状态
	window.isProgramLooping     = TRUE;                                 // 程序运行状态，缺省为真
	window.mouse.x              = 0;                                    // 初始化为零
	window.mouse.y              = 0;                                    // 初始化为零	
	
	if (MessageBox (HWND_DESKTOP, "你想在全屏状态下运行么?", "开启全屏运行", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		window.isFullScreen = FALSE;								
	}

	WindowFrame* windowframe = new WindowFrame();
	
	if (windowframe->RegisterWindowClass(&application) == FALSE)
		return -1;
	
	while (window.isProgramLooping)											
	{							
		if (windowframe->CreateWindowGL(&window) == TRUE)							
		{			
			if (windowframe->Initialize() == FALSE)					
			{				
				windowframe->TerminateApplication(&window);							
			}			
			else														
			{							
				windowframe->MsgLoop(&window);											
			}
			windowframe->DestroyWindowGL(&window);									
		}		
		else															
		{
			MessageBox (HWND_DESKTOP, "创建OpenGL窗口错误", "错误", MB_OK | MB_ICONEXCLAMATION);
			window.isProgramLooping = FALSE;									
		}
	}	
	
	UnregisterClass(application.className, application.hInstance);	
	delete(windowframe);
	return 0;
}																		
