///////////////////////////////////////////////////////////////////////////////
// ControllerGL.h
// ==============
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//派生控制类， openGL窗口。控制渲染窗口，初始化渲染描述表、着色描述表
//为openGL渲染循环开启新线程
// When this class is constructed, it gets the pointers to model and view
// components.
//当这个类建立时，会获取模型和视图组件的指针
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2008-09-15
// UPDATED: 2017-10-18
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_CONTROLLER_GL_H
#define WIN_CONTROLLER_GL_H

#include <thread>
#include "Controller.h"
#include "ViewGL.h"
#include "ModelGL.h"


namespace Win
{
    class ControllerGL : public Controller
    {
    public:
        ControllerGL(ModelGL* model, ViewGL* view);
        ~ControllerGL() {};

        int close();                                // close the RC and OpenGL window
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int create();                               // create RC for OpenGL window and start new thread for rendering
        int paint();
        int lButtonDown(WPARAM state, int x, int y);
        int lButtonUp(WPARAM state, int x, int y);
        int rButtonDown(WPARAM state, int x, int y);
        int rButtonUp(WPARAM state, int x, int y);
        int mouseMove(WPARAM state, int x, int y);
        int mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

		int mButtonDown(WPARAM state, int x, int y);

    private:
        void runThread();                           // thread for OpenGL rendering

        ModelGL* model;                             // pointer to model component
        ViewGL* view;                               // pointer to view component
        std::thread glThread;                       // opengl rendering thread object
		std::thread glThread2;

        volatile bool loopFlag;                     // rendering loop flag

    };
}

#endif
