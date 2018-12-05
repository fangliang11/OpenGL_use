#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();
	glutSwapBuffers();
}

void CMyStatic::GLGrid(coordpoint& pt1, coordpoint& pt2, int num)

{

	const float _xLen = (pt2.x - pt1.x) / num;

	const float _yLen = (pt2.y - pt1.y) / num;

	const float _zLen = (pt2.z - pt1.z) / num;



	glLineWidth(1.0f);

	glLineStipple(1, 0x0303);//线条样式



	glBegin(GL_LINES);

	glEnable(GL_LINE_SMOOTH);

	//glColor3f(0.0f,0.0f, 1.0f); //白色线条



	int xi = 0;

	int yi = 0;

	int zi = 0;



	//绘制平行于X的直线

	for (zi = 0; zi <= num; zi++)

	{

		float z = _zLen * zi + pt1.z;



		for (yi = 0; yi <= num; yi++)

		{

			float y = _yLen * yi + pt1.y;



			glVertex3f(pt1.x, y, z);

			glVertex3f(pt2.x, y, z);

		}

	}

	//绘制平行于Y的直线

	for (zi = 0; zi <= num; zi++)

	{

		float z = _zLen * zi + pt1.z;



		for (xi = 0; xi <= num; xi++)

		{

			float x = _xLen * xi + pt1.x;



			glVertex3f(x, pt1.y, z);

			glVertex3f(x, pt2.y, z);

		}

	}

	//绘制平行于Z的直线

	for (yi = 0; yi <= num; yi++)

	{

		float y = _yLen * yi + pt1.y;



		for (xi = 0; xi <= num; xi++)

		{

			float x = _xLen * xi + pt1.x;



			glVertex3f(x, y, pt1.z);

			glVertex3f(x, y, pt2.z);

		}

	}





	glEnd();

}


int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(renderScene);
	glutMainLoop();//enters the GLUT event processing loop.
	return 0;
}

