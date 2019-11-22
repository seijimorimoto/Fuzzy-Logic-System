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
	vector<double> defuzzifiedOutputs(this->outputVars.size(), 0);
	auto rulesAntecedents = this->ruleBase.getAllRulesAntecedents();
	double firingStrengthAccum = 0;
	for (unsigned int i = 0; i < rulesAntecedents.size(); i++)
	{
		auto firingStrength = this->ruleBase.getFiringStrength(rulesAntecedents[i]);
		if (firingStrength != 0)
		{
			firingStrengthAccum += firingStrength;
			auto ruleConsequent = this->ruleBase.getConsequent(rulesAntecedents[i]);
			for (unsigned int j = 0; j < ruleConsequent.size(); j++)
			{
				auto outputLabel = ruleConsequent[j];
				auto centerOfSet = this->outputVars[j].getCenterOfSet(outputLabel);
				defuzzifiedOutputs[j] += centerOfSet * firingStrength;
			}
		}
	}

	for (unsigned int i = 0; i < defuzzifiedOutputs.size(); i++)
	{
		defuzzifiedOutputs[i] /= firingStrengthAccum;
	}

	return defuzzifiedOutputs;
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
}
