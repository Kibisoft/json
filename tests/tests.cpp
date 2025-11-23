#include "pch.h"
#include "CppUnitTest.h"

#define INLINE_JSON_API
#include <json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(tests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			json j;

			j << "pays" << "France";
			j << "capital" << "Paris";
			j << "departements" << beginarray();
			j << beginobj();
			j << "nom" << "Ain";
			j << "code" << 1;
			j << "villes" << beginarray();
			j << beginobj();
			j << "nom" << "Bourg-en-Bresse";
			j << "population" << 40000;
			j << endobj();
			j << beginobj();
			j << "nom" << "Oyonnax";							
			j << "population" << 20000;
			j << endobj();
			j << endarray();
			j << endobj();
			j << beginobj();
			j << "nom" << "Aisne";
			j << "code" << 2;
			j << endobj();
			j << endarray();

			std::string s = j.to_string();
		}
	};
}
