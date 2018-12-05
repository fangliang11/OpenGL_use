/*
*本程序实现3D可旋转缩放坐标系
*/

#include<stdio.h>
#include <Windows.h>

#include<GL/freeglut.h>

#include "coordinate3D.h"
#include "Control.h"

using namespace std;

GLfloat WINDOW_WIDE = 800;
GLfloat WINDOW_HEIGHT = 800;



//程序初始化
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glShadeModel(GL_SMOOTH);

	Control Ctl;
	Ctl.WIDE = WINDOW_WIDE;
	Ctl.HEIGHT = WINDOW_HEIGHT;

	//glEnable(GL_DEPTH_TEST);
}

//绘图 glutDisplayFunc
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); //白色画笔

	Ctl.scaleSizeX = Ctl.times; Ctl.scaleSizeY = Ctl.times; Ctl.scaleSizeZ = Ctl.times;

	//三维坐标轴
	GLAxis();

	//三维网格线
	glPushMatrix();
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	coordpoint cpoint1 = { 0,0,0 };
	coordpoint cpoint2 = { AXES_LEN,0,AXES_LEN };
	coordpoint cpoint3 = { 0,0,0 };
	coordpoint cpoint4 = { AXES_LEN,0,AXES_LEN };
	coordpoint cpoint5 = { 0,0,0 };
	coordpoint cpoint6 = { AXES_LEN,0,AXES_LEN };
	GLGrid(cpoint1, cpoint2, cpoint3, cpoint4, cpoint5, cpoint6, 20);
	glPopMatrix();

	//画圆锥体
	GLUquadricObj *objCylinder = gluNewQuadric(); //创建二次曲面对象——-圆柱
	glPushMatrix();
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glColor3f(0.9f, 0.9f, 0.0f);
	glTranslatef(1.0, 1.0, 1.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	gluCylinder(objCylinder, 0.5, 0.2, 3, 10, 5);
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	WINDOW_WIDE = w;
	WINDOW_HEIGHT = h;

	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);//改变显示区域，起始位置为客户端窗口左下角（非坐标原点）

	//设置投影视图
	glMatrixMode(GL_PROJECTION);//修改投影矩阵
	glLoadIdentity();//导入单位阵
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);//设置投影矩阵，改变视角/宽高比/近面/远面

	//设置模型视图
	glMatrixMode(GL_MODELVIEW);//修改模型视图矩阵
	glLoadIdentity();
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//设置观察者位置，相机坐标/相机角度/相机旋转

}

int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDE, WINDOW_HEIGHT);
	glutCreateWindow("三维坐标系");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	Control mouseCB;
	Control mouseMotionCB;

	//Control *myControl = new Control;
	//myControl->mouseCB();
	//myControl->mouseMotionCB();

	glutMouseFunc(&mouseCB);
	glutMotionFunc(&mouseMotionCB);
	glutMainLoop();

	return 0;
}