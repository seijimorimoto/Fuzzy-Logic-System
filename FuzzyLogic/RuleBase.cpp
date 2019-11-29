#include <iostream>
#include <string>
#include "RuleBase.h"

RuleBase::RuleBase(map<vector<string>, vector<string>> rules)
{
	this->rules = rules;
}

RuleBase::~RuleBase()
{
}

void RuleBase::addRule(vector<string> inputLingLabels, vector<string> outputLingLabels)
{
	this->rules[inputLingLabels] = outputLingLabels;
}

void RuleBase::computeFiringStrengths()
{
	for (auto it = this->rulesInputValues.begin(); it != this->rulesInputValues.end(); ++it)
	{
		auto ruleLabels = it->first;
		auto ruleValues = it->second;
		double minValue = 2;
		for (unsigned int i = 0; i < ruleValues.size(); i++)
		{
			if (ruleValues[i] < minValue)
				minValue = ruleValues[i];
		}
		this->firingStrengths[ruleLabels] = minValue;
	}
}

void RuleBase::formatLingValuesAsRules(vector<vector<string>>& linguisticLabels, vector<vector<double>>& values)
{
	this->rulesInputValues.clear();
	for (unsigned int i = 0; i < linguisticLabels.size(); i++)
	{
		auto ruleLabels = linguisticLabels[i];
		auto ruleValues = values[i];
		this->rulesInputValues[ruleLabels] = ruleValues;
	}
}

vector<vector<string>> RuleBase::getAllRulesAntecedents()
{
	vector<vector<string>> antecedents;
	for (auto it = this->rules.begin(); it != this->rules.end(); ++it)
	{
		antecedents.push_back(it->first);
	}
	return antecedents;
}

vector<string> RuleBase::getConsequent(vector<string>& antecedent)
{
	return this->rules[antecedent];
}


double RuleBase::getFiringStrength(vector<string>& antecedent)
{
	return this->firingStrengths[antecedent];
}

void RuleBase::printFiringStrenghts()
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

unsigned int RuleBase::size()
{
	return this->rules.size();
}
