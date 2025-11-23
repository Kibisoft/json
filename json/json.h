#pragma once

#include <sstream>
#include <stack>

#include <Windows.h>
#include "utils.h"

struct begin_block {};
struct end_block {};
struct begin_array {};
struct end_array {};

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

std::string to_utf8(const std::string& input);
std::string to_codepage(const std::string& utf8Str);

class json
{
	private:
		struct stack_frame
		{
			bool attribute = false;
			int attributes_count = 0;
			int objects_count = 0;
		};

private:
	std::ostringstream str;
	unsigned int codepage;

	std::stack<stack_frame> context;

public:

	json(unsigned int cp) : codepage(cp)
	{
		context.push(stack_frame());
		*this << begin_block();
	}

	json() : codepage(CP_UTF8)
	{
		context.push(stack_frame());
		*this << begin_block();
	}

	std::string to_string() const
	{
		return str.str() + " }";
	}

	friend json& operator<<(json& os, const std::string& s)
	{
		os.str << '\"' << (espaceJson(s)) << '\"';

		return os;
	}

	friend json& operator<<(json& os, begin_block s)
	{
		if (os.context.top().objects_count == 0)
		{
			os.str << '{';
		}
		else
		{
			os.str << ", {";
		}

		os.context.push(stack_frame());

		return os;
	}

	friend json& operator<<(json& os, end_block s)
	{
		os.str << '}';

		os.context.pop();
		os.context.top().objects_count++;

		return os;
	}

	friend json& operator<<(json& os, begin_array s)
	{
		if (os.context.top().attribute)
		{
			os.str << " : [";
		}
		else
		{
			os.str << '[';
		}

		os.context.push(stack_frame());

		return os;
	}

	friend json& operator<<(json& os, end_array s)
	{
		os.str << ']';

		os.context.pop();

		os.context.top().attribute = !os.context.top().attribute;
		if (!os.context.top().attribute)
		{
			os.context.top().attributes_count++;
		}

		return os;
	}

	friend json& operator<<(json& os, const char* s)
	{
		if (os.context.top().attribute)
		{
			os.str << " : ";
		}
		else if (os.context.top().attributes_count)
		{
			os.str << ", ";
		}

		os.str << '\"' << (espaceJson(s)) << '\"';

		os.context.top().attribute = !os.context.top().attribute;
		if (!os.context.top().attribute)
		{
			os.context.top().attributes_count++;
		}
		
		return os;
	}

	friend json& operator<<(json& os, const json& j)
	{
		os.str << ' ' << j.to_string();

		return os;
	}

	friend json& operator<<(json& os, bool b)
	{
		if (os.context.top().attribute)
		{
			os.str << " : ";
		}
		else if (os.context.top().attributes_count)
		{
			os.str << ", ";
		}

		os.str << ' ' << (b ? "true" : "false");

		os.context.top().attribute = !os.context.top().attribute;
		if (!os.context.top().attribute)
		{
			os.context.top().attributes_count++;
		}

		return os;
	}

	friend json& operator<<(json& os, const std::ostringstream& rawJson)
	{
		os.str << ' ' << rawJson.str(); // no quotes

		return os;
	}


	template<class T>
	friend json& operator<<(json& os, const T& value)
	{
		if (os.context.top().attribute)
		{
			os.str << " : ";
		}
		else if (os.context.top().attributes_count)
		{
			os.str << ", ";
		}

		os.str << ' ' << value; //

		os.context.top().attribute = !os.context.top().attribute;
		if (!os.context.top().attribute)
		{
			os.context.top().attributes_count++;
		}

		return os;
	}
};



