#include "ExceptionHack.h"

void ExceptionHack::raise_Exception(const char * message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}

void ExceptionHack::raise_Exception(string message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}
