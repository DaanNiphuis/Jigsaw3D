#include "exception_hack.h"
#include <iostream>
#include <string>
using namespace std;

void raise_Exception(const char * message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}

void raise_Exception(const string &message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}
