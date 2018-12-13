#include <Windows.h>
#include <iostream>

#include "cA.h"
#include "cB.h"

using namespace std;

CA::CA() {

	a1 = 0;
	a2 = 0;
}

CA::~CA() {

}

int CA::addA() {
	

	CB *myCB = new CB;
	myCB->b1 = a1;


//	myCB->addB;
	//cout << "the value of b1 in class CA is :" << myCB::b1 << endl;

	//a1 = myCB::b1;
	cout << "the value of a1 in class CA is :  " << a1 << endl;

	//a2 = myCB.addB;
	cout << "the value of a2 in class CA is :  " << a2 << endl;

	return a1;
}

