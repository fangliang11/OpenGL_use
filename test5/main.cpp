#include <Windows.h>
#include <iostream>

#include "main.h"
#include "support.h"
using namespace std;




int main() {

	Support Sup;
	Sup.write_extern();
	Sup.m = 49;
	std::cout << "m in main is : " << Sup.m << std::endl;



	std::cout << "arry is : " << Sup.n3[0] << std::endl;





	system("pause");
	return 0;
}