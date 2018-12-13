#define GLUT_DISABLE_ATEXIT_HACK 
#define MAX_CHAR        128

#include <GL\glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>

#pragma comment (lib, "glew32d.lib")
//glVertex3f

using namespace std;
string num2string(int i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

void drawString(string strn) {
	static int isFirstCall = 1;
	static GLuint lists;
	const char *str = strn.c_str();
	if (isFirstCall) { // 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void createCoordinate(GLfloat x0, GLfloat y0)
{
	int i = 0, k = 5;
	GLfloat x[5], y[5];
	for (i = 0; i < k; i++) {
		x[i] = x0 + (200 - 2 * x0) / k * (i + 1);
		y[i] = y0 + (150 - 2 * y0) / k * (i + 1);
	}

	//设置颜色
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	GLfloat lineWidth = 2.0f;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2f(10.0f, 5.0f);   //纵轴
	glVertex2f(10.0f, 147.0f);
	glVertex2f(9.0f, 143.0f);
	glVertex2f(10.0f, 147.0f);
	glVertex2f(11.0f, 143.0f);
	glVertex2f(10.0f, 147.0f);
	glVertex2f(5.0f, 10.0f);   //横轴
	glVertex2f(197.0f, 10.0f);
	glVertex2f(193.0f, 9.0f);
	glVertex2f(197.0f, 10.0f);
	glVertex2f(193.0f, 11.0f);
	glVertex2f(197.0f, 10.0f);
	glEnd();

	lineWidth = 1.0f;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);  //坐标轴上的短线
	for (i = 0; i < k; i++) {
		glVertex2f(x[i], x0 + 3.0);
		glVertex2f(x[i], x0);
	}
	for (i = 0; i < k; i++) {
		glVertex2f(y0 + 3.0, y[i]);
		glVertex2f(y0, y[i]);
	}
	glEnd();

	string s;
	for (i = 0; i < k; i++) {
		s = num2string(20 * (i + 1));
		glRasterPos2f(x[i] - 2.0f, 5.0f);
		drawString(s);
		s = num2string(10 * (i + 1));
		glRasterPos2f(5.0f, y[i] - 1.0f);
		drawString(s);
	}
}

void scatter(GLfloat x0, GLfloat y0)
{
	const int n = 100;
	int i, k = 5;
	//随机生成n个点的x坐标和y坐标
	int x_data[n], y_data[n];
	for (i = 0; i < n; i++) {
		x_data[i] = 1 + rand() % 101;  //x坐标的范围在【1，100】
		y_data[i] = 1 + rand() % 51;   //y坐标的范围在【1，50】
	}
	//由坐标系坐标转成这个图的坐标
	float x_real[n], y_real[n];
	for (i = 0; i < n; i++) {
		x_real[i] = x_data[i] * (200 - 2 * x0) / k / 20 + x0;
		y_real[i] = y_data[i] * (150 - 2 * y0) / k / 10 + y0;
	}

	GLfloat pointSize = 5.0f;
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);       //显示点，一半蓝色一半红色
	for (i = 0; i < n / 2; i++) {
		glVertex2f(x_real[i], y_real[i]);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (i = n / 2; i < n; i++) {
		glVertex2f(x_real[i], y_real[i]);
	}
	glEnd();
}

void Display()
{
	GLfloat x0 = 10.0f, y0 = 10.0f;
	createCoordinate(x0, y0);
	scatter(x0, y0);

	glFlush();
}

void Initial()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //清屏颜色
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);   //投影到裁剪窗大小：世界
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Scatter Plot");
	glutDisplayFunc(Display);
	Initial();
	glutMainLoop();

	return 0;
}
