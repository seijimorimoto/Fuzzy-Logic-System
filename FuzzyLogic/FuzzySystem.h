#pragma once
#include <map>
#include <vector>
#include "LinguisticVariable.h"
#include "RuleBase.h"

using namespace std;

class FuzzySystem
{
public:
	FuzzySystem(vector<LinguisticVariable> &inputVars, vector<LinguisticVariable> &outputVars, RuleBase &ruleBase);
	~FuzzySystem();
	vector<double> execute(vector<double> &inputs);

private:
	vector<double> defuzzify();
	vector<map<string, double>> fuzzify(vector<double> &inputs);
	void processRules(vector<map<string, double>> fuzzifiedInputs);
	
	vector<LinguisticVariable> inputVars;
	RuleBase ruleBase;
	vector<LinguisticVariable> outputVars;
};

