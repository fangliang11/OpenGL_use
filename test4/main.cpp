#include <Windows.h>
#include <iostream>

#include "cA.h"
#include "cB.h"

using namespace std;

int main() {

	CA CA;

	int result = CA.addA();
	cout << " result is : " << result << endl;

	system("pause");
	return 0;
}