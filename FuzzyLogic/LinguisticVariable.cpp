#include "LinguisticVariable.h"

LinguisticVariable::LinguisticVariable(string name)
{
	this->name = name;
}

LinguisticVariable::LinguisticVariable(string name, map<string, FuzzySet> &labelToFuzzySetMapping)
{
	this->name = name;
	this->labelToFuzzySetMapping = labelToFuzzySetMapping;
}

LinguisticVariable::~LinguisticVariable()
= default;

void LinguisticVariable::addLabelWithFuzzySet(const string &valueName, const FuzzySet &fuzzySet)
{
	this->labelToFuzzySetMapping[valueName] = fuzzySet;
}

map<string, double> LinguisticVariable::fuzzify(double input)
{
	map<string, double> linguisticLabelsToValues;
	for (auto it = this->labelToFuzzySetMapping.begin(); it != this->labelToFuzzySetMapping.end(); ++it)
	{
		const double value = it->second.computeMembershipValue(input);
		linguisticLabelsToValues[it->first] = value;
	}
	return linguisticLabelsToValues;
}

double LinguisticVariable::getCenterOfSet(string & fuzzySetName)
{
	return this->labelToFuzzySetMapping[fuzzySetName].getCenter();
}
