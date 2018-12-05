
#include <Windows.h>
#include <GL/freeglut.h>
#include "Control.h"

//���������Ϊ glutMouseFunc
void Control::mouseCB(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //����������
	{
		mouseRotateX = x;//���������ʱ��¼�������
		mouseRotateY = y;
	}
	//else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) //����Ҽ�����
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

//������궯�� glutMotionFunc
void Control::mouseMotionCB(int x, int y)
{
	GLint deltax = mouseRotateX - x;
	GLint deltay = mouseRotateY - y;
	cameraAngleY += 360 * (GLfloat)deltax / (GLfloat)WIDE;//������Ļ����껬���ľ�����������ת�ĽǶ�
	cameraAngleX += 360 * (GLfloat)deltay / (GLfloat)HEIGHT;

	GLint movex = mouseRotateX - x;
	GLint movey = mouseRotateY - y;
	cameraDistanceY += 0.001f*(GLfloat)movex / (GLfloat)WIDE;//������Ļ����껬���ľ����������ƶ��ľ���
	cameraDistanceX += 0.001f*(GLfloat)movey / (GLfloat)HEIGHT;

	mouseRotateX = x;//��¼��ʱ��������꣬�����������
	mouseRotateY = y;//����û����������䣬��������ת���ò��ɿ�

	glutPostRedisplay();
}
