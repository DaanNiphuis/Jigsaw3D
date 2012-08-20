#ifndef IOFUNCTIONS_H
#define IOFUNCTIONS_H

#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string> Lines;
typedef std::map<std::string, std::string> Settings;

bool getFileAsString(std::string& p_string, const char* p_filename);
bool getFileLines(Lines& p_lines, const char* p_filename);
void getSaveData(std::string& p_content, const char* p_filename);

void writeToFile(const std::string& p_content, const char* p_filename);
void writeToFileBinary(const std::string& p_content, const char* p_filename);
void writeToSaveFile(const std::string& p_content, const char* p_filename);
bool fileExists(const char* p_filename);
bool saveDataExists(const char* p_filename);
// Returns true if file was created.
bool createFileIfNotExisting(const std::string& p_content, const char* p_filename);
// Returns true if file was created.
bool createSaveFileIfNotExisting(const std::string& p_content, const char* p_filename);

void deleteFile(const char* p_filename);
void deleteSaveFile(const char* p_filename);

#endif
