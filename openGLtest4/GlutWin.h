#pragma once
#pragma once
#include <windows.h>
#include "freeglut.h"

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
