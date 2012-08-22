#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <sstream>

#if defined(_WIN32)
#define ENDASSERT {_asm{int 3}}
#elif defined(__linux__)
#define ENDASSERT {raise(SIGTRAP)}
#else
#define ENDASSERT 
#endif

#if !defined(_RELEASE)

void printAssertion(const std::string& p_message, const char* p_file);

#define PRINT(message) (std::cout << message << "\n")
#define WARNING(message) std::cout << "Warning!\nDescription: " << message << "\n"
#define PANIC(message) std::cout << "Error!\nDescription: " << message << "\n"
#define ASSERT(statement, message) if (!(statement)) {std::stringstream ss; ss << message; printAssertion((ss.str()), __FILE__); ENDASSERT}

#else

#pragma warning(disable : 4091) // Disable "ignored on left of 'void' when no variable is declared"
#define PRINT(message)
#define WARNING(message)
#define PANIC(message)
#define ASSERT(statement, message)

#endif

#endif


