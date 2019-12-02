#include <limits>
#include "HierarchicalFuzzySystem.h"


HierarchicalFuzzySystem::HierarchicalFuzzySystem(vector<FuzzySystem> behaviourFuzzySystems, vector<FuzzySet> behaviourFuzzySets)
{
	this->behaviourFuzzySystems = behaviourFuzzySystems;
	this->behaviourFuzzySets = behaviourFuzzySets;
}

HierarchicalFuzzySystem::~HierarchicalFuzzySystem()
= default;

vector<double> HierarchicalFuzzySystem::execute(vector<vector<double>>& inputs)
{
	vector<vector<double>> outputs;
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		auto behaviourOutput = behaviourFuzzySystems[i].execute(inputs[i]);
		if (outputs.size() < behaviourOutput.size())
		{
			outputs.resize(behaviourOutput.size());
		}
		for (unsigned int j = 0; j < behaviourOutput.size(); j++)
		{
			outputs[j].push_back(behaviourOutput[j]);
		}
	}

	vector<double> behavioursFiringStrengths;
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		double minInput = numeric_limits<double>::max();
		for (unsigned int j = 0; j < inputs[i].size(); j++)
		{
			if (inputs[i][j] < minInput)
			{
				minInput = inputs[i][j];
			}
		}
		behavioursFiringStrengths.push_back(minInput);
	}

	vector<double> crispOutputs;
	for (unsigned int i = 0; i < outputs.size(); i++)
	{
		double crispOutput = 0;
		double firingStrengthsAccum = 0;
		for (unsigned int j = 0; j < outputs[i].size(); j++)
		{
			crispOutput += behavioursFiringStrengths[j] * outputs[i][j];
			firingStrengthsAccum += behavioursFiringStrengths[j];
		}
		crispOutput /= firingStrengthsAccum;
		crispOutputs.push_back(crispOutput);
	}

	return crispOutputs;
}


