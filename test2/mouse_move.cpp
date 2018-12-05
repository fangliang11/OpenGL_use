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

//�����ʼ��
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glShadeModel(GL_SMOOTH);
	GLint dx, dy;
	dx = 0; dy = 0; xangle = 0; yangle = 0;

	//glEnable(GL_DEPTH_TEST);
}

//**********************************************
//****************��ά����ϵ********************
//**********************************************
//�����ַ�
#define MAX_CHAR 128
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC(); //��ȡ��ʾ�豸
	GLuint list = glGenLists(1); //����1����ʾ�б�
	//�����ַ��ĸ���
	//�����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�
	//����һ���ֽ���һ���ַ�
	len = 0;
	for (i = 0; str[i] != ' '; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// ������ַ�ת��Ϊ���ַ�
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L' ';// ֻ��ת���,�������������wchar_t
	// �������ַ�
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// ����������ʱ��Դ
	free(wstring);
	glDeleteLists(list, 1);
}

//����ṹ�嶨��
struct coordpoint {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//����������
void GLGrid(coordpoint& pt1, coordpoint& pt2, coordpoint& pt3, coordpoint& pt4, coordpoint& pt5, coordpoint& pt6, int num) {
	const float _xLen = (pt2.x - pt1.x) / num;
	const float _yLen = (pt2.y - pt1.y) / num;
	const float _zLen = (pt2.z - pt1.z) / num;
	glLineWidth(1.0f);
	glLineStipple(1, 0x0303);//������ʽ

	int xi1 = 0, xi2 = 0, xi3 = 0;
	int yi1 = 0, yi2 = 0, yi3 = 0;
	int zi1 = 0, zi2 = 0, zi3 = 0;

	/////////////////////////////////////////////////point1  point2
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f); //��ɫ����
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //�����
	//����ƽ����X��ֱ��
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (yi1 = 0; yi1 <= num; yi1++) {
			float y = _yLen * yi1 + pt1.y;
			glVertex3f(pt1.x, y, z);
			glVertex3f(pt2.x, y, z);
		}
	}
	//����ƽ����Y��ֱ��
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (xi1 = 0; xi1 <= num; xi1++) {
			float x = _xLen * xi1 + pt1.x;
			glVertex3f(x, pt1.y, z);
			glVertex3f(x, pt2.y, z);
		}
	}
	//����ƽ����Z��ֱ��
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
	glColor3f(0.0f, 0.9f, 0.9f); //��ɫ����
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //�����
	//����ƽ����X��ֱ��
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (yi2 = 0; yi2 <= num; yi2++) {
			float y = _yLen * yi2 + pt3.y;
			glVertex3f(pt3.x, y, z);
			glVertex3f(pt4.x, y, z);
		}
	}
	//����ƽ����Y��ֱ��
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (xi2 = 0; xi2 <= num; xi2++) {
			float x = _xLen * xi2 + pt3.x;
			glVertex3f(x, pt3.y, z);
			glVertex3f(x, pt4.y, z);
		}
	}
	//����ƽ����Z��ֱ��
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
	glColor3f(0.0f, 0.9f, 0.0f); //��ɫ����
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //�����
	//����ƽ����X��ֱ��
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (yi3 = 0; yi3 <= num; yi3++) {
			float y = _yLen * yi3 + pt5.y;
			glVertex3f(pt5.x, y, z);
			glVertex3f(pt6.x, y, z);
		}
	}
	//����ƽ����Y��ֱ��
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (xi3 = 0; xi3 <= num; xi3++) {
			float x = _xLen * xi3 + pt5.x;
			glVertex3f(x, pt5.y, z);
			glVertex3f(x, pt6.y, z);
		}
	}
	//����ƽ����Z��ֱ��
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

//����������
#define AXES_LEN 1
void GLAxis() {

	GLUquadricObj *objCylinder = gluNewQuadric(); //��������������󡪡�-Բ��

	//��ԭ��
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glutSolidSphere(0.02, 6, 6);//��ɫԭ��
	glRasterPos3f(0.05f, 0.05f, 0.05f);
	drawCNString("ԭ�� ");
	glPopMatrix();

	//��������
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glScalef(xsize, ysize, zsize);
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//Z_��_��ɫ
	glTranslatef(0, 0, AXES_LEN);
	gluCylinder(objCylinder, 0.03, 0.0, 0.2, 10, 5);//Z_��ͷ
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
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//Y_��_��ɫ
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
	gluCylinder(objCylinder, 0.01, 0.01, AXES_LEN, 10, 5);//X_��_��ɫ
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
//****************openGL����********************
//**********************************************
//��ͼ glutDisplayFunc
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); //��ɫ����
	//gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	xsize = times; ysize = times; zsize = times;

	//�����
	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glutSolidTeapot(2);
	glPopMatrix();

	//��ά������
	GLAxis();

	//��ά������
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

	//��Բ׶��
	GLUquadricObj *objCylinder = gluNewQuadric(); //��������������󡪡�-Բ��
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

//���������Ϊ glutMouseFunc
void mouseCB(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //����������
	{
		oldx = x;//���������ʱ��¼�������
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

//������궯�� glutMotionFunc
void mouseMotionCB(int x, int y)
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	yangle += 360 * (GLfloat)deltax / (GLfloat)WINDOW_WIDE;//������Ļ����껬���ľ�����������ת�ĽǶ�
	xangle += 360 * (GLfloat)deltay / (GLfloat)WINDOW_HEIGHT;
	oldx = x;//��¼��ʱ��������꣬�����������
	oldy = y;//����û����������䣬��������ת���ò��ɿ�


	glutPostRedisplay();
}

//���ڴ�С�ı����»��� glutReshapeFunc
void reshape(int w, int h)
{
	WINDOW_WIDE = w;
	WINDOW_HEIGHT = h;

	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);//�ı���ʾ������ʼλ��Ϊ�ͻ��˴������½ǣ�������ԭ�㣩

	//����ͶӰ��ͼ
	glMatrixMode(GL_PROJECTION);//�޸�ͶӰ����
	glLoadIdentity();//���뵥λ��
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);//����ͶӰ���󣬸ı��ӽ�/��߱�/����/Զ��

	//����ģ����ͼ
	glMatrixMode(GL_MODELVIEW);//�޸�ģ����ͼ����
	glLoadIdentity();
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//���ù۲���λ�ã��������/����Ƕ�/�����ת

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