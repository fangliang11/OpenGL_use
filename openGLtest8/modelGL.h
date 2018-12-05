#ifndef MODELGL_H
#define MODELGL_H

#include <Windows.h>
#include <iostream>
#include<GL/freeglut.h>


GLint WINDOW_WIDE = 800;
GLint WINDOW_HEIGHT = 800;
GLfloat mouseRotateX = 0, mouseRotateY = 0;
GLfloat mouseMoveX = 0, mouseMoveY = 0;
GLfloat cameraDistanceX = 0, cameraDistanceY = 0;
GLfloat cameraAngleX = 0, cameraAngleY = 0;
GLfloat times = 1, times_n = 1;
GLfloat scaleSizeX, scaleSizeY, scaleSizeZ;

int AXES_LEN = 5;

//坐标结构体定义
struct coordpoint {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

void GLGrid(coordpoint& pt1, coordpoint& pt2, coordpoint& pt3, coordpoint& pt4, coordpoint& pt5, coordpoint& pt6, int num);
void GLAxis();
void GLPoints();
void display();
void reshape(int w, int h);
void mouseCB(int button, int state, int x, int y);
void mouseMotionCB(int x, int y);


extern const int row_MAX = 10955;
extern const int column_MAX = 13;

extern float arry[row_MAX][column_MAX];
extern float arryX[row_MAX];
extern float arryY[row_MAX];
extern float arryZ[row_MAX];
extern float maxX;
extern float maxY;
extern float maxZ;
extern float minX;
extern float minY;
extern float minZ;
extern float cvalueX, cvalueY, cvalueZ;
extern int readFiles();

#endif