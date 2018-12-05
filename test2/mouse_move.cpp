#include<stdio.h>
#include <Windows.h>

//GLEW
#include <GL/glew.h>
#pragma comment (lib, "glew32d.lib")

//FREEGLUT
#include<GL/freeglut.h>

using namespace std;

GLint WINDOW_WIDE = 800;
GLint WINDOW_HEIGHT = 800;

GLfloat xangle, yangle;
GLfloat oldx, oldy;
GLfloat xsize, ysize, zsize;

bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
GLfloat mouseX, mouseY;
GLfloat cameraDistanceX;
GLfloat cameraDistanceY;
GLfloat cameraAngleX;
GLfloat cameraAngleY;
GLfloat times = 1;

//程序初始化
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glShadeModel(GL_SMOOTH);
	GLint dx, dy;
	dx = 0; dy = 0; xangle = 0; yangle = 0;

	//glEnable(GL_DEPTH_TEST);
}

//**********************************************
//****************三维坐标系********************
//**********************************************
//绘制字符
#define MAX_CHAR 128
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC(); //获取显示设备
	GLuint list = glGenLists(1); //申请1个显示列表
	//计算字符的个数
	//如果是双字节字符的（比如中文字符），两个字节才算一个字符
	//否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != ' '; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L' ';// 只是转义符,它本身的类型是wchar_t
	// 逐个输出字符
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

//坐标结构体定义
struct coordpoint {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//绘制网格线
void GLGrid(coordpoint& pt1, coordpoint& pt2, coordpoint& pt3, coordpoint& pt4, coordpoint& pt5, coordpoint& pt6, int num) {
	const float _xLen = (pt2.x - pt1.x) / num;
	const float _yLen = (pt2.y - pt1.y) / num;
	const float _zLen = (pt2.z - pt1.z) / num;
	glLineWidth(1.0f);
	glLineStipple(1, 0x0303);//线条样式

	int xi1 = 0, xi2 = 0, xi3 = 0;
	int yi1 = 0, yi2 = 0, yi3 = 0;
	int zi1 = 0, zi2 = 0, zi3 = 0;

	/////////////////////////////////////////////////point1  point2
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f); //白色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (yi1 = 0; yi1 <= num; yi1++) {
			float y = _yLen * yi1 + pt1.y;
			glVertex3f(pt1.x, y, z);
			glVertex3f(pt2.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (xi1 = 0; xi1 <= num; xi1++) {
			float x = _xLen * xi1 + pt1.x;
			glVertex3f(x, pt1.y, z);
			glVertex3f(x, pt2.y, z);
		}
	}
	//绘制平行于Z的直线
	for (yi1 = 0; yi1 <= num; yi1++) {
		float y = _yLen * yi1 + pt1.y;
		for (xi1 = 0; xi1 <= num; xi1++) {
			float x = _xLen * xi1 + pt1.x;
			glVertex3f(x, y, pt1.z);
			glVertex3f(x, y, pt2.z);
		}
	}
	glEnd();
	glPopMatrix();

	//////////////////////////////////////////////point3  point4
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glColor3f(0.0f, 0.9f, 0.9f); //紫色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (yi2 = 0; yi2 <= num; yi2++) {
			float y = _yLen * yi2 + pt3.y;
			glVertex3f(pt3.x, y, z);
			glVertex3f(pt4.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (xi2 = 0; xi2 <= num; xi2++) {
			float x = _xLen * xi2 + pt3.x;
			glVertex3f(x, pt3.y, z);
			glVertex3f(x, pt4.y, z);
		}
	}
	//绘制平行于Z的直线
	for (yi2 = 0; yi2 <= num; yi2++) {
		float y = _yLen * yi2 + pt3.y;
		for (xi2 = 0; xi2 <= num; xi2++) {
			float x = _xLen * xi2 + pt3.x;
			glVertex3f(x, y, pt3.z);
			glVertex3f(x, y, pt4.z);
		}
	}
	glEnd();
	glPopMatrix();

