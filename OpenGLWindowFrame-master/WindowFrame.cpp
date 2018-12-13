
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);							    // 黑色背景
	glClearDepth(1.0f);													// 安装深度缓冲器
	glDepthFunc(GL_LEQUAL);												// 深度的类型测试
	glEnable(GL_DEPTH_TEST);											// 打开深度测试
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);					// 打开 Alpha 混合
	glEnable(GL_BLEND);													// 打开混合
	glAlphaFunc(GL_GREATER,0.1f);										// 设 Alpha 测试
	glEnable(GL_ALPHA_TEST);											// 打开 Alpha 测试
	glEnable(GL_TEXTURE_2D);											// 打开材质贴图
	glEnable(GL_CULL_FACE);												// 去掉画物体的背面	
	return TRUE;														// 返回正确 (设初值成功)
}
	
BOOL WindowFrame::RegisterWindowClass (Application* application)					
{						
	HICON icon = (HICON)(LoadImage(NULL, "appicon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));

	WNDCLASSEX windowClass;												// 窗口类
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// 清空结构为0
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// 窗口结构的大小
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 设置窗口类型为，移动时重画，并为窗口取得DC
	windowClass.lpfnWndProc		= (WNDPROC)(MsgRouter);			        // 消息路由
	windowClass.hInstance		= application->hInstance;				// 设置实例
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// 设置背景	
	windowClass.hIcon           = icon;                                 // 程序图标
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 载入光标
	windowClass.lpszClassName	= application->className;				// 设置类名

	if (RegisterClassEx (&windowClass) == 0)							// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;													// 退出并返回FALSE									
	}
	return TRUE;														
}

BOOL WindowFrame::ChangeScreenResolution (int width, int height, int bitsPerPixel)	
{
	DEVMODE dmScreenSettings;											// 设备模式
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// 清0
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// 设置为该结构的大小
	dmScreenSettings.dmPelsWidth		= width;						// 设置宽度
	dmScreenSettings.dmPelsHeight		= height;						// 设置高度
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// 设置位深
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;			// 设置需要改变的地方
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 	// 改变显示模式
	{
		return FALSE;													// 出错则返回
	}
	return TRUE;														// 成功返回
}

BOOL WindowFrame::CreateWindowGL (GL_Window* window)									
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// 设置窗口类型为一般的弹出式窗口
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// 设置窗口的扩展属性

	PIXELFORMATDESCRIPTOR pfd =											// pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{																	
		sizeof (PIXELFORMATDESCRIPTOR),									// 上述格式描述符的大小
		1,																// 版本号
		PFD_DRAW_TO_WINDOW |											// 格式支持窗口
		PFD_SUPPORT_OPENGL |											// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,												// 必须支持双缓冲
		PFD_TYPE_RGBA,													// 申请 RGBA 格式
		window->init.bitsPerPixel,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,												// 忽略的色彩位
		0,																// 无Alpha缓存
		0,																// 忽略Shift Bit
		0,																// 无累加缓存
		0, 0, 0, 0,														// 忽略聚集位
		16,																// 16位 Z-缓存 (深度缓存) 
		0,																// 无蒙板缓存
		0,																// 无辅助缓存
		PFD_MAIN_PLANE,													// 主绘图层
		0,																// 不使用重叠层
		0, 0, 0															// 忽略层遮罩
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// 记录窗口的大小

	GLuint PixelFormat;													// 将保存的像素格式

	if (window->isFullScreen == TRUE)								    // 如果为全屏模式
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{			
			MessageBox (HWND_DESKTOP, "模式切换失败\n在窗口模式下运行", "错误", MB_OK | MB_ICONEXCLAMATION);
			window->isFullScreen = FALSE;							    // 设置isFullscreen为False
		}		
		else															
		{
			ShowCursor(FALSE);											// 不显示光标
			windowStyle = WS_POPUP;										// 设置窗口模式为弹出式窗口
			windowExtendedStyle |= WS_EX_TOPMOST;						// 设置窗口的扩展模式为最顶层窗口
		}																
	}
	else																// 如果不为全屏模式
	{		
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle); //调整窗口大小，以容纳边框等窗口部件
	}
	
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// 扩展窗体风格
								   window->init.application->className,	// 类名字
								   window->init.title,					// 窗口标题
								   windowStyle,							// 必须的窗体风格属性
								   0, 0,								// 窗口位置
								   windowRect.right - windowRect.left,	// 计算调整好的窗口宽度
								   windowRect.bottom - windowRect.top,	// 计算调整好的窗口高度
								   HWND_DESKTOP,						// 无父窗口
								   0,									// 无菜单
								   window->init.application->hInstance, // 实例
								   window);								// 向window的WM_CREATE传递window结构
																		
	if (window->hWnd == 0)												// 窗口创建是否成功	
	{
		return FALSE;													// 不成功则返回
	}

	window->hDC = GetDC (window->hWnd);									// 返回设备描述表
	if (window->hDC == 0)												// 是否成功获得设备描述表
	{		
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回失败
	}

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// 选择一种相容的像素格式
	if (PixelFormat == 0)												// 是否获得相容的像素格式
	{		
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// 设置像素格式
	{		
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	window->hRC = wglCreateContext (window->hDC);						// 设置OpenGL渲染描述表
	if (window->hRC == 0)												// 如果没有成功创建
	{
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)				// 设置RC为当前的OpenGL的渲染描述表
	{
		wglDeleteContext (window->hRC);									// 删除当前的OpenGL渲染描述表
		window->hRC = 0;												// 释放设备描述表
		ReleaseDC (window->hWnd, window->hDC);							// 设置窗口设备描述表为0
		window->hDC = 0;												// 删除窗口
		DestroyWindow (window->hWnd);									// 设置窗口句柄为0
		window->hWnd = 0;												// 返回错误
		return FALSE;													
	}

	ShowWindow (window->hWnd, SW_NORMAL);								// 显示窗口
	window->isVisible = TRUE;											// 设置窗口为可见

	ReshapeGL (window->init.width, window->init.height);				// 根据窗口大小，调整OpenGL对应的窗口大小
	ZeroMemory (window->keys, sizeof (Keys));							// 初始化按键记录表	

	return TRUE;														// 窗口创建完毕，成功返回																		
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
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// 设置窗口可见区为窗口大小
	glMatrixMode (GL_PROJECTION);										// 设置投影矩阵
	glLoadIdentity ();													// 重置它
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// 设置可视冷苔体
					1.0f, 100.0f);		
	glMatrixMode (GL_MODELVIEW);										// 设置为模型变换矩阵
	glLoadIdentity ();													// 重置它
}

