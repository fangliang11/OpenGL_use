#include <GL\glew.h>
#include <GL/glut.h>

#pragma comment (lib, "glew32d.lib")
void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

}
void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor4f(0.0f, 1.0f, 0.0f, 0.9f);
	glRectf(-0.6f, -0.6f, 0.6f, 0.6f);

	glColor4f(0.0f, 0.0f, 1.0f, 0.5);
	glRectf(-0.3f, -0.3f, 0.3f, 0.3f);

	glFlush();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 300);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
	glutCreateWindow("opengl");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
