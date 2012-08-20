#include "exception_hack.h"

void raise_Exception(const char * message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}

void raise_Exception(string message) {
	cerr << "Exception: " << message << endl << flush;
	throw 1;
}
