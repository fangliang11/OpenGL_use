
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "readFile.h"
using namespace std;

int readFiles() {

	ifstream infile;
	infile.open("D:/position4.dat", ifstream::in);  //文件位置和名称

	for (int i = 0; i < row_MAX; i++) {
		for (int j = 0; j < 13; j++) {
			infile >> arry[i][j];
		}
	}

	infile.close();

	for (int p = 0; p < row_MAX; p++) {
		arryX[p] = arry[p][2];     // 定义取哪几列
		arryY[p] = arry[p][3];
		arryZ[p] = arry[p][4];

		//maxX = *max_element(arryX, arryX + 10955);
		//minX = *min_element(arryX, arryX + 10955);
		//maxY = *max_element(arryY, arryY + 10955);
		//minY = *min_element(arryY, arryY + 10955);
		//maxZ = *max_element(arryZ, arryZ + 10955);
		//minZ = *min_element(arryZ, arryZ + 10955);
	}
	cvalueX = 1.5* AXES_LEN / (maxX - minX);
	cvalueY = 1.5* AXES_LEN / (maxY - minY);
	cvalueZ = 2 / (maxZ - minZ); //深度的变化范围对应坐标系Z轴2个单位的长度。

	cout << "maxX is " << maxX << "  ,  " << minX << endl;
	cout << "maxY is " << maxY << "  ,  " << minY << endl;
	cout << "maxZ is " << maxZ << "  ,  " << minZ << endl;

	cout << "cvalueX is " << cvalueX << endl;
	cout << "cvalueY is " << cvalueY << endl;
	cout << "cvalueZ is " << cvalueZ << endl;

	return 0;

}

