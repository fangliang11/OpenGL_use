//
//

#ifndef	 CONTROL_H
#define  CONTROL_H


#include <Windows.h>
#include <GL/freeglut.h>

class Control
{
public:
	GLfloat WIDE;
	GLfloat HEIGHT;
	GLfloat times;
	GLfloat cameraDistanceX;
	GLfloat cameraDistanceY;
	GLfloat cameraAngleX;
	GLfloat cameraAngleY;
	GLfloat scaleSizeX;
	GLfloat scaleSizeY;
	GLfloat scaleSizeZ;


	void mouseCB(int button, int state, int x, int y);

	void mouseMotionCB(int x, int y);


private:

	GLfloat times_n;
	GLfloat mouseRotateX,  mouseRotateY;
	GLfloat mouseMoveX , mouseMoveY;

	   

};

#endif