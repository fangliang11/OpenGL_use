// ConsoleApplication2.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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


	while (getline(myfile, temp))                    //����getline������ȡÿһ�У���������Ϊ��λ���뵽vector
	{
		vec.push_back(temp);
	}

	vector <float> radius;


	cout << "���������Ϊ " << endl;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
		istringstream is(*it);                    //��ÿһ�е����ݳ�ʼ��һ���ַ�����������
		string s;
		int pam = 0;

		while (is >> s)                          //�Կո�Ϊ�磬��istringstream������ȡ�����뵽����s��
		{
			if (pam == 1)                       //��ȡ�����е�����

			{
				float r = atof(s.c_str());     //����������ת������string����ת����float
				radius.push_back(r);
				//cout << r << endl;
			}

			pam++;

		}
	}
	cout << "����ĵ�6������Ϊ " << endl;

	for (auto it = radius.begin(); it != radius.end(); it++)
	{
		cout << *it << endl;
	}

	//cout << "successfully to open myfile";
	system("pause");
	return 1;
}
