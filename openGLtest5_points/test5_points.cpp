#define GLUT_DISABLE_ATEXIT_HACK 
#define MAX_CHAR        128

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#pragma comment(lib,"glew32d.lib")
//glVertex3f

using namespace std;

vector<float> x_vol;
vector<float> y_vol;
float x_value[] = { 0 };
float y_value[] = { 0 };

int readFile() {
	ifstream myfile("D:\\position3.dat");
	if (!myfile.is_open()) {
		cout << "Unable to open myfile";
		system("pause");
		exit(1);
	}
	vector<string> vec;
	string temp;
	while (getline(myfile, temp))                    //����getline������ȡÿһ�У���������Ϊ��λ���뵽vector
	{
		vec.push_back(temp);
	}
	vector <float> radius;
	cout << "���������Ϊ " << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
		istringstream is(*it);                    //��ÿһ�е����ݳ�ʼ��һ���ַ�����������
		string s;
		int pam = 0, i;

		while (is >> s)                          //�Կո�Ϊ�磬��istringstream������ȡ�����뵽����s��
		{
			//for (i = 0; i < 100; i++) {
				if (pam == 1)                       //��ȡ��2�е�����
				{
					float r = atof(s.c_str());     //����������ת������string����ת����float
					x_vol.push_back(r);
					//x_value[i] = x_vol[i];
					radius.push_back(r);
					cout << r << endl;
				}
				if (pam == 2)                       //��ȡ��3�е�����

				{
					float r = atof(s.c_str());     //����������ת������string����ת����float
					y_vol.push_back(r);
					//y_value[i] = y_vol[i];
					radius.push_back(r);
					cout << r << endl;
				}
				pam++;
			//}
		}
	}
	cout << "����ĵ�6������Ϊ " << endl;

	/*for (auto it = radius.begin(); it != radius.end(); it++)
	{
		cout << *it << endl;
	}*/

	cout << "successfully to open myfile";
	system("pause");
	return 1;
}

string num2string(int i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

void drawString(string strn) {
	static int isFirstCall = 1;
	static GLuint lists;
	const char *str = strn.c_str();
	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
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

	//������ɫ
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	GLfloat lineWidth = 2.0f;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2f(10.0f, 5.0f);   //����
	glVertex2f(10.0f, 147.0f);
	glVertex2f(9.0f, 143.0f);
	glVertex2f(10.0f, 147.0f);
	glVertex2f(11.0f, 143.0f);
	glVertex2f(10.0f, 147.0f);
	glVertex2f(5.0f, 10.0f);   //����
	glVertex2f(197.0f, 10.0f);
	glVertex2f(193.0f, 9.0f);
	glVertex2f(197.0f, 10.0f);
	glVertex2f(193.0f, 11.0f);
	glVertex2f(197.0f, 10.0f);
	glEnd();

	lineWidth = 1.0f;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);  //�������ϵĶ���
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
	//readFile();
	const int n = 100;	
	int i = 0, k = 5;
	//�������n�����x�����y����
	int x_data[n], y_data[n];
	for (i = 0; i < n; i++) {
		x_data[i] = 1 + rand() % 101;  //x����ķ�Χ�ڡ�1��100��
		y_data[i] = 1 + rand() % 51;   //y����ķ�Χ�ڡ�1��50��
	}
	//������ϵ����ת�����ͼ������
	float x_real[n], y_real[n];
	for (i = 0; i < n; i++) {
		x_real[i] = x_data[i] * (200 - 2 * x0) / k / 20 + x0;
		y_real[i] = y_data[i] * (150 - 2 * y0) / k / 10 + y0;
	}
	
	GLfloat pointSize = 5.0f;
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);       //��ʾ�㣬һ����ɫһ���ɫ	
	float x_point[] = { 21, 43, 32, 65, 11 };
	float y_point[] = { 42, 22, 76, 49, 37 };
	//glVertex2f(x_point[1], y_point[1]);
	for (int i = 0; i < 5; i++) {
		glVertex2f(x_point[i], y_point[i]);
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	//for (i = n / 2; i < n; i++) {
	//	glVertex2f(x_real[i], y_real[i]);
	//}
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //������ɫ
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);   //ͶӰ���ü�����С������
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
