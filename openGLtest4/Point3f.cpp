#pragma once
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

#include"Point3f.h"

Point3f::Point3f() :x(0), y(0), z(0)
{

}

Point3f::Point3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{

}

int Point3f::SetParam(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	return 0;
}

GLfloat Point3f::GetParamX()
{
	return x;
}
GLfloat Point3f::GetParamY()
{
	return y;
}
GLfloat Point3f::GetParamZ()
{
	return z;
}
