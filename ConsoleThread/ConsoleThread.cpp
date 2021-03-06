// ConsoleThread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <string>

////向线程函数传递参数
//void thread_function(std::string &aa, std::string &bb)
//{
//	std::cout << "thread function: ";
//	std::cout << "message is = " << aa << " | " << bb << std::endl;
//}
//
//int main()
//{
//	std::string s1 = "Kathy Perry";
//	std::string s2 = "Fang Liang";
//	std::thread t(&thread_function, std::ref(s1), std::ref(s2));
//	std::cout << " //main thread message = " << s1 << s2 << std::endl;
//	t.join();
//	return 0;
//}


using namespace std;

bool i = 0;
int j = 5;
int k = 3;

void func() {
	std::cout << "a new thread is call!" << std::endl;

	while (!i) {
		std::cout << "the new thread is run looping " << std::endl; //需向线程传入参数 i

		if (j < k ){
			std::cout << "the new thread receive j, the value of j is  " << j << std::endl; //需向线程传入参数 i
			break;
		}
		else std::cout << "j is bigger than k" << std::endl;

	}

}

void func2() {
	std::cout << "during the new thread, a func of main thread " << std::endl;

}
int main()
{
	std::cout << "start a new thread :" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));


	std::thread t(func);
	std::cout << "during the new thread :" << std::endl;
	func2();
	//std::cin >> i ;
	std::cin >> j;

	//std::this_thread::sleep_for(std::chrono::seconds(5));

	t.join();

    std::cout << "back to main thread!\n"; 
}
//
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