	///////////////////////////////////////////////////////////////point5  point6
	glPushMatrix();
	glRotatef(90, 0.0, 0.0, 1.0);
	glColor3f(0.0f, 0.9f, 0.0f); //绿色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (yi3 = 0; yi3 <= num; yi3++) {
			float y = _yLen * yi3 + pt5.y;
			glVertex3f(pt5.x, y, z);
			glVertex3f(pt6.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (xi3 = 0; xi3 <= num; xi3++) {
			float x = _xLen * xi3 + pt5.x;
			glVertex3f(x, pt5.y, z);
			glVertex3f(x, pt6.y, z);
		}
	}
	//绘制平行于Z的直线
	for (yi3 = 0; yi3 <= num; yi3++) {
		float y = _yLen * yi3 + pt5.y;
		for (xi3 = 0; xi3 <= num; xi3++) {
			float x = _xLen * xi3 + pt5.x;
			glVertex3f(x, y, pt5.z);
			glVertex3f(x, y, pt6.z);
		}
	}
	glEnd();
	glPopMatrix();

}

//绘制坐标轴
#define AXES_LEN 1
void GLAxis() {

	GLUquadricObj *objCylinder = gluNewQuadric(); //创建二次曲面对象——-圆柱

	//画原点
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glutSolidSphere(0.02, 6, 6);//黄色原点
	glRasterPos3f(0.05f, 0.05f, 0.05f);
	drawCNString("原点 ");
	glPopMatrix();

	//画坐标轴
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//Z_轴_蓝色
	glTranslatef(0, 0, AXES_LEN);
	gluCylinder(objCylinder, 0.03, 0.0, 0.2, 10, 5);//Z_箭头
	glPopMatrix();

	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glRasterPos3f(0, 0.08, AXES_LEN);
	drawCNString("Z ");// Print GL Text ToThe Screen
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//Y_轴_红色
	glTranslatef(0, 0, AXES_LEN);
	gluCylinder(objCylinder, 0.03, 0.0, 0.2, 10, 5);//Y
	glPopMatrix();

	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glRasterPos3f(0.08, AXES_LEN, 0);
	drawCNString("Y ");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//X_轴_绿色
	glTranslatef(0, 0, AXES_LEN);
	gluCylinder(objCylinder, 0.03, 0.0, 0.2, 10, 5);//X
	glPopMatrix();

	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glRasterPos3f(AXES_LEN, 0.08, 0);
	drawCNString("X ");// Print GL Text ToThe Screen
	glPopMatrix();


}

//**********************************************
//****************openGL配置********************
//**********************************************
//绘图 glutDisplayFunc
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); //白色画笔
	//gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	xsize = times; ysize = times; zsize = times;

	//画茶壶
	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glutSolidTeapot(2);
	glPopMatrix();

	//三维坐标轴
	GLAxis();

	//三维网格线
	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	coordpoint cpoint1 = { 0,0,0 };
	coordpoint cpoint2 = { 1,0,1 };
	coordpoint cpoint3 = { 0,0,0 };
	coordpoint cpoint4 = { 1,0,1 };
	coordpoint cpoint5 = { 0,0,0 };
	coordpoint cpoint6 = { 1,0,1 };
	GLGrid(cpoint1, cpoint2, cpoint3, cpoint4, cpoint5, cpoint6, 20);
	glPopMatrix();

	//画圆锥体
	GLUquadricObj *objCylinder = gluNewQuadric(); //创建二次曲面对象——-圆柱
	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	glColor3f(0.9f, 0.9f, 0.0f);
	glTranslatef(0.3, 0.3, 0.3);
	glRotatef(60, 0.0, 0.0, 1.0);
	gluCylinder(objCylinder, 0.1, 0.05, 0.5, 10, 5);
	glPopMatrix();

	glutSwapBuffers();
}

//定义鼠标行为 glutMouseFunc
void mouseCB(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //鼠标左键按下
	{
		oldx = x;//当左键按下时记录鼠标坐标
		oldy = y;
	}
	else if (state == GLUT_UP && button == 3) {
		times = 0.08f + 1;
		glutPostRedisplay();
	}
	else if (state == GLUT_UP && button == 4) {
		times = -0.08f + 1;
		glutPostRedisplay();
	}
}

//定义鼠标动作 glutMotionFunc
void mouseMotionCB(int x, int y)
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	yangle += 360 * (GLfloat)deltax / (GLfloat)WINDOW_WIDE;//根据屏幕上鼠标滑动的距离来设置旋转的角度
	xangle += 360 * (GLfloat)deltay / (GLfloat)WINDOW_HEIGHT;
	oldx = x;//记录此时的鼠标坐标，更新鼠标坐标
	oldy = y;//若是没有这两句语句，滑动是旋转会变得不可控


	glutPostRedisplay();
}

//窗口大小改变重新绘制 glutReshapeFunc
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
	glutCreateWindow("Test");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();

	return 0;
}