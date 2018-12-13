
#ifndef WINDOWFRAME_H
#define WINDOWFRAME_H
#include <windows.h>
#include "glew.h"
#include "structs.h"

#define WM_TOGGLEFULLSCREEN (WM_USER+1)	                                                             // �Զ�����Ϣ                                             

class WindowFrame
{
public:
	WindowFrame(void);
	~WindowFrame(void);
	BOOL Initialize();                                                                               // ��ʼ��
	void ReshapeGL(int width, int height);								                             // ���ݴ��ڴ�С������OpenGL�ӿڴ�С
	void ToggleFullscreen(GL_Window* window);		                                                 // �л�����ģʽ��ȫ��ģʽ		
	void TerminateApplication (GL_Window* window);				                                     // �رճ���
	BOOL DestroyWindowGL(GL_Window* window);							                             // ɾ������	
	BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel);                            // �ı���Ļ�Ĵ�С��λ��
	BOOL RegisterWindowClass(Application* application);						                         // ע��һ������		
	BOOL CreateWindowGL(GL_Window* window);		                                                     // ��������
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg,	WPARAM wParam, LPARAM lParam);	             // ���ں���
	static LRESULT CALLBACK MsgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);           // ��Ϣ·��	
	int MsgLoop(GL_Window* window);	                                                                 // ��Ϣѭ��	
};

#endif