#ifndef EXCEPTIONHACK_H
#define EXCEPTIONHACK_H

#include <iostream>
#include <string>
using namespace std;

class ExceptionHack {
public:
	static void raise_Exception(const char * message);
	static void raise_Exception(string message);
};

#endif