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

#include "ReadTxtFIle.h"

ReadTxtFile::ReadTxtFile() :m_x(""), m_y(""), m_z(""), m_vertNum(NULL), m_triNum(NULL)
{
	*m_line = { 0 };
	m_fin = new ifstream(FILENAME, ios::in);

}

ReadTxtFile::~ReadTxtFile()
{

}

vector<Point3f> ReadTxtFile::Get3DPoint(const char *fileName)
{
	int i = 0;

	int vertNum;
	int triNum;
	float x, y, z;

	*m_fin >> vertNum;
	*m_fin >> triNum;

	time(&m_t1);
	for (int i = 0; i < vertNum; i++)
	{
		*m_fin >> x >> y >> z;
		m_pointList.push_back(Point3f(x, y, z));

	}
	time(&m_t2);

	//读取文件
	//time(&m_t1);
	//while (m_fin->getline(m_line, sizeof(m_line)))
	//{
	//  if (i < 1)
	//  {
	//      m_VertexNum = formatConvert(m_line);
	//      i += 1;
	//      continue;
	//  }

	//  if (i == 1)
	//  {
	//      m_EdgeNum = formatConvert(m_line);
	//      i += 1;
	//      continue;
	//  }

	//  stringstream word(m_line);
	//  word >> m_x;
	//  word >> m_y;
	//  word >> m_z;

	//  const char * xChar = m_x.c_str();
	//  const char * yChar = m_y.c_str();
	//  const char * zChar = m_z.c_str();

	//  m_pointList.push_back(Point3f(atof(xChar), atof(yChar), atof(zChar)));
	//  //pointList.at(i - 2).SetParam(atof(xChar), atof(xChar), atof(xChar));

	//  i++;
	//}
	//time(&m_t2);

	closeFile();

	return m_pointList;
}

vector<Point3f> ReadTxtFile::Get3DPoint()
{
	int i = 0;

	float x, y, z;
	*m_fin >> m_vertNum;
	*m_fin >> m_triNum;

	time(&m_t1);
	for (int i = 0; i < m_vertNum; i++)
	{
		*m_fin >> x >> y >> z;
		m_pointList.push_back(Point3f(x, y, z));

	}
	time(&m_t2);

	////读取文件
	//time(&m_t1);
	//while (m_fin->getline(m_line, sizeof(m_line)))
	//{
	//  if (i < 1)
	//  {
	//      m_VertexNum = formatConvert(m_line);
	//      i += 1;
	//      continue;
	//  }

	//  if (i == 1)
	//  {
	//      m_EdgeNum = formatConvert(m_line);
	//      i += 1;
	//      continue;
	//  }

	//  stringstream word(m_line);
	//  word >> m_x;
	//  word >> m_y;
	//  word >> m_z;

	//  const char * xChar = m_x.c_str();
	//  const char * yChar = m_y.c_str();
	//  const char * zChar = m_z.c_str();

	//  m_pointList.push_back(Point3f(atof(xChar), atof(yChar), atof(zChar)));
	//  //pointList.at(i - 2).SetParam(atof(xChar), atof(xChar), atof(xChar));

	//  i++;
	//}
	//time(&m_t2);

	closeFile();

	return m_pointList;
}

//double ReadTxtFile::getVertexNum()
//{
//  return m_VertexNum;
//}
//
//double ReadTxtFile::getEdgeNum()
//{
//  return m_EdgeNum;
//}


int ReadTxtFile::getVertexNum() const
{
	return m_vertNum;
}

int ReadTxtFile::getEdgeNum() const
{
	return m_triNum;
}

time_t ReadTxtFile::getLoadTime() const
{
	return (m_t2 - m_t1);
}
