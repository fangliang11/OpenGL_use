#include <GL\glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>

#define AXES_LEN 0.8
#define MAX_CHAR 128
#pragma comment (lib, "glew32d.lib")

using namespace std;

#include "ReadTxtFIle.h"
#include "Point3f.h"
#include "GlutWin.h"

#pragma comment(lib,"freeglut.lib")

vector<Point3f> pointListt;
//double gVertexNum;
//double gEdgeNum;

int gVertexNum;
int gEdgeNum;
GlutWin *win;

using namespace std;

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cout << "���ڻ��ơ���" << endl;
	cout << pointListt.at(0).GetParamX() << endl;
	cout << pointListt.at(1).GetParamY() << endl;
	cout << pointListt.at(2).GetParamZ() << endl;

	/*glBegin(GL_TRIANGLES);
		glVertex3f(0.032027, 0.056575, -0.050364);
		glVertex3f(0.031798, 0.056575, -0.050338);
		glVertex3f(0.032377, 0.056564, -0.050338);
	glEnd();*/

	static double theta = 0;
	theta += 0.05;
	glColor3d(1, 0.5, 1);
	glRotated(theta, 0, 1, 0);

	glBegin(GL_TRIANGLES);

	//for (int i = 0; i < gVertexNum - 2; i++)
	//{
	//  glVertex3f(pointListt.at(i+0).GetParamX(), pointListt.at(i+0).GetParamY(), pointListt.at(i+0).GetParamZ());
	//  glVertex3f(pointListt.at(i+1).GetParamX(), pointListt.at(i+1).GetParamY(), pointListt.at(i+1).GetParamZ());
	//  glVertex3f(pointListt.at(i+2).GetParamX(), pointListt.at(i+2).GetParamY(), pointListt.at(i+2).GetParamZ());
	//}

	for (vector<Point3f>::iterator it = pointListt.begin(); it != pointListt.end(); it++)
	{
		glVertex3f(it->GetParamX(), it->GetParamY(), it->GetParamZ());
		/*glVertex3f( (it+1)->GetParamX(), (it+1)->GetParamY(), (it+1)->GetParamZ() ) ;
		glVertex3f( (it+2)->GetParamX(), (it+2)->GetParamY(), (it+2)->GetParamZ() );*/
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void myReshape(GLsizei width, GLsizei height)
{
	const float ar = (float)width / (GLfloat)height;
	if (height == 0)
	{
		height = 1;
	}

	//ͶӰ->�ӿڱ任
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#if 0
	glOrtho(-1.0, 1.0, -1.0 / ar, 1.0 / ar, -1.0, 1.0);
#else
	gluPerspective(45.0, ar, 0.1, 100.0);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
#endif // ͸��ͶӰ��ƽ��ͶӰ
	glScalef(10.0, 10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void myidle()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	ReadTxtFile DPoint;
	cout << "����������..." << endl;
	pointListt = DPoint.Get3DPoint();  //
	if (pointListt.empty())
	{
		cerr << "��������Ϊ��" << endl;
	}

	gVertexNum = DPoint.getVertexNum();
	//gVertexNum = DPoint.getVertexNum();
	//gEdgeNum = DPoint.getEdgeNum();

	//cout << "�������:" << gVertexNum << endl;
	//cout << "��Ƭ����:" << gEdgeNum << endl;
	cout << "����������" << DPoint.getLoadTime() << "s" << endl;

	win = new GlutWin(600, 800, 100, 100, GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH, FILENAME);

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myidle);
	glutMainLoop();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);  //�����������ֵС�ڲο�ֵ����ͨ��

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);  //������ɫ׷��  

	delete win;
	return 0;
}
