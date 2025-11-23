#pragma once

#include <sstream>
#include <Windows.h>
#include "utils.h"

typedef struct { const char* data; } json_begin_block;
typedef struct { const char* data; } json_end_block;

#ifndef INLINE_JSON_API

json_begin_block beginobj();
json_end_block endobj();
json_begin_block beginarray();
json_end_block endarray();
std::string espaceJson(const std::string& input);

#else

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

#endif // !INLINE_JSON_API

std::string to_utf8(const std::string& input);
std::string to_codepage(const std::string& utf8Str);

class json
{
private:
	std::ostringstream str;
	mutable size_t attribute = -1;
	unsigned int codepage;

public:

	json(unsigned int cp) : codepage(cp)
	{
		str << "{ ";
	}

	json() : codepage(CP_UTF8)
	{
		str << "{ ";
	}
	std::string to_string() const
	{
		attribute = -1;
		return str.str() + " }";
	}

	friend json& operator<<(json& os, const std::string& s)
	{
		os.before();

		os.str << '\"' << (espaceJson(s)) << '\"';

		os.after();

		return os;
	}

	friend json& operator<<(json& os, json_begin_block s)
	{
		os.before();

		os.str << s.data;

		os.attribute = -1;

		return os;
	}

	friend json& operator<<(json& os, json_end_block s)
	{
		os.str << s.data;

		return os;
	}

	friend json& operator<<(json& os, const char* s)
	{
		os.before();

		os.str << '\"' << (espaceJson(s)) << '\"';

		os.after();

		return os;
	}

	friend json& operator<<(json& os, const json& j)
	{
		if (os.attribute)
		{
			os.before();

			os.str << ' ' << j.to_string();

			os.after();
		}
		else
			throw std::runtime_error("An error occurred!");

		return os;
	}

	friend json& operator<<(json& os, bool b)
	{
		if (os.attribute)
		{
			os.before();

			os.str << ' ' << (b ? "true" : "false");

			os.after();
		}
		else
			throw std::runtime_error("An error occurred!");

		return os;
	}

	friend json& operator<<(json& os, const std::ostringstream& rawJson)
	{
		if (os.attribute)
		{
			os.before();
			os.str << ' ' << rawJson.str(); // no quotes
			os.after();
		}
		else
		{
			throw std::runtime_error("An error occurred!");
		}

		return os;
	}


	template<class T>
	friend json& operator<<(json& os, const T& value)
	{
		if (os.attribute)
		{
			os.before();

			os.str << ' ' << value;

			os.after();
		}
		else
			throw std::runtime_error("An error occurred!");

		return os;
	}



private:

	void before()
	{
		if (attribute == -1)
			attribute = 0;
		else if (attribute)
			str << ": ";
		else
			str << ", ";
	}

	void after()
	{
		attribute = (++attribute) & 1;
	}



};



