
#include "WindowFrame.h"

WindowFrame::WindowFrame(void)
{	
}

WindowFrame::~WindowFrame(void)
{
}	  

void WindowFrame::ToggleFullscreen (GL_Window* window)						
{
	PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				
}

BOOL WindowFrame::Initialize()								
{	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);							    // ��ɫ����
	glClearDepth(1.0f);													// ��װ��Ȼ�����
	glDepthFunc(GL_LEQUAL);												// ��ȵ����Ͳ���
	glEnable(GL_DEPTH_TEST);											// ����Ȳ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);					// �� Alpha ���
	glEnable(GL_BLEND);													// �򿪻��
	glAlphaFunc(GL_GREATER,0.1f);										// �� Alpha ����
	glEnable(GL_ALPHA_TEST);											// �� Alpha ����
	glEnable(GL_TEXTURE_2D);											// �򿪲�����ͼ
	glEnable(GL_CULL_FACE);												// ȥ��������ı���	
	return TRUE;														// ������ȷ (���ֵ�ɹ�)
}
	
BOOL WindowFrame::RegisterWindowClass (Application* application)					
{						
	HICON icon = (HICON)(LoadImage(NULL, "appicon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));

	WNDCLASSEX windowClass;												// ������
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// ��սṹΪ0
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// ���ڽṹ�Ĵ�С
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	windowClass.lpfnWndProc		= (WNDPROC)(MsgRouter);			        // ��Ϣ·��
	windowClass.hInstance		= application->hInstance;				// ����ʵ��
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// ���ñ���	
	windowClass.hIcon           = icon;                                 // ����ͼ��
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// ������
	windowClass.lpszClassName	= application->className;				// ��������

	if (RegisterClassEx (&windowClass) == 0)							// ����ע�ᴰ����
	{
		MessageBox(NULL,"ע�ᴰ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;													// �˳�������FALSE									
	}
	return TRUE;														
}

BOOL WindowFrame::ChangeScreenResolution (int width, int height, int bitsPerPixel)	
{
	DEVMODE dmScreenSettings;											// �豸ģʽ
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// ��0
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// ����Ϊ�ýṹ�Ĵ�С
	dmScreenSettings.dmPelsWidth		= width;						// ���ÿ��
	dmScreenSettings.dmPelsHeight		= height;						// ���ø߶�
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// ����λ��
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;			// ������Ҫ�ı�ĵط�
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 	// �ı���ʾģʽ
	{
		return FALSE;													// �����򷵻�
	}
	return TRUE;														// �ɹ�����
}

BOOL WindowFrame::CreateWindowGL (GL_Window* window)									
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// ���ô�������Ϊһ��ĵ���ʽ����
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// ���ô��ڵ���չ����

	PIXELFORMATDESCRIPTOR pfd =											// pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{																	
		sizeof (PIXELFORMATDESCRIPTOR),									// ������ʽ�������Ĵ�С
		1,																// �汾��
		PFD_DRAW_TO_WINDOW |											// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |											// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,												// ����֧��˫����
		PFD_TYPE_RGBA,													// ���� RGBA ��ʽ
		window->init.bitsPerPixel,										// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,												// ���Ե�ɫ��λ
		0,																// ��Alpha����
		0,																// ����Shift Bit
		0,																// ���ۼӻ���
		0, 0, 0, 0,														// ���Ծۼ�λ
		16,																// 16λ Z-���� (��Ȼ���) 
		0,																// ���ɰ建��
		0,																// �޸�������
		PFD_MAIN_PLANE,													// ����ͼ��
		0,																// ��ʹ���ص���
		0, 0, 0															// ���Բ�����
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// ��¼���ڵĴ�С

	GLuint PixelFormat;													// ����������ظ�ʽ

	if (window->isFullScreen == TRUE)								    // ���Ϊȫ��ģʽ
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{			
			MessageBox (HWND_DESKTOP, "ģʽ�л�ʧ��\n�ڴ���ģʽ������", "����", MB_OK | MB_ICONEXCLAMATION);
			window->isFullScreen = FALSE;							    // ����isFullscreenΪFalse
		}		
		else															
		{
			ShowCursor(FALSE);											// ����ʾ���
			windowStyle = WS_POPUP;										// ���ô���ģʽΪ����ʽ����
			windowExtendedStyle |= WS_EX_TOPMOST;						// ���ô��ڵ���չģʽΪ��㴰��
		}																
	}
	else																// �����Ϊȫ��ģʽ
	{		
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle); //�������ڴ�С�������ɱ߿�ȴ��ڲ���
	}
	
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// ��չ������
								   window->init.application->className,	// ������
								   window->init.title,					// ���ڱ���
								   windowStyle,							// ����Ĵ���������
								   0, 0,								// ����λ��
								   windowRect.right - windowRect.left,	// ��������õĴ��ڿ��
								   windowRect.bottom - windowRect.top,	// ��������õĴ��ڸ߶�
								   HWND_DESKTOP,						// �޸�����
								   0,									// �޲˵�
								   window->init.application->hInstance, // ʵ��
								   window);								// ��window��WM_CREATE����window�ṹ
																		
	if (window->hWnd == 0)												// ���ڴ����Ƿ�ɹ�	
	{
		return FALSE;													// ���ɹ��򷵻�
	}

	window->hDC = GetDC (window->hWnd);									// �����豸������
	if (window->hDC == 0)												// �Ƿ�ɹ�����豸������
	{		
		DestroyWindow (window->hWnd);									// ɾ������
		window->hWnd = 0;												// ���ô��ھ��Ϊ0
		return FALSE;													// ����ʧ��
	}

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// ѡ��һ�����ݵ����ظ�ʽ
	if (PixelFormat == 0)												// �Ƿ������ݵ����ظ�ʽ
	{		
		ReleaseDC (window->hWnd, window->hDC);							// �ͷ��豸������
		window->hDC = 0;												// ���ô����豸������Ϊ0
		DestroyWindow (window->hWnd);									// ɾ������
		window->hWnd = 0;												// ���ô��ھ��Ϊ0
		return FALSE;													// ���ش���
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// �������ظ�ʽ
	{		
		ReleaseDC (window->hWnd, window->hDC);							// �ͷ��豸������
		window->hDC = 0;												// ���ô����豸������Ϊ0
		DestroyWindow (window->hWnd);									// ɾ������
		window->hWnd = 0;												// ���ô��ھ��Ϊ0
		return FALSE;													// ���ش���
	}

	window->hRC = wglCreateContext (window->hDC);						// ����OpenGL��Ⱦ������
	if (window->hRC == 0)												// ���û�гɹ�����
	{
		ReleaseDC (window->hWnd, window->hDC);							// �ͷ��豸������
		window->hDC = 0;												// ���ô����豸������Ϊ0
		DestroyWindow (window->hWnd);									// ɾ������
		window->hWnd = 0;												// ���ô��ھ��Ϊ0
		return FALSE;													// ���ش���
	}

	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)				// ����RCΪ��ǰ��OpenGL����Ⱦ������
	{
		wglDeleteContext (window->hRC);									// ɾ����ǰ��OpenGL��Ⱦ������
		window->hRC = 0;												// �ͷ��豸������
		ReleaseDC (window->hWnd, window->hDC);							// ���ô����豸������Ϊ0
		window->hDC = 0;												// ɾ������
		DestroyWindow (window->hWnd);									// ���ô��ھ��Ϊ0
		window->hWnd = 0;												// ���ش���
		return FALSE;													
	}

	ShowWindow (window->hWnd, SW_NORMAL);								// ��ʾ����
	window->isVisible = TRUE;											// ���ô���Ϊ�ɼ�

	ReshapeGL (window->init.width, window->init.height);				// ���ݴ��ڴ�С������OpenGL��Ӧ�Ĵ��ڴ�С
	ZeroMemory (window->keys, sizeof (Keys));							// ��ʼ��������¼��	

	return TRUE;														// ���ڴ�����ϣ��ɹ�����																		
}


