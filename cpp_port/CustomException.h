#ifndef CUSTOMEXCEPTION_H
#define CUSTOMEXCEPTION_H

#include "common.h"
using namespace std;

class CustomException : public exception {
	
public:
	CustomException(const char * message);
	~CustomException(void);
};

#endif