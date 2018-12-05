//
//

#ifndef	 COORDINATE3D_H
#define  COORDINATE3D_H



#include <Windows.h>
#include <GL/freeglut.h>
#include "Control.h"


//绘制字符
#define MAX_CHAR 128
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC(); //获取显示设备
	GLuint list = glGenLists(1); //申请1个显示列表
	//计算字符的个数
	//如果是双字节字符的（比如中文字符），两个字节才算一个字符
	//否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != ' '; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L' ';// 只是转义符,它本身的类型是wchar_t
	// 逐个输出字符
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

//坐标结构体定义
struct coordpoint {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//绘制网格线
void GLGrid(coordpoint& pt1, coordpoint& pt2, coordpoint& pt3, coordpoint& pt4, coordpoint& pt5, coordpoint& pt6, int num) {
	const float _xLen = (pt2.x - pt1.x) / num;
	const float _yLen = (pt2.y - pt1.y) / num;
	const float _zLen = (pt2.z - pt1.z) / num;
	glLineWidth(1.0f);
	glLineStipple(1, 0x0303);//线条样式

	int xi1 = 0, xi2 = 0, xi3 = 0;
	int yi1 = 0, yi2 = 0, yi3 = 0;
	int zi1 = 0, zi2 = 0, zi3 = 0;

	/////////////////////////////////////////////////point1  point2
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f); //白色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (yi1 = 0; yi1 <= num; yi1++) {
			float y = _yLen * yi1 + pt1.y;
			glVertex3f(pt1.x, y, z);
			glVertex3f(pt2.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi1 = 0; zi1 <= num; zi1++) {
		float z = _zLen * zi1 + pt1.z;
		for (xi1 = 0; xi1 <= num; xi1++) {
			float x = _xLen * xi1 + pt1.x;
			glVertex3f(x, pt1.y, z);
			glVertex3f(x, pt2.y, z);
		}
	}
	//绘制平行于Z的直线
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
	glColor3f(0.0f, 0.9f, 0.9f); //紫色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (yi2 = 0; yi2 <= num; yi2++) {
			float y = _yLen * yi2 + pt3.y;
			glVertex3f(pt3.x, y, z);
			glVertex3f(pt4.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi2 = 0; zi2 <= num; zi2++) {
		float z = _zLen * zi2 + pt3.z;
		for (xi2 = 0; xi2 <= num; xi2++) {
			float x = _xLen * xi2 + pt3.x;
			glVertex3f(x, pt3.y, z);
			glVertex3f(x, pt4.y, z);
		}
	}
	//绘制平行于Z的直线
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
	glColor3f(0.0f, 0.9f, 0.0f); //绿色网格
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH); //抗锯齿
	//绘制平行于X的直线
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (yi3 = 0; yi3 <= num; yi3++) {
			float y = _yLen * yi3 + pt5.y;
			glVertex3f(pt5.x, y, z);
			glVertex3f(pt6.x, y, z);
		}
	}
	//绘制平行于Y的直线
	for (zi3 = 0; zi3 <= num; zi3++) {
		float z = _zLen * zi3 + pt5.z;
		for (xi3 = 0; xi3 <= num; xi3++) {
			float x = _xLen * xi3 + pt5.x;
			glVertex3f(x, pt5.y, z);
			glVertex3f(x, pt6.y, z);
		}
	}
	//绘制平行于Z的直线
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

Control Ctl;

//绘制坐标轴
#define AXES_LEN 5
void GLAxis() {


	GLUquadricObj *objCylinder = gluNewQuadric(); //创建二次曲面对象——-圆柱

	//画原点
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glutSolidSphere(0.08, 6, 6);//黄色原点
	glRasterPos3f(0.05f, 0.05f, 0.05f);
	drawCNString("O ");
	glPopMatrix();

	//画坐标轴
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	gluCylinder(objCylinder, 0.03, 0.03, 1.1*AXES_LEN, 10, 5);//Z_轴_蓝色
	glTranslatef(0, 0, 1.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//Z_箭头
	glPopMatrix();

	glPushMatrix();
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glRasterPos3f(0, 0.2, 1.1*AXES_LEN);
	drawCNString("Z ");// Print GL Text ToThe Screen
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(objCylinder, 0.03, 0.03, 1.1*AXES_LEN, 10, 5);//Y_轴_红色
	glTranslatef(0, 0, 1.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//Y
	glPopMatrix();

	glPushMatrix();
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glRasterPos3f(0.2, 1.1*AXES_LEN, 0);
	drawCNString("Y ");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(objCylinder, 0.03, 0.03, 1.1*AXES_LEN, 10, 5);//X_轴_绿色
	glTranslatef(0, 0, 1.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//X
	glPopMatrix();

	glPushMatrix();
	glRotatef(Ctl.cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(Ctl.cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(Ctl.scaleSizeX, Ctl.scaleSizeY, Ctl.scaleSizeZ);
	glRasterPos3f(1.1*AXES_LEN, 0.2, 0);
	drawCNString("X ");// Print GL Text ToThe Screen
	glPopMatrix();


}


#endif