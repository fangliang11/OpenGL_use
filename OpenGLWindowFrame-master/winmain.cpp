
#include <windows.h>
#include "WindowFrame.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib" )		

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
	Application			application;				                    // ����ṹ					
	GL_Window			window;					                    	// ���ڽṹ			
	Keys				keys;						                    // �����ṹ		
			
	application.className = "OpenGLBookBar";		                    // ������ΪOpenGL					
	application.hInstance = hInstance;				                    // ��������ʵ��	

	ZeroMemory(&keys, sizeof(Keys));									// ��ձ��水��������
	ZeroMemory(&window, sizeof(GL_Window));							    // ������ṹ��0
	window.keys					= &keys;								// ���ñ��水��������
	window.init.application		= &application;							// �������ʵ��
	window.init.title			= "BaseWindowFrame";					// ���ñ���
	window.init.width			= 640;									// ���ô��ڿ��
	window.init.height			= 480;									// ���ô��ڸ߶�
	window.init.bitsPerPixel	= 32;									// ������ɫ���Ϊ32									
	window.isFullScreen     	= TRUE;									// ����ȥȫ��Ļ״̬
	window.isProgramLooping     = TRUE;                                 // ��������״̬��ȱʡΪ��
	window.mouse.x              = 0;                                    // ��ʼ��Ϊ��
	window.mouse.y              = 0;                                    // ��ʼ��Ϊ��	
	
	if (MessageBox (HWND_DESKTOP, "������ȫ��״̬������ô?", "����ȫ������", MB_YESNO | MB_ICONQUESTION) == IDNO)
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
			MessageBox (HWND_DESKTOP, "����OpenGL���ڴ���", "����", MB_OK | MB_ICONEXCLAMATION);
			window.isProgramLooping = FALSE;									
		}
	}	
	
	UnregisterClass(application.className, application.hInstance);	
	delete(windowframe);
	return 0;
}																		
