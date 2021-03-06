// ReadDatauseVector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() {

	ifstream myfile("D:\\position2.dat");
	if (!myfile.is_open()) {
		cout << "Unable to open myfile";
		system("pause");
		exit(1);
	}

	vector<string> vec;
	string temp;
	while (getline(myfile, temp))                    //利用getline（）读取每一行，并按照行为单位放入到vector
	{
		vec.push_back(temp);
	}

	vector<float> vectorX;
	vector<float> vectorY;
	vector<float> vectorZ;


	cout << "读入的数据为 " << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
		istringstream is(*it);                    //用每一行的数据初始化一个字符串输入流；
		string s;
		int pam = 1;                              //从第一列开始

		while (is >> s)                          //以空格为界，把istringstream中数据取出放入到依次s中
		{
			if (pam == 1)                       //获取第 P 列的数据
			{
				float r = atof(s.c_str());     //做数据类型转换，将string类型转换成float
				vectorX.push_back(r);
			}
			if (pam == 2) {
				float y = atof(s.c_str());
				vectorY.push_back(y);
			}
			if (pam == 3) {
				float z = atof(s.c_str());
				vectorZ.push_back(z);
			}

			pam++;

		}
	}
	cout << "读入的第 1 列数据为 " << endl;
	for (auto it = vectorX.begin(); it != vectorX.end(); it++)
	{
		cout << *it << endl;
	}

	cout << "读入的第 2 列数据为 " << endl;
	for (auto it = vectorY.begin(); it != vectorY.end(); it++)
	{
		cout << *it << endl;
	}

	cout << "读入的第 3 列数据为 " << endl;
	for (auto it = vectorZ.begin(); it != vectorZ.end(); it++)
	{
		cout << *it << endl;
	}



	system("pause");
	return 1;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
