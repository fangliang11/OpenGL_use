#include <GL\glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>

#define AXES_LEN 0.8
#define MAX_CHAR 128
#pragma comment (lib, "glew32d.lib")

using namespace std;

#include "GlutWin.h"

GlutWin::GlutWin(int windowHeight, int windowWidth, int windowPosX, int windowPosY,
	unsigned int displayMode, const char * windowTitle)
{
	windowTitle = windowTitle;
	windowHeight = windowHeight;
	windowWidth = windowWidth;
	windowPosX = windowPosX;
	windowPosY = windowPosY;
	displayMode = displayMode;
	fullScreen = false;

	char    cmd_line[8];
	char *  argv[1];
	argv[0] = cmd_line;
	int     argc = 1;

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitDisplayMode(displayMode);
	windowID = glutCreateWindow(windowTitle);

	const float ar = (float)windowWidth / (GLfloat)windowHeight;

#if 0
	glOrtho(-1.0, 1.0, -1.0 / ar, 1.0 / ar, -1.0, 1.0);
#else
	gluPerspective(45.0, ar, 0.1, 100.0);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
#endif // 透视投影、平行投影

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 黑
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(10.0, 10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
