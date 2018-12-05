#pragma once
#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Point3f.h"

#if 0
#define FILENAME "bunny.txt"
#else
#define FILENAME "dragon.txt"
#endif

#define BUFFERLINE    50  //读取一行大小
using namespace std;

class ReadTxtFile
{
public:
	ReadTxtFile();
	~ReadTxtFile();

	vector<Point3f> Get3DPoint(const char *fileName);
	vector<Point3f> Get3DPoint();
	/*double getVertexNum();
	double getEdgeNum();
	double formatConvert(char *m_line);
*/
	int getVertexNum() const;
	int getEdgeNum() const;
	time_t getLoadTime() const;

	inline void closeFile()
	{
		m_fin->clear();
		m_fin->close();
	}

private:
	string m_x, m_y, m_z;
	time_t m_t1;
	time_t m_t2;
	//const char * m_VertexNumchar;
	//const char * m_EdgeNumchar;
	//const char * m_Numchar;      //用于格式转换
	//double m_Num;
	//double m_VertexNum;            //顶点数
	//double m_EdgeNum;          //面片数
	//
	int m_vertNum;
	int m_triNum;
	ifstream *m_fin;
	char m_line[BUFFERLINE];     //读一行所用数组
	vector<Point3f> m_pointList;
	//vector<float> m_pointList;
};
