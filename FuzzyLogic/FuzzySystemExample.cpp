#include <iostream>
#include "Examples.h"
#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"

namespace FuzzySystemExample
{
	int main()
	{
		// Setting up the membership functions for the input variables.
		// In this example, 2 membership functions for 'Food' and 3 for 'Service'.
		double foodRancidFunc[] = { 0, 0, 1, 5 };
		double foodDeliciFunc[] = { 4, 8, 9 ,9 };
		double servicePoorFunc[] = { 0, 0, 1, 3 };
		double serviceGoodFunc[] = { 1, 3, 5, 7 };
		double serviceExceFunc[] = { 5, 7, 9, 9 };

		// Setting up the membership functions for the output variables.
		double tipCheapFunc[] = { 0, 6, 6, 12 };
		double tipAverageFunc[] = { 10, 15, 15, 20 };
		double tipGenerousFunc[] = { 18, 24, 24, 30 };

		// Associating each membership function (fuzzy set) to a name. In this case:
		// The 'Food' membership functions are: 'Rancid' and 'Delici'.
		// The 'Service' membership functions are: 'Poor', 'Good' and 'Exce'.
		// The 'Tip' membership functions are: 'Cheap', 'Average' and 'Generous'.
		map<string, FuzzySet> foodMapping = {
			{ "Rancid", FuzzySet(foodRancidFunc) },
			{ "Delici", FuzzySet(foodDeliciFunc) },
		};
		map<string, FuzzySet> serviceMapping = {
			{ "Poor", FuzzySet(servicePoorFunc) },
			{ "Good", FuzzySet(serviceGoodFunc) },
			{ "Exce", FuzzySet(serviceExceFunc) }
		};
		map<string, FuzzySet> tipMapping = {
			{ "Cheap", FuzzySet(tipCheapFunc) },
			{ "Average", FuzzySet(tipAverageFunc) },
			{ "Generous", FuzzySet(tipGenerousFunc) }
		};

		// Create a linguistic variable for each input and output. This kind of variables hold the fuzzy
		// sets related to them.
		LinguisticVariable lingFood(foodMapping);
		LinguisticVariable lingService(serviceMapping);
		LinguisticVariable lingTip(tipMapping);

		// Create the rule base with all the combinations of linguistic labels for each of the
		// linguistic variables. Associate to each rule the labels that will be produced in the output
		// linguistic variables. The order of labels in each rule is important and should be kept
		// consistent among all rules.
		RuleBase ruleBase(map<vector<string>, vector<string>> {
			{ vector<string>{ "Rancid", "Poor" }, vector<string>{ "Cheap" } },
			{ vector<string>{ "Rancid", "Good" }, vector<string>{ "Cheap" } },
			{ vector<string>{ "Rancid", "Exce" }, vector<string>{ "Average" } },
			{ vector<string>{ "Delici", "Poor" }, vector<string>{ "Average" } },
			{ vector<string>{ "Delici", "Good" }, vector<string>{ "Average" } },
			{ vector<string>{ "Delici", "Exce" }, vector<string>{ "Generous" } }
		});

		// Create the fuzzy logic system.
		auto inputVars = vector<LinguisticVariable>{ lingFood, lingService };
		auto outputVars = vector<LinguisticVariable>{ lingTip };
		FuzzySystem fuzzySystem(inputVars, outputVars, ruleBase);

		// Initialize values for the inputs. In real scenarios, these are going to be obtained through
		// some defined method, like sensor readings.
		vector<double> inputs{ 4, 8.5 };

		// Process the inputs and get outputs with the fuzzy logic system.
		auto outputs = fuzzySystem.execute(inputs);

		for (unsigned int i = 0; i < outputs.size(); i++)
		{
			cout << outputs[i] << " ";
		}
		cout << endl;

		cin.get();
		return 0;
	}
}