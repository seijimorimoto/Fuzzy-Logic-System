#pragma once
#include <map>
#include <vector>

using namespace std;

class RuleBase
{
public:
	RuleBase(map<vector<string>, vector<string>> rules);
	~RuleBase();
	void addRule(vector<string> inputLingLabels, vector<string> outputLingLabels);
	void computeFiringStrengths();
	void formatLingValuesAsRules(vector<vector<string>> &linguisticLabels, vector<vector<double>> &values);
	vector<vector<string>> getAllRulesAntecedents();
	vector<string> getConsequent(vector<string> &antecedent);
	double getFiringStrength(vector<string> &antecedent);
	void printFiringStrengths();
	unsigned int size();

private:
	map<vector<string>, double> firingStrengths;
	map<vector<string>, vector<string>> rules;
	map<vector<string>, vector<double>> rulesInputValues;
};