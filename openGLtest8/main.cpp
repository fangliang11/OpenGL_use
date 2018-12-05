/*
*本程序实现3D可旋转缩放坐标系
*/

#include<stdio.h>
#include <Windows.h>
#include<GL/freeglut.h>
#include "readFile.h"
#include "modelGL.h"


using namespace std;


//程序初始化
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}



int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDE, WINDOW_HEIGHT);
	glutCreateWindow("三维坐标系");

	init();
	readFiles();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();

	return 0;
}