void WindowFrame::TerminateApplication(GL_Window* window)							
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// 发送退出命令		
	window->isProgramLooping = FALSE;				                    // 停止程序循环
}

BOOL WindowFrame::DestroyWindowGL(GL_Window* window)							
{
	if (window->hWnd != 0)												// 如果窗口句柄不为0
	{	
		if (window->hDC != 0)											// 如果窗口设备描述表不为0
		{
			wglMakeCurrent (window->hDC, 0);							// 释放当前的OpenGL渲染描述表
			if (window->hRC != 0)										
			{
				wglDeleteContext (window->hRC);						
				window->hRC = 0;										

			}
			ReleaseDC (window->hWnd, window->hDC);						// 释放窗口设备描述表	
			window->hDC = 0;											
		}
		DestroyWindow(window->hWnd);									// 删除窗口
		window->hWnd = 0;												
	}
	
	ChangeDisplaySettings(NULL,0);	                                    // 总是设置为初始模式
	ShowCursor(TRUE);													// 显示鼠标
	return TRUE;														// 删除窗口完毕，成功返回
}

LRESULT CALLBACK WindowFrame::WindowProc(HWND hWnd, UINT uMsg,	 WPARAM wParam,	LPARAM lParam)	
{
	// 返回创建信息
	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));
	
	switch (uMsg)														
	{
		case WM_SYSCOMMAND:												// 捕获系统消息
		{
			switch (wParam)												// 检测系统调用
			{
				case SC_SCREENSAVE:										// 如果是要运行屏保程序
				case SC_MONITORPOWER:									// 如果是要关闭显示器
				return 0;												// 截获上面的消息，不做处理
			}
			break;														
		}
		return 0;														

		case WM_CREATE:													// 窗口创建
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// 获得创建的窗口信息
			window = (GL_Window*)(creation->lpCreateParams);			
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														

		case WM_CLOSE:													// 关闭窗口
			TerminateApplication(window);								// 结束程序
		return 0;														

		case WM_SIZE:													// 改变大小
			switch (wParam)												
			{
				case SIZE_MINIMIZED:									// 最小化
					window->isVisible = FALSE;							// 如果是，设窗口不可见
				return 0;												

				case SIZE_MAXIMIZED:									// 最大化
					window->isVisible = TRUE;							// 设置窗口可见
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// 更改OpenGL窗口的大小
				return 0;												

				case SIZE_RESTORED:										// 创口是否被重新载入了
					window->isVisible = TRUE;							// 设置窗口可见
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// 更改OpenGL窗口的大小
				return 0;												
			}
		break;															

		case WM_KEYDOWN:												// 更新按键控制
			if ((wParam >= 0) && (wParam <= 255))						// 如果为键盘支持的256个字符
			{				
				window->keys->keyDown [wParam] = TRUE;					// 记录那个字符被按下				
				return 0;												
			}
		break;															

		case WM_KEYUP:													// 更新按键控制
			if ((wParam >= 0) && (wParam <= 255))						
			{				
				window->keys->keyDown [wParam] = FALSE;					// 记录那个字符被释放				
				return 0;												
			}
		break;															

		case WM_TOGGLEFULLSCREEN:										// 切换全拼/窗口模式			
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
				DispatchMessage (&msg);	                                // 如果不是退出消息，把消息发送到处理函数
			}			
			else	
			{				
				isMessagePumpActive = FALSE;                            // 否则结束退出
			}
		}		
		else                                                            // 如果没有消息	
		{			
			if (window->isVisible == FALSE)	                            // 窗口不可见
			{				
				WaitMessage ();	                                        // 等待下一条消息	
			}			
			else                                                        // 窗口可见	
			{	
				if (window->keys->keyDown[VK_ESCAPE])				    // 按下 ESC?
				{
					TerminateApplication(window);						// 推出程序					
				}
				if (window->keys->keyDown[VK_F1])						// 按下f1?
				{
					ToggleFullscreen(window);							// 换到全屏模式					
				}
				//Draw();                                               // 绘制场景					
				SwapBuffers(window->hDC);		                        // 交换帧缓存			
			}
		}
	}	
	
	return (msg.wParam);
}