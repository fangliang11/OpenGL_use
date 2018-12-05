#include <Windows.h>
#include <iostream>
#include "support.h"

using namespace std;

void Support::write_extern(void)
{
	m = 23;
	std::cout << "m in support is : " << m << std::endl;
}