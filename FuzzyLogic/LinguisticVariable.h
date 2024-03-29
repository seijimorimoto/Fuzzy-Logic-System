#pragma once
#include <map>
#include "FuzzySet.h"

using namespace std;

class LinguisticVariable
{
public:
	LinguisticVariable();
	LinguisticVariable(map<string, FuzzySet> &labelToFuzzySetMapping);
	~LinguisticVariable();
	void addLabelWithFuzzySet(const string &label, const FuzzySet &fuzzySet);
	map<string, double> fuzzify(double input);
	double getCenterOfSet(string &fuzzySetName);

private:
	map<string, FuzzySet> labelToFuzzySetMapping;
};

