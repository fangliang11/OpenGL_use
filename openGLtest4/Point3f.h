#pragma once
#pragma once
#include "freeglut.h"


//-----------------------------����ṹ��----------------------------------

struct Vertex //����ṹ
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
���ߣ�tingzhushaohua
��Դ��CSDN
ԭ�ģ�https ://blog.csdn.net/tingzhushaohua/article/details/70170891 
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