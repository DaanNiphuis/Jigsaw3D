#ifndef IOVARIABLES_H
#define IOVARIABLES_H

#include <string>

class IOVariables
{
public:
	static const std::string rootDirectory;
	static const std::string saveDirectory;
private:
	IOVariables();
	
	static std::string getRootDirectory();
	static std::string getSaveDirectory();
};

#endif
