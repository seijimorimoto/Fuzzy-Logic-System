#include "FuzzySystem.h"
#include "Util.h"

// Constructs a fuzzy system.
// Params:
// - inputVars: Vector of linguistic variables that are going to be used as inputs to the system.
// - outputVars: Vector of linguistic variables that are going to be used as outputs to the system.
// - ruleBase: Set of rules that define how the output variables behave depending on the values of
// the input variables.
FuzzySystem::FuzzySystem(vector<LinguisticVariable> &inputVars, vector<LinguisticVariable> &outputVars, RuleBase &ruleBase)
	:inputVars(inputVars), outputVars(outputVars), ruleBase(ruleBase) {}

// Default destructor of a fuzzy system.
FuzzySystem::~FuzzySystem()
= default;

// Executes the complete fuzzy logic system process from given input values.
// Params:
// - inputs: Vector with the crisp values to be used as inputs in the input linguistic variables.
// Returns: A vector with the crisp outputs produced by the fuzzy logic system.
vector<double> FuzzySystem::execute(vector<double>& inputs)
{
	auto fuzzifiedInputs = fuzzify(inputs);
	processRules(fuzzifiedInputs);
	return defuzzify();
}

// Defuzzifies the fuzzy values of the output linguistic variables.
// Returns: A vector with the crisp values of the output linguistic variables.
vector<double> FuzzySystem::defuzzify()
{
	vector<double> defuzzifiedOutputs(this->outputVars.size(), 0);
	auto rulesAntecedents = this->ruleBase.getAllRulesAntecedents();
	double firingStrengthAccum = 0;

	// Iterate over all rules in the rule base.
	for (unsigned int i = 0; i < rulesAntecedents.size(); i++)
	{
		// Get the firing strength of the rule. If it is greater than 0, proceed with the
		// defuzzification process with this rule.
		auto firingStrength = this->ruleBase.getFiringStrength(rulesAntecedents[i]);
		if (firingStrength != 0)
		{
			firingStrengthAccum += firingStrength;
			auto ruleConsequent = this->ruleBase.getConsequent(rulesAntecedents[i]);
			// Iterate over the consequent of the rule (i.e. the vector with the linguistic value
			// of each of the output linguistic variables of the system).
			for (unsigned int j = 0; j < ruleConsequent.size(); j++)
			{
				// Calculate the center of set of the output linguistic label and multiply it by
				// the firing strength of the rule (center of sets defuzzification).
				auto outputLabel = ruleConsequent[j];
				auto centerOfSet = this->outputVars[j].getCenterOfSet(outputLabel);
				defuzzifiedOutputs[j] += centerOfSet * firingStrength;
			}
		}
	}

	// Divide the defuzzified output of each output linguistic variable by the sum of all the
	// firing strengths of the rules (part of the center of sets defuzzification).
	for (unsigned int i = 0; i < defuzzifiedOutputs.size(); i++)
	{
		defuzzifiedOutputs[i] /= firingStrengthAccum;
	}

	return defuzzifiedOutputs;
}

// Fuzzifies a set of given input values.
// Params:
// - inputs: Vector with the values to be fuzzified using the input linguistic variables.
// Returns: A vector of mappings from linguistic labels to membership values. Each mapping
// corresponds to an input linguistic variable.
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

// Given some fuzzified inputs, processes the rules in the rule base of the fuzzy logic system to
// generate the firing strength of each rule.
// Params:
// - fuzzifiedInputs: A vector of mappings from linguistic labels to membership values. Each
// mapping corresponds to an input linguistic variable.
void FuzzySystem::processRules(vector<map<string, double>> fuzzifiedInputs)
{
	vector<vector<string>> inputsLingLabels;
	vector<vector<double>> inputsLingValues;

	// Separate the linguistic labels from the membership values associated with them by keeping
	// them in two separate (but parallel) vectors.
	for (unsigned int i = 0; i < fuzzifiedInputs.size(); i++)
	{
		auto lingLabels = Util::getKeysFromMap(fuzzifiedInputs[i]);
		auto lingValues = Util::getValuesFromMap(fuzzifiedInputs[i]);
		inputsLingLabels.push_back(lingLabels);
		inputsLingValues.push_back(lingValues);
	}

	// Get all the combinations of linguistic labels between linguistic variables.
	// Do the same for the membershiip values.
	auto ruleBaseInputLabels = Util::getCombinations(inputsLingLabels);
	auto ruleBaseInputValues = Util::getCombinations(inputsLingValues);

	// Set the combinations generated to the correct format and calculate the firing strengths.
	this->ruleBase.formatLingValuesAsRules(ruleBaseInputLabels, ruleBaseInputValues);
	this->ruleBase.computeFiringStrengths();
}
