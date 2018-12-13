///////////////////////////////////////////////////////////////////////////////
// ControllerGL.cpp
// ================
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2006-07-09
// UPDATED: 2017-11-10
///////////////////////////////////////////////////////////////////////////////

#include "ControllerGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL(ModelGL* model, ViewGL* view) : model(model), view(view),
                                                           loopFlag(false)
{
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::close()
{
    // wait for rendering thread is terminated
    loopFlag = false;
    glThread.join();

    ::DestroyWindow(handle);
    Win::log("OpenGL window is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::create()
{
    // create a OpenGL rendering context
    if(!view->createContext(handle, 32, 24, 8))
    {
        Win::log(L"[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
        return -1;
    }

    // create a thread for OpenGL rendering
    glThread = std::thread(&ControllerGL::runThread, this);
    loopFlag = true;
    Win::log(L"Created a rendering thread for OpenGL.");

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_PAINT
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::paint()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::command(int id, int cmd, LPARAM msg)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// rendering thread
// initialize OpenGL states and start rendering loop
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::runThread()
{
    // set the current RC in this thread
    ::wglMakeCurrent(view->getDC(), view->getRC());

    // initialize OpenGL states
    model->init();
    Win::log(L"Initialized OpenGL states.");

    // load bmp and create texture
    HBITMAP hBitmap = (HBITMAP)LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_EARTH),
                                         IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    if(hBitmap)
    {
        // get bitmap info
        BITMAPINFO bitmap;
        bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        ::GetDIBits(view->getDC(), hBitmap, 0, 0, 0, &bitmap, DIB_RGB_COLORS);
        int width = bitmap.bmiHeader.biWidth;
        int height = bitmap.bmiHeader.biHeight;         // negative means top-to-bottom orientation
        int bitCount = bitmap.bmiHeader.biBitCount;     // bits per pixel
        int byteCount = bitmap.bmiHeader.biSizeImage;   // # of bytes of bitmap

        // get bitmap data as BGR
        unsigned char* buffer = new unsigned char[byteCount];
        int result = ::GetDIBits(view->getDC(), hBitmap, 0, height, buffer, &bitmap, DIB_RGB_COLORS);
        if(result > 0)
        {
            // convert BGR to RGB
            swapRedBlue(buffer, byteCount, bitCount >> 3);
            // flip vertically
            flipImage(buffer, width, height, bitCount >> 3);
            Win::log(L"Loaded a bitmap: %d x %d, %d bits", width, height, bitCount);

            // create texture
            model->createTexture(width, height, bitCount, buffer);
            Win::log(L"Created OpenGL texture object.");
        }
        else
        {
            Win::log(L"[ERROR] Failed to get bitmap data.");
        }

        delete [] buffer;
    }
    else
    {
        Win::log(L"[ERROR] Failed to load earth.bmp.");
    }

    // cofigure projection matrix
    RECT rect;
    ::GetClientRect(handle, &rect);
    model->resizeWindow(rect.right, rect.bottom);
    Win::log(L"Initialized OpenGL viewport and projection matrix.");

    // rendering loop
    Win::log(L"Entering OpenGL rendering thread...");
    while(loopFlag)
    {
        //std::this_thread::yield();      // yield to other processes or threads
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  // yield to other processes or threads
        model->draw();
        view->swapBuffers();
    }

    // close OpenGL Rendering Context (RC)
    view->closeContext(handle);
    ::wglMakeCurrent(0, 0);             // unset RC
    Win::log(L"Closed OpenGL context.");

    Win::log(L"Exit OpenGL rendering thread.");
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    if(state == MK_LBUTTON)
    {
        model->setMouseLeft(true);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    model->setMouseLeft(false);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    if(state == MK_RBUTTON)
    {
        model->setMouseRight(true);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    model->setMouseRight(false);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
    // for tracking mouse hover/leave tracking
    if(!mouseHovered)
    {
        // mouse hover/leave tracking
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = handle;
        tme.dwHoverTime = HOVER_DEFAULT;
        ::TrackMouseEvent(&tme);
        mouseHovered = true;
    }

    if(state == MK_LBUTTON)
    {
        model->rotateCamera(x, y);
    }
    if(state == MK_RBUTTON)
    {
        model->zoomCamera(y);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEHOVER
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseHover(int state, int x, int y)
{
    mouseHovered = true;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSELEAVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseLeave()
{
    mouseHovered = false;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseWheel(int state, int delta, int x, int y)
{
    if(mouseHovered)
    {
        model->zoomCameraByWheel(delta / 120.0f * 0.5f);
        paint();
    }
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::size(int w, int h, WPARAM wParam)
{
    model->resizeWindow(w, h);
    Win::log(L"OpenGL window is resized: %dx%d.", w, h);
    return 0;
}




///////////////////////////////////////////////////////////////////////////////
// convert BGR to RGB or vice versa
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::swapRedBlue(unsigned char* data, int dataSize, int channelCount)
{
    if(!data) return;
    if(channelCount < 3) return;            // must be 3 or 4
    if(dataSize % channelCount) return;     // must be divisible by the number of channels

    unsigned char tmp;
    int i;

    // swap the position of red and blue components
    for(i=0; i < dataSize; i+=channelCount)
    {
        tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }
}



///////////////////////////////////////////////////////////////////////////////
// flip bitmap vertically
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::flipImage(unsigned char* data, int width, int height, int channelCount)
{
    if(!data) return;

    int lineSize = width * channelCount;
    unsigned char* tmp = new unsigned char [lineSize];
    int half = height / 2;

    int line1 = 0;                          // first line
    int line2 = (height - 1) * lineSize;    // last line

    // scan only half of height
    for(int i = 0; i < half; ++i)
    {
        // copy line by line
        memcpy(tmp, &data[line1], lineSize);
        memcpy(&data[line1], &data[line2], lineSize);
        memcpy(&data[line2], tmp, lineSize);

        // move to next line
        line1 += lineSize;
        line2 -= lineSize;
    }

    // deallocate temp arrays
    delete [] tmp;
}
