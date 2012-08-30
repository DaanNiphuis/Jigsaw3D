#ifndef EXCEPTION_HACK_H
#define EXCEPTION_HACK_H

#include <string>

void raise_Exception(const char * message);
void raise_Exception(const std::string &message);

#endif