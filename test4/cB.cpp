#include <Windows.h>
#include <iostream>

#include "cB.h"

using namespace std;

CB::CB() {
	b1 = 3;
	b2 = 3;
}

CB::~CB() {

}

int CB::addB() {

	b2 = b1 + 2;
	cout << "the value of b1 in class CB is :  " << b1 << endl;
	cout << "the value of b2 in class CB is :  " << b2 << endl;



	return b2;
}