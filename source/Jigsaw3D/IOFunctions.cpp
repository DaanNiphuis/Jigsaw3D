#include "IOFunctions.h"

#include "Debug.h"
#include "IOVariables.h"

#include <fstream>
#include <stdio.h>

bool getFileAsString(std::string& p_string, const char* p_filename)
{
	std::ifstream filestream((IOVariables::rootDirectory + p_filename).c_str(), std::ios::binary);
	
	ASSERT(!filestream.fail(), "Failed to open " << p_filename);

	// Get file length.
	filestream.seekg (0, std::ios::end);
	unsigned int length = static_cast<unsigned int>(filestream.tellg());
	filestream.seekg (0, std::ios::beg);

	char* data = new char[length];
	filestream.read(data, length);
	
	p_string = std::string(data, length);
	delete[] data;

	return true;
}

bool getFileLines(Lines& p_lines, const char* p_filename)
{
	std::ifstream filestream((IOVariables::rootDirectory + p_filename).c_str(), std::ios::binary);
	
	ASSERT(!filestream.fail(), "Failed to open: " << p_filename);

	// Get file length.
	filestream.seekg (0, std::ios::end);
	unsigned int length = static_cast<unsigned int>(filestream.tellg());
	filestream.seekg (0, std::ios::beg);

	char* data = new char[length];
	filestream.read(data, length);

	unsigned int start = 0;
	bool comment = false;
	unsigned int i = 0;
	for (; i < length; ++i)
	{
		char letter = data[i];
		if (i == start && letter == '#')
		{
			comment = true;
		}
		if (letter == '\n' || letter == '\r')
		{
			if (comment == false && i != start)
			{
				ASSERT((start + (i - start) <= length), "Index out of range.");
				p_lines.push_back(std::string(data, start, i - start));
			}
			comment = false;
			start = i + 1;
		}
	}
	if (start < length && comment == false)
	{
		ASSERT(length == (start + (i - start)), "Index out of range.");
		std::string line = std::string(data, start, i - start);
		p_lines.push_back(line);
	}
	
	delete[] data;

	return true;
}

void getSaveData(std::string& p_content, const char* p_filename)
{
	// Check if the file exists.
	std::ifstream filestream((IOVariables::saveDirectory + p_filename).c_str());
	
	ASSERT(!filestream.fail(), "Failed to open save file: " << p_filename);
	
	std::string line;
	while (std::getline(filestream, line))
	{
		p_content += line;
	}
}

void writeToFile(const std::string& p_content, const char* p_filename)
{
	// Open the file and save the content.
	std::ofstream outfile((IOVariables::rootDirectory + p_filename).c_str());
	
	ASSERT(!outfile.fail(), "Failed to create file: " << p_filename);

	outfile << p_content;
}

void writeToFileBinary(const std::string& p_content, const char* p_filename)
{
	// Open the file and save the content.
	std::ofstream outfile((IOVariables::rootDirectory + p_filename).c_str(), std::ios_base::binary);
	
	ASSERT(!outfile.fail(), "Failed to create file: " << p_filename);

	outfile << p_content;
}

void writeToSaveFile(const std::string& p_content, const char* p_filename)
{
	// Open the file and save the content.
	std::ofstream outfile((IOVariables::saveDirectory + p_filename).c_str());
	
	ASSERT(!outfile.fail(), "Failed to create file: " << p_filename);

	outfile << p_content;
}

bool fileExists(const char* p_filename)
{
	// Check if the file exists.
	std::ifstream file((IOVariables::rootDirectory + p_filename).c_str());

	return !file.fail();
}

bool saveDataExists(const char* p_filename)
{
	// Check if the file exists.
	std::ifstream file((IOVariables::saveDirectory + p_filename).c_str());

	return !file.fail();
}

bool createFileIfNotExisting(const std::string& p_content, const char* p_filename)
{
	std::string filename = IOVariables::rootDirectory + p_filename;
	// Check if the file exists.
	std::ifstream file(filename.c_str());

	if (!file.fail())
	{
		return false;
	}
	
	// Open the file and save the content.
	std::ofstream outfile(filename.c_str());
	
	ASSERT(!outfile.fail(), "Failed to create file: " << p_filename);

	outfile << p_content;

	return true;
}

bool createSaveFileIfNotExisting(const std::string& p_content, const char* p_filename)
{
	std::string filename = IOVariables::saveDirectory + p_filename;
	// Check if the file exists.
	std::ifstream file(filename.c_str());

	if (!file.fail())
	{
		return false;
	}
	
	// Open the file and save the content.
	std::ofstream outfile(filename.c_str());
	
	ASSERT(!outfile.fail(), "Failed to create file: " << p_filename);

	outfile << p_content;

	return true;
}

void deleteFile(const char* p_filename)
{
	remove((IOVariables::rootDirectory + p_filename).c_str());
}

void deleteSaveFile(const char* p_filename)
{
	remove((IOVariables::saveDirectory + p_filename).c_str());
}
