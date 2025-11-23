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
			j << "departements" << begin_array();
			j << begin_block();
			j << "nom" << "Ain";
			j << "code" << 1;
			j << end_block();
			j << begin_block();
			j << "nom" << "Aisne";
			j << "code" << 2;
			j << end_block();
			j << begin_block();
			j << "nom" << "Allier";
			j << "code" << 3;
			j << end_block();
			j << end_array();

			std::string s = j.to_string();
		};

		TEST_METHOD(TestMethod2)
		{
			json j;

			j << "pays" << "France";
			j << "capital" << "Paris";
			j << "departements" << begin_array();
			j << begin_block();
			j << "nom" << "Ain";
			j << "code" << 1;
			j << "villes" << begin_array();
			j << begin_block();
			j << "nom" << "Bourg-en-Bresse";
			j << "population" << 40000;
			j << end_block();
			j << begin_block();
			j << "nom" << "Oyonnax";
			j << "population" << 20000;
			j << end_block();
			j << end_array();
			j << end_block();
			j << begin_block();
			j << "nom" << "Aisne";
			j << "code" << 2;
			j << end_block();
			j << end_array();

			std::string s = j.to_string();
		};

		TEST_METHOD(TestMethod3)
		{
			json j;

			j << "pays" << "France";
			j << "capital" << "Paris";
			j << "departements" << begin_array();
			j << end_array();
			j << "cantons" << begin_array();
			j << end_array();

			std::string s = j.to_string();
		};
	};
}
