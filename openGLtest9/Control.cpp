
#include <Windows.h>
#include <GL/freeglut.h>
#include "Control.h"

//定义鼠标行为 glutMouseFunc
void Control::mouseCB(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //鼠标左键按下
	{
		mouseRotateX = x;//当左键按下时记录鼠标坐标
		mouseRotateY = y;
	}
	//else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) //鼠标右键按下
	//{
	//	mouseMoveX = x;
	//	mouseMoveY = y;
	//}
	else if (state == GLUT_UP && button == 3) {
		times_n = 0.08f + times;
		glutPostRedisplay();
	}
	else if (state == GLUT_UP && button == 4) {
		times_n = -0.08f + times;
		glutPostRedisplay();
	}
	times = times_n;
}

//定义鼠标动作 glutMotionFunc
void Control::mouseMotionCB(int x, int y)
{
	GLint deltax = mouseRotateX - x;
	GLint deltay = mouseRotateY - y;
	cameraAngleY += 360 * (GLfloat)deltax / (GLfloat)WIDE;//根据屏幕上鼠标滑动的距离来设置旋转的角度
	cameraAngleX += 360 * (GLfloat)deltay / (GLfloat)HEIGHT;

	GLint movex = mouseRotateX - x;
	GLint movey = mouseRotateY - y;
	cameraDistanceY += 0.001f*(GLfloat)movex / (GLfloat)WIDE;//根据屏幕上鼠标滑动的距离来设置移动的距离
	cameraDistanceX += 0.001f*(GLfloat)movey / (GLfloat)HEIGHT;

	mouseRotateX = x;//记录此时的鼠标坐标，更新鼠标坐标
	mouseRotateY = y;//若是没有这两句语句，滑动是旋转会变得不可控

	glutPostRedisplay();
}
