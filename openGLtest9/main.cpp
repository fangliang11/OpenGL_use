/*
*本程序实现3D可旋转缩放坐标系
*/

#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <GL/freeglut.h>

using namespace std;

GLint WINDOW_WIDE = 800;
GLint WINDOW_HEIGHT = 800;
#define AXES_LEN 5


GLfloat mouseRotateX = 0, mouseRotateY = 0;
GLfloat mouseMoveX = 0, mouseMoveY = 0;
GLfloat cameraDistanceX = 0, cameraDistanceY = 0;
GLfloat cameraAngleX = 0, cameraAngleY = 0;
GLfloat times = 1, times_n = 1;
GLfloat scaleSizeX, scaleSizeY, scaleSizeZ;


//程序初始化
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);


}

vector<float> coordinateX;
vector<float> coordinateY;
vector<float> coordinateZ;
int ROWNUM;

int readData() {

	ifstream myfile("D:\\MYdata1126.dat");
	if (!myfile.is_open()) {
		cout << "Unable to open myfile";
		system("pause");
		exit(1);
	}

	vector<string> vec;
	vector<float> vectorX;
	vector<float> vectorY;
	vector<float> vectorZ;

	string temp;
	while (getline(myfile, temp))                    //利用getline（）读取每一行，并按照行为单位放入到vector
	{
		vec.push_back(temp);
	}
	myfile.close();
	ROWNUM = vec.size();
	cout << " the num of row is: " << ROWNUM << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		//cout << *it << endl;
		istringstream is(*it);                    //用每一行的数据初始化一个字符串输入流；
		string s;
		int pam = 1;                              //从第一列开始

		while (is >> s)                          //以空格为界，把istringstream中数据取出放入到依次s中
		{
			if (pam == 3)                       //获取第 P 列的数据
			{
				float r = atof(s.c_str());     //做数据类型转换，将string类型转换成float
				vectorX.push_back(r);
			}
			if (pam == 4) {
				float y = atof(s.c_str());
				vectorY.push_back(y);
			}
			if (pam == 5) {
				float z = atof(s.c_str());
				vectorZ.push_back(z);
			}

			pam++;

		}
	}

	float maxX, maxY, maxZ, minX, minY, minZ;
	maxX = *max_element(begin(vectorX), end(vectorX));
	minX = *min_element(begin(vectorX), end(vectorX));

	maxY = *max_element(begin(vectorY), end(vectorY));
	minY = *min_element(begin(vectorY), end(vectorY));

	maxZ = *max_element(begin(vectorZ), end(vectorZ));
	minZ = *min_element(begin(vectorZ), end(vectorZ));

	for (int i = 0; i < ROWNUM; i++) {

		float coeffX = 1.5*AXES_LEN / (maxX - minX); //求坐标系下的坐标数值
		coordinateX.push_back(coeffX * vectorX[i]);

		float coeffY = 1.5*AXES_LEN / (maxY - minY);
		coordinateY.push_back(coeffY * vectorY[i]);

		float coeffZ = 2 / (maxZ - minZ);  ////深度的变化范围对应坐标系Z轴2个单位的长度
		coordinateZ.push_back(coeffZ * (vectorZ[i] - minZ));

	}

	cout << "MaxX and MinX is " << maxX << "    " << minX << endl;
	cout << "MaxY and MinY is " << maxY << "    " << minY << endl;
	cout << "MaxZ and MinZ is " << maxZ << "    " << minZ << endl;

	return 0;
}

//**********************************************
//****************三维坐标系********************
//**********************************************
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
	glColor4f(0.9f, 0.9f, 0.9f, 0.4f); //白色网格
	glTranslatef(0, 0, -5);
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
	glColor4f(0.0f, 0.9f, 0.9f, 0.4f); //紫色网格
	glTranslatef(0, 0, -5);
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
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor4f(0.0f, 0.9f, 0.0f, 0.4f); //绿色网格
	glTranslatef(0, 0, -5);
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

