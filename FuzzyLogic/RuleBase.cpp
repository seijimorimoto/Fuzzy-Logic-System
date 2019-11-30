#include <iostream>
#include <string>
#include "RuleBase.h"

// Constructs a rule base for a fuzzy logic system.
// Params:
// - rules: The set of rules to be included in the rule base. It is represented as a mapping of
// sequences of input linguistic labels to sequences of output linguistic labels.
RuleBase::RuleBase(map<vector<string>, vector<string>> rules)
{
	this->rules = rules;
}

// Default destructor of a rule base.
RuleBase::~RuleBase()
= default;

// Adds a rule to the rule base.
// Params:
// - inputLingLabels: Vector of input linguistic labels to be used as antecedent of the new rule.
// - outputLingLabels: Vector of output linguistic labels to be used as consequent of the new rule.
void RuleBase::addRule(vector<string> inputLingLabels, vector<string> outputLingLabels)
{
	this->rules[inputLingLabels] = outputLingLabels;
}

// Computes the firing strengths of the rules in the rule base.
void RuleBase::computeFiringStrengths()
{
	// Iterate over all the input-side (antecedents) of the rules.
	for (auto it = this->rulesInputValues.begin(); it != this->rulesInputValues.end(); ++it)
	{
		auto ruleLabels = it->first;
		auto ruleValues = it->second;
		double minValue = 2;

		// Use the minimum operator in the input linguistic values to determine the firing strength
		// of the rule.
		for (unsigned int i = 0; i < ruleValues.size(); i++)
		{
			if (ruleValues[i] < minValue)
				minValue = ruleValues[i];
		}
		this->firingStrengths[ruleLabels] = minValue;
	}
}

// Formats linguistic labels and linguistic values (that are parallel/associated to each other) in
// the same way as the rules are internally stored.
// Params:
// - linguisticLabels: A vector where each position within it contains the linguistic labels of an
// input linguistic variable.
// - values: A vector where each position within it contains the membership values of linguistic
// labels found in the equivalent index in the 'linguisticLabels' parameter.
void RuleBase::formatLingValuesAsRules(vector<vector<string>>& linguisticLabels, vector<vector<double>>& values)
{
	for (unsigned int i = 0; i < linguisticLabels.size(); i++)
	{
		auto ruleLabels = linguisticLabels[i];
		auto ruleValues = values[i];
		this->rulesInputValues[ruleLabels] = ruleValues;
	}
}

// Returns the antecedents of all the rules in the rule base. The returned value is formatted as a
// vector, where each position within it contains the linguistic labels associated to the rule at
// that index.
vector<vector<string>> RuleBase::getAllRulesAntecedents()
{
	vector<vector<string>> antecedents;
	for (auto it = this->rules.begin(); it != this->rules.end(); ++it)
	{
		antecedents.push_back(it->first);
	}
	return antecedents;
}

// Returns the consequent of a given rule.
// Params:
// - antecedent: The antecedent of the rule for which the consequent is desired.
// Returns: The consequent of the rule as a vector of linguistic labels.
vector<string> RuleBase::getConsequent(vector<string>& antecedent)
{
	return this->rules[antecedent];
}

// Returns the firing strength of a particular rule.
// Params:
// - antecedent: The antecedent of the rule for which the firing strength is wanted.
double RuleBase::getFiringStrength(vector<string>& antecedent)
{
	return this->firingStrengths[antecedent];
}

// Prints the firing strengths of all rules within the rule base.
void RuleBase::printFiringStrengths()
{
	cout << "--------------------FIRING STRENGTHS--------------------" << endl;
	for (auto it = this->firingStrengths.begin(); it != this->firingStrengths.end(); ++it)
	{
		for (unsigned int i = 0; i < it->first.size(); i++)
		{
			if (i != 0) cout << " ";
			cout << it->first[i];
		}
		cout << ": " << it->second << endl;
	}
}

// Returns the amount of rules stored in this rule base.
unsigned int RuleBase::size()
{
	return this->rules.size();
}