LRESULT CALLBACK WindowFrame::MsgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowFrame* pWnd;

	if (uMsg == WM_NCCREATE)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
	}
	
	pWnd =  (WindowFrame *)GetWindowLong(hwnd, GWL_USERDATA);           // get the pointer to the window	

	// if we have the pointer, go to the message handler of the window else, use DefWindowProc
	if (pWnd)
		return pWnd->WindowProc(hwnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowFrame::ReshapeGL (int width, int height)								
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// ���ô��ڿɼ���Ϊ���ڴ�С
	glMatrixMode (GL_PROJECTION);										// ����ͶӰ����
	glLoadIdentity ();													// ������
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// ���ÿ�����̦��
					1.0f, 100.0f);		
	glMatrixMode (GL_MODELVIEW);										// ����Ϊģ�ͱ任����
	glLoadIdentity ();													// ������
}

void WindowFrame::TerminateApplication(GL_Window* window)							
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// �����˳�����		
	window->isProgramLooping = FALSE;				                    // ֹͣ����ѭ��
}

BOOL WindowFrame::DestroyWindowGL(GL_Window* window)							
{
	if (window->hWnd != 0)												// ������ھ����Ϊ0
	{	
		if (window->hDC != 0)											// ��������豸������Ϊ0
		{
			wglMakeCurrent (window->hDC, 0);							// �ͷŵ�ǰ��OpenGL��Ⱦ������
			if (window->hRC != 0)										
			{
				wglDeleteContext (window->hRC);						
				window->hRC = 0;										

			}
			ReleaseDC (window->hWnd, window->hDC);						// �ͷŴ����豸������	
			window->hDC = 0;											
		}
		DestroyWindow(window->hWnd);									// ɾ������
		window->hWnd = 0;												
	}
	
	ChangeDisplaySettings(NULL,0);	                                    // ��������Ϊ��ʼģʽ
	ShowCursor(TRUE);													// ��ʾ���
	return TRUE;														// ɾ��������ϣ��ɹ�����
}

