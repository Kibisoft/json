#include "json.h"

#include <Windows.h>

json_begin_block beginobj()
{
	return json_begin_block{ "{ " };
}

json_end_block endobj()
{
	return json_end_block{ "}" };
}

json_begin_block beginarray()
{
	return json_begin_block{ "[" };
}

json_end_block endarray()
{
	return json_end_block{ "]" };
}

std::string to_utf8(const std::string& input)
{
	// CPXXXX -> UTF-16 (wide string)
	int wideLen = MultiByteToWideChar(1252, 0, input.c_str(), -1, nullptr, 0);
	if (wideLen == 0) return "";

	std::wstring wideStr(wideLen, L'\0');
	MultiByteToWideChar(1252, 0, input.c_str(), -1, &wideStr[0], wideLen);

	// UTF-16 -> UTF-8
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Len == 0) return "";

	std::string utf8Str(utf8Len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

	return utf8Str;
}

std::string to_codepage(const std::string& utf8Str)
{
	// UTF-8 -> UTF-16
	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	if (wideLen == 0) return "";

	std::wstring wideStr(wideLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], wideLen);

	// UTF-16 -> CP1252
	int ansiLen = WideCharToMultiByte(1252, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (ansiLen == 0) return "";

	std::string ansiStr(ansiLen, '\0');
	WideCharToMultiByte(1252, 0, wideStr.c_str(), -1, &ansiStr[0], ansiLen, nullptr, nullptr);

	return ansiStr;
}

std::string espaceJson(const std::string& input)
{
	std::string output;
	for (char c : input)
	{
		switch (c)
		{
		case '\"':
			output += "\\\"";
			break;
		case '\\':
			output += "\\\\";
			break;
		case '\b':
			output += "\\b";
			break;
		case '\f':
			output += "\\f";
			break;
		case '\n':
			output += "\\n";
			break;
		case '\r':
			output += "\\r";
			break;
		case '\t':
			output += "\\t";
			break;
		default:
			if (static_cast<unsigned char>(c) < 0x20)
			{
				// control characters must be escaped as \uXXXX
				char buf[7];
				snprintf(buf, sizeof(buf), "\\u%04x", c);
				output += buf;
			}
			else
			{
				output += c;
			}
		}
	}
	return output;
}
