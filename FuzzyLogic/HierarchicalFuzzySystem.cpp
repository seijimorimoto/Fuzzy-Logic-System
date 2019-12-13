#include <limits>
#include "HierarchicalFuzzySystem.h"

// Constructs a hierarchical fuzzy system.
// Params:
// - behaviourFuzzySystems: A vector of the fuzzy systems that will form part of this hierarchical
//   fuzzy system.
// - behaviourFuzzySets: A vector of the fuzzy sets that will be used for regulating the effect of
//   the outputs of each fuzzy system within the hierarchical fuzzy system. This vector is parallel
//   to the behaviourFuzzySystems vector.
HierarchicalFuzzySystem::HierarchicalFuzzySystem(vector<FuzzySystem> behaviourFuzzySystems, vector<FuzzySet> behaviourFuzzySets)
{
	this->behaviourFuzzySystems = behaviourFuzzySystems;
	this->behaviourFuzzySets = behaviourFuzzySets;
}

// Default destructor of a hierarchical fuzzy system.
HierarchicalFuzzySystem::~HierarchicalFuzzySystem()
= default;

// Executes the hierarchical fuzzy logic system process from given input values.
// Params:
// - inputs: Vector containing vectors of crisp values. Each vector represents the crisp values
//   that will serve as inputs to the corresponding fuzzy system that forms part of the
//   hierarchical fuzzy system.
// Returns: A vector with the crisp values for each output variable of the hierarchical fuzzy
// system (i.e. each output variable in all the fuzzy systems forming part of the hierarchy).
vector<double> HierarchicalFuzzySystem::execute(vector<vector<double>>& inputs)
{
	// Iterate over the vector of inputs.
	vector<vector<double>> outputs;	
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		// Execute the fuzzy system at the current iteration by passing the current input values.
		auto behaviourOutput = behaviourFuzzySystems[i].execute(inputs[i]);

		// Resize the vector of outputs of the hierarchical fuzzy system if it was smaller than the
		// output vector of the fuzzy system in the current iteration.
		if (outputs.size() < behaviourOutput.size())
		{
			outputs.resize(behaviourOutput.size());
		}

		// Append the output values returned by the fuzzy system of the current iteration into the
		// corresponding position in the vector of outputs of the hierarchical fuzzy system.
		for (unsigned int j = 0; j < behaviourOutput.size(); j++)
		{
			outputs[j].push_back(behaviourOutput[j]);
		}
	}

	// Iterate over the vector of inputs.
	vector<double> behavioursFiringStrengths;
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		// Get the minimum value of the inputs at this iteration.
		double minInput = numeric_limits<double>::max();
		for (unsigned int j = 0; j < inputs[i].size(); j++)
		{
			if (inputs[i][j] < minInput)
			{
				minInput = inputs[i][j];
			}
		}

		// Assign the retrieved minimum value as the firing strength of the fuzzy system
		// corresponding to this iteration of the inputs vector.
		behavioursFiringStrengths.push_back(minInput);
	}

	// Iterate over the vector of outputs of the hierarchical fuzzy system.
	vector<double> crispOutputs;
	for (unsigned int i = 0; i < outputs.size(); i++)
	{
		double crispOutput = 0;
		double firingStrengthsAccum = 0;
		
		// Iterate over the values obtained from the fuzzy systems for the output variable being
		// explored currently. Multiply each value by the firing strength of the corresponding
		// fuzzy system and accumulate them. Also calculate the sum of all firing strengths.
		for (unsigned int j = 0; j < outputs[i].size(); j++)
		{
			crispOutput += behavioursFiringStrengths[j] * outputs[i][j];
			firingStrengthsAccum += behavioursFiringStrengths[j];
		}

		// Calculate the final value of the output variable corresponding to this iteration by
		// dividing the accumulated weighted value by the sum of all firing strengths. Add that
		// result to the vector of final crisp output values.
		crispOutput /= firingStrengthsAccum;
		crispOutputs.push_back(crispOutput);
	}

	return crispOutputs;
}


