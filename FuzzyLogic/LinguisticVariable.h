#pragma once

#include <map>
#include "FuzzySet.h"

using namespace std;

class LinguisticVariable
{
public:
	LinguisticVariable(string &name);
	LinguisticVariable(string &name, map<string, FuzzySet> &valueNameToFuzzySetMapping);
	void addValueWithFuzzySet(const string &valueName, const FuzzySet &fuzzySet);
	map<string, double> fuzzify(double input);
	~LinguisticVariable();
	map<string, FuzzySet> valueNameToFuzzySetMapping;
	string name;
};

