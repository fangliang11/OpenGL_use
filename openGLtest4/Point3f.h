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

//-----------------------------顶点结构体----------------------------------

struct Vertex //顶点结构
{
	Vertex() {}
	Vertex(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
};

class Point3f
{
public:
	Point3f();
	Point3f(float _x, float _y, float _z);
	int SetParam(float _x, float _y, float _z);
	GLfloat GetParamX();
	GLfloat GetParamY();
	GLfloat GetParamZ();
	inline Vertex IVertex()
	{
		return Vertex(x, y, z);
	}
private:
	float x, y, z;
};