LRESULT CALLBACK WindowFrame::WindowProc(HWND hWnd, UINT uMsg,	 WPARAM wParam,	LPARAM lParam)	
{
	// ���ش�����Ϣ
	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));
	
	switch (uMsg)														
	{
		case WM_SYSCOMMAND:												// ����ϵͳ��Ϣ
		{
			switch (wParam)												// ���ϵͳ����
			{
				case SC_SCREENSAVE:										// �����Ҫ������������
				case SC_MONITORPOWER:									// �����Ҫ�ر���ʾ��
				return 0;												// �ػ��������Ϣ����������
			}
			break;														
		}
		return 0;														

		case WM_CREATE:													// ���ڴ���
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// ��ô����Ĵ�����Ϣ
			window = (GL_Window*)(creation->lpCreateParams);			
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														

		case WM_CLOSE:													// �رմ���
			TerminateApplication(window);								// ��������
		return 0;														

		case WM_SIZE:													// �ı��С
			switch (wParam)												
			{
				case SIZE_MINIMIZED:									// ��С��
					window->isVisible = FALSE;							// ����ǣ��贰�ڲ��ɼ�
				return 0;												

				case SIZE_MAXIMIZED:									// ���
					window->isVisible = TRUE;							// ���ô��ڿɼ�
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// ����OpenGL���ڵĴ�С
				return 0;												

				case SIZE_RESTORED:										// �����Ƿ�����������
					window->isVisible = TRUE;							// ���ô��ڿɼ�
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// ����OpenGL���ڵĴ�С
				return 0;												
			}
		break;															

		case WM_KEYDOWN:												// ���°�������
			if ((wParam >= 0) && (wParam <= 255))						// ���Ϊ����֧�ֵ�256���ַ�
			{				
				window->keys->keyDown [wParam] = TRUE;					// ��¼�Ǹ��ַ�������				
				return 0;												
			}
		break;															

		case WM_KEYUP:													// ���°�������
			if ((wParam >= 0) && (wParam <= 255))						
			{				
				window->keys->keyDown [wParam] = FALSE;					// ��¼�Ǹ��ַ����ͷ�				
				return 0;												
			}
		break;															

		case WM_TOGGLEFULLSCREEN:										// �л�ȫƴ/����ģʽ			
			window->isFullScreen = (window->isFullScreen == TRUE)? FALSE : TRUE;
			PostMessage(hWnd, WM_QUIT, 0, 0);
		break;				

		case WM_LBUTTONDOWN:
			{
				window->mouse.x = LOWORD(lParam);          
				window->mouse.y = HIWORD(lParam);				
			}
		break;

		case WM_MOUSEMOVE:
			{
	            window->mouse.x = LOWORD(lParam);          
				window->mouse.y = HIWORD(lParam);
			}
		break;
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					
}


int WindowFrame::MsgLoop(GL_Window* window)
{
	MSG	 msg;
	BOOL isMessagePumpActive = TRUE;

	while (isMessagePumpActive == TRUE)
	{		
		if (PeekMessage (&msg, window->hWnd, 0, 0, PM_REMOVE) != 0)
		{			
			if (msg.message != WM_QUIT)	
			{
				DispatchMessage (&msg);	                                // ��������˳���Ϣ������Ϣ���͵�������
			}			
			else	
			{				
				isMessagePumpActive = FALSE;                            // ��������˳�
			}
		}		
		else                                                            // ���û����Ϣ	
		{			
			if (window->isVisible == FALSE)	                            // ���ڲ��ɼ�
			{				
				WaitMessage ();	                                        // �ȴ���һ����Ϣ	
			}			
			else                                                        // ���ڿɼ�	
			{	
				if (window->keys->keyDown[VK_ESCAPE])				    // ���� ESC?
				{
					TerminateApplication(window);						// �Ƴ�����					
				}
				if (window->keys->keyDown[VK_F1])						// ����f1?
				{
					ToggleFullscreen(window);							// ����ȫ��ģʽ					
				}
				//Draw();                                               // ���Ƴ���					
				SwapBuffers(window->hDC);		                        // ����֡����			
			}
		}
	}	
	
	return (msg.wParam);
}