#pragma once
#pragma once
#include "freeglut.h"


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
-------------------- -
作者：tingzhushaohua
来源：CSDN
原文：https ://blog.csdn.net/tingzhushaohua/article/details/70170891 
版权声明：本文为博主原创文章，转载请附上博文链接！