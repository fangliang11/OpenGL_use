#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;


float arry[10955][13] = { 0 };  //定义文件的行数
float arryX[10955] = { 0 };
float arryY[10955] = { 0 };
float arryZ[10955] = { 0 };


int readFile() {

	ifstream infile;
	infile.open("D:/position4.dat", ifstream::in);  //文件位置和名称


	for (int i = 0; i < 10955; i++) {
		for (int j = 0; j < 13; j++) {
			infile >> arry[i][j];
		}
	}

	infile.close();

	for (int p = 0; p < 10955; p++) {
		arryX[p] = arry[p][2];     // 定义取哪几列
		arryY[p] = arry[p][3];
		arryZ[p] = arry[p][4];
	}

	return 0;
}


int main()
{
	readFile();

	cout << arry[10954][0] << ',' << arry[10954][1] << ',' << arry[10954][2] << ',' << arry[10954][3] << ',' << arry[10954][4]<< ',';
	cout << arry[10954][5] << ',' << arry[10954][6] << ',' << arry[10954][7] << ',' << arry[10954][8] << ',' << arry[10954][9]<< ',';
	cout << arry[10954][10] << ',' << arry[10954][11] << ',' << arry[10954][12] << endl;

	cout << arryX[10954] << ',' << arryY[10954] << ',' << arryZ[10954] << endl;

	system("pause");

	return 0;
}
