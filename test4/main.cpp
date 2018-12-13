#include <Windows.h>
#include <iostream>

#include "cA.h"
#include "cB.h"

using namespace std;


int main() {

	CA myCA;
	CB myCB;

	myCB.i = 7;

	int result = myCA.addA();
	cout << " result is : " << result << endl;

	system("pause");
	return 0;
}