//绘制坐标轴
void GLAxis() {

	GLUquadricObj *objCylinder = gluNewQuadric(); //创建二次曲面对象——-圆柱

	//画原点
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	//glutSolidSphere(0.08, 6, 6);//黄色原点（网格原点）

	glColor3f(1.0f, 0.0f, 1.0f);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	glTranslatef(0, 0, 10);
	//glutSolidSphere(0.2, 6, 6);//粉红色原点(相机原点）

	glColor3f(1.0f, 1.0f, 0.3f);
	glTranslatef(5, 5, -5);
	glutSolidSphere(0.05, 6, 6);//位置标记点1
	glTranslatef(-10, 0, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点2
	glTranslatef(0, -10, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点3
	glTranslatef(10, 0, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点4
	glTranslatef(0, 0, -10);
	glutSolidSphere(0.05, 6, 6);//位置标记点5
	glTranslatef(0, 10, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点6
	glTranslatef(-10, 0, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点7
	glTranslatef(0, -10, 0);
	glutSolidSphere(0.05, 6, 6);//位置标记点8

	glRasterPos3f(0.02f, 0.05f, 0.05f);//字符位置
	drawCNString("O ");
	glPopMatrix();

	//画坐标轴
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	glTranslatef(-5, -5, -5);
	gluCylinder(objCylinder, 0.03, 0.03, 2.1*AXES_LEN, 10, 5);//Z_轴_蓝色
	glTranslatef(0, 0, 2.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//Z_箭头
	glRasterPos3f(-0.3, 0.3, 0);
	drawCNString("Z ");// Print GL Text ToThe Screen
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	glTranslatef(-5, -5, -5);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(objCylinder, 0.03, 0.03, 2.1*AXES_LEN, 10, 5);//Y_轴_红色
	glTranslatef(0, 0, 2.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//Y
	glRasterPos3f(-0.3, 0, 0.3);
	drawCNString("Y ");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	glTranslatef(-5, -5, -5);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(objCylinder, 0.03, 0.03, 2.1*AXES_LEN, 10, 5);//X_轴_绿色
	glTranslatef(0, 0, 2.1*AXES_LEN);
	gluCylinder(objCylinder, 0.08, 0.0, 0.1*AXES_LEN, 10, 5);//X
	glRasterPos3f(0, 0.3, 0);
	drawCNString("X ");// Print GL Text ToThe Screen
	glPopMatrix();


}

//**********************************************
//****************openGL配置********************
//**********************************************
//绘图 glutDisplayFunc
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //白色画笔

	scaleSizeX = times; scaleSizeY = times; scaleSizeZ = times;

	//三维坐标轴
	GLAxis();

	//三维网格线
	glPushMatrix();
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	coordpoint cpoint1 = { AXES_LEN, AXES_LEN, 0 };
	coordpoint cpoint2 = { -AXES_LEN, -AXES_LEN, 0 };
	coordpoint cpoint3 = { AXES_LEN, AXES_LEN, 0 };
	coordpoint cpoint4 = { -AXES_LEN, -AXES_LEN, 0 };
	coordpoint cpoint5 = { AXES_LEN, AXES_LEN, 0 };
	coordpoint cpoint6 = { -AXES_LEN, -AXES_LEN, 0 };
	GLGrid(cpoint1, cpoint2, cpoint3, cpoint4, cpoint5, cpoint6, 20);
	glPopMatrix();


	////画矩形
	//glPushMatrix();
	//glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	//glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	//glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	//glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	//glRectf(-2.0f, -2.0f, 2.0f, 2.0f);
	//glPopMatrix();


	//画点云
	GLfloat pointSize = 5.0f;
	glPointSize(pointSize);

	glPushMatrix();
	glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
	glRotatef(cameraAngleY, 0.0, 1.0, 0.0);
	glScalef(scaleSizeX, scaleSizeY, scaleSizeZ);
	glBegin(GL_POINTS);
	for (int i = 0; i < ROWNUM; i++) {

		glColor4f(0.0f, 0.0f, 0.5*coordinateZ[i] + 0.1, 1.0f);
		glVertex3f(coordinateX[i], coordinateY[i], 1 - coordinateZ[i]);

	}
	glEnd();
	glPopMatrix();

	cout << "视角变化，重绘图形" << endl;
	glutSwapBuffers();
}

//定义鼠标行为 glutMouseFunc
void mouseCB(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //鼠标左键按下
	{
		mouseRotateX = x;//当左键按下时记录鼠标坐标
		mouseRotateY = y;
	}
	//else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) //鼠标右键按下
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

//定义鼠标动作 glutMotionFunc
void mouseMotionCB(int x, int y)
{
	GLint deltax = mouseRotateX - x;
	GLint deltay = mouseRotateY - y;
	cameraAngleY += 360 * (GLfloat)deltax / (GLfloat)WINDOW_WIDE;//根据屏幕上鼠标滑动的距离来设置旋转的角度
	cameraAngleX += 360 * (GLfloat)deltay / (GLfloat)WINDOW_HEIGHT;

	GLint movex = mouseRotateX - x;
	GLint movey = mouseRotateY - y;
	cameraDistanceY += 0.001f*(GLfloat)movex / (GLfloat)WINDOW_WIDE;//根据屏幕上鼠标滑动的距离来设置移动的距离
	cameraDistanceX += 0.001f*(GLfloat)movey / (GLfloat)WINDOW_HEIGHT;

	mouseRotateX = x;//记录此时的鼠标坐标，更新鼠标坐标
	mouseRotateY = y;//若是没有这两句语句，滑动是旋转会变得不可控

	glutPostRedisplay();
}

//窗口大小改变重新绘制 glutReshapeFunc
void reshape(int w, int h)
{
	WINDOW_WIDE = w;
	WINDOW_HEIGHT = h;

	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);//改变显示区域，起始位置为客户端窗口左下角（非坐标原点）

	//设置投影视图
	glMatrixMode(GL_PROJECTION);//修改投影矩阵
	glLoadIdentity();//导入单位阵
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 50.0);//设置投影矩阵，改变视角/宽高比/近面/远面

	//设置模型视图
	glMatrixMode(GL_MODELVIEW);//修改模型视图矩阵
	glLoadIdentity();
	gluLookAt(15.0, 15.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//设置观察者位置，相机坐标/相机角度/相机旋转

}

int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDE, WINDOW_HEIGHT);
	glutCreateWindow("三维坐标系");

	init();
	//readFile();
	readData();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();

	return 0;
}