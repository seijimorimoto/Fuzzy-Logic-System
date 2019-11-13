#include "LinguisticVariable.h"

LinguisticVariable::LinguisticVariable(string &name)
{
	this->name = name;
}

LinguisticVariable::LinguisticVariable(string &name, map<string, FuzzySet> &valueNameToFuzzySetMapping)
{
	this->name = name;
	this->valueNameToFuzzySetMapping = valueNameToFuzzySetMapping;
}

void LinguisticVariable::addValueWithFuzzySet(const string &valueName, const FuzzySet &fuzzySet)
{
	this->valueNameToFuzzySetMapping[valueName] = fuzzySet;
}

map<string, double> LinguisticVariable::fuzzify(double input)
{
	map<string, double> linguisticLabelsToValues;
	for (auto it = this->valueNameToFuzzySetMapping.begin(); it != this->valueNameToFuzzySetMapping.end(); ++it)
	{
		const double value = it->second.computeMembershipValue(input);
		linguisticLabelsToValues[it->first] = value;
	}
	return linguisticLabelsToValues;
}


LinguisticVariable::~LinguisticVariable()
= default;
