#include "utils.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>

std::string toLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	return str;
}

char unicodeToChar(const std::string& unicode)
{
	int unicodeValue = std::stoi(unicode, nullptr, 16);
	return *std::string(1, static_cast<char>(unicodeValue)).c_str();
}


std::string getFileExtension(const char* filePath)
{
	if (filePath == nullptr)
	{
		return "";
	}

	const char* dot = strrchr(filePath, '.');
	const char* backslash = strrchr(filePath, '\\');

	if (!dot || (backslash && dot < backslash))
	{
		return "";
	}

	return dot + 1;
}

bool ErrorIniOrCfgFile(char* filePath1, char* filePath2, char** ccsIni, char** ccsCfg)
{
	int ret = 0;
	if (filePath1)
	{
		if (getFileExtension(filePath1) == "ini")
		{
			*ccsIni = filePath1;
		}
		else if (getFileExtension(filePath1) == "cfg")
		{
			*ccsCfg = filePath1;
		}
		else
		{
			std::cerr << "Wrong file extension (not ini and not cfg) " << filePath1 << " \n";
			ret = 1;
		}
	}

	if (filePath2)
	{
		if (getFileExtension(filePath2) == "ini")
		{
			if (*ccsIni == nullptr)
			{
				*ccsIni = filePath2;
			}
			else
			{
				std::cerr << "Cannot use both files with extension ini " << filePath2 << " \t" << filePath2 << " \n";
				ret = 1;
			}

		}
		else if (getFileExtension(filePath2) == "cfg")
		{
			if (*ccsCfg == nullptr)
			{
				*ccsCfg = filePath2;
			}
			else
			{
				std::cerr << "Cannot use both files with extension cfg " << filePath2 << " \t" << filePath2 << " \n";
				ret = 1;
			}
		}
		else
		{
			std::cerr << "Wrong file extension (not ini and not cfg) " << filePath2 << " \n";
			ret = 1;
		}
	}

	return ret;
}