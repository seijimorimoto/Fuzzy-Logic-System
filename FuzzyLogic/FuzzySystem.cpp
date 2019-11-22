#include "FuzzySystem.h"
#include "Util.h"


FuzzySystem::FuzzySystem(vector<LinguisticVariable> &inputVars, vector<LinguisticVariable> &outputVars, RuleBase &ruleBase)
	:inputVars(inputVars), outputVars(outputVars), ruleBase(ruleBase) {}


FuzzySystem::~FuzzySystem()
{
}

vector<double> FuzzySystem::execute(vector<double>& inputs)
{
	auto fuzzifiedInputs = fuzzify(inputs);
	processRules(fuzzifiedInputs);
	return defuzzify();
}

vector<double> FuzzySystem::defuzzify()
{
	return vector<double>();
}

vector<map<string, double>> FuzzySystem::fuzzify(vector<double>& inputs)
{
	vector<map<string, double>> fuzzifiedInputs;
	for (unsigned int i = 0; i < this->inputVars.size(); i++)
	{
		auto fuzzifiedInput = this->inputVars[i].fuzzify(inputs[i]);
		fuzzifiedInputs.push_back(fuzzifiedInput);
	}
	return fuzzifiedInputs;
}

void FuzzySystem::processRules(vector<map<string, double>> fuzzifiedInputs)
{
	vector<vector<string>> inputsLingLabels;
	vector<vector<double>> inputsLingValues;

	for (unsigned int i = 0; i < fuzzifiedInputs.size(); i++)
	{
		auto lingLabels = Util::getKeysFromMap(fuzzifiedInputs[i]);
		auto lingValues = Util::getValuesFromMap(fuzzifiedInputs[i]);
		inputsLingLabels.push_back(lingLabels);
		inputsLingValues.push_back(lingValues);
	}

	auto ruleBaseInputLabels = Util::getCombinations(inputsLingLabels);
	auto ruleBaseInputValues = Util::getCombinations(inputsLingValues);

	this->ruleBase.formatLingValuesAsRules(ruleBaseInputLabels, ruleBaseInputValues);
	this->ruleBase.computeFiringStrengths();
	this->ruleBase.printFiringStrenghts();
}
