#pragma once

#include <string>

std::string toLower(std::string str);
char unicodeToChar(const std::string& unicode);
std::string getFileExtension(const char* filePath);
bool ErrorIniOrCfgFile(char* filePath1, char* filePath2, char** ccsIni, char** ccsCfg);
