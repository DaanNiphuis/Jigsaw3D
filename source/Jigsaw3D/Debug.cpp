#include "Debug.h"

#if defined _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

void printAssertion(const std::string& p_message, const char* p_file)
{
	MessageBoxA(0,
				std::string("Assertion failed! " + p_message + "\nFile: " + p_file).c_str(),
				"Assertion failed.",
				MB_ICONERROR | MB_OK);
}

#elif defined __linux__

void printAssertion(const std::string& p_message, const char* p_file)
{
	//not implemented
}

#else

void printAssertion(const std::string& p_message, const char* p_file)
{
	PRINT("Assertion failed! " << p_message << std::endl << "File: " + p_file);
}

#endif
