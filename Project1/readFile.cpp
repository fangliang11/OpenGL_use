// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

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

	vector <float> radius;


	cout << "读入的数据为 " << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
		istringstream is(*it);                    //用每一行的数据初始化一个字符串输入流；
		string s;
		int pam = 0;

		while (is >> s)                          //以空格为界，把istringstream中数据取出放入到依次s中
		{
			if (pam == 1)                       //获取第六列的数据

			{
				float r = atof(s.c_str());     //做数据类型转换，将string类型转换成float
				radius.push_back(r);
				//cout << r << endl;
			}

			pam++;

		}
	}
	cout << "读入的第6列数据为 " << endl;

	for (auto it = radius.begin(); it != radius.end(); it++)
	{
		cout << *it << endl;
	}

	//cout << "successfully to open myfile";
	system("pause");
	return 1;
}
