#pragma once
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

//Glut≥ı ºªØ
class GlutWin
{
public:
	GlutWin(int windowHeight, int windowWidth, int windowPosX, int windowPosY,
		unsigned int displayMode, const char * windowTitle);
	~GlutWin() {};

private:
	const char *    windowTitle;
	int             windowHeight, windowWidth;
	int             windowPosX, windowPosY;
	int             windowID;
	unsigned int    displayMode;
	bool            fullScreen;
};
