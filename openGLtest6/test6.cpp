/*
*该程序实现点，线，面的绘制
*/

#include <GL/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#pragma comment (lib, "glew32d.lib")

void draw()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//绿色画线
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, -0.1);
	glVertex2f(-0.1, 0.0);
	glVertex2f(-0.1, -0.1);
	glEnd();

	// 绘制x、y、z轴线段
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); //绘制线段的起点，这里为相对坐标，即相将glTranslatef移动后的点作为原点
	glVertex3f(3.0f, 0.0f, 0.0f); //确定第一条线段的终点（3，0，0） 
	glVertex3f(0.0f, 0.0f, 0.0f); //确定第二条线段的起点（0，0，0）
	glVertex3f(0.0f, 3.0f, 0.0f); //确定第二条线段的终点（0，3，0）
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();

	//平移起点并重新设置画笔颜色用于区分绘制的物体
	glTranslatef(-1.0f, -1.0f, -6.0f);
	glColor3f(0.5f, 0.5f, 1.0f);

	//绘制一个矩形 
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f); // Top Left
	glVertex3f(0.0f, 3.0f, 0.0f); // Top Right
	glVertex3f(0.0f, 3.0f, 3.0f); // Bottom Right
	glVertex3f(0.0f, 0.0f, 3.0f); // Bottom Left
	glEnd();

	//更改画笔颜色，并绘制另外一个矩形 
	glColor3f(0.0f, 0.0f, 1.0f); // Set The Color To Blue One Time Only
	glBegin(GL_QUADS); // Draw A Quad
	glVertex3f(0.0f, 0.0f, 0.0f); // Top Left
	glVertex3f(3.0f, 0.0f, 0.0f); // Top Right
	glVertex3f(3.0f, 0.0f, 3.0f); // Bottom Right
	glVertex3f(0.0f, 0.0f, 3.0f); // Bottom Left
	glEnd();

	//在窗口上显示“X”表示X轴
	glTranslatef(4.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();

	//在窗口上显示“Y”表示Y轴
	glTranslatef(-5.0f, 4.0f, 0.0f);
	glColor3f(0.0f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glEnd();

	//在窗口上显示“Z”表示Z轴
	glTranslatef(0.0f, -4.0f, 4.0f);
	glColor3f(0.5f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();
	
	glFlush();
}

void reshape(int x, int y)
{
	int dis = x > y ? y : x; //取小的
	glViewport(0, 0, dis, dis);
}

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("opengl");
	glewInit();
	init();
	glutDisplayFunc(draw);
	glutMainLoop();
	return 0;
}