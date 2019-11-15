#include <iostream>
#include <string>
#include "FuzzySet.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"
#include "Util.h"

int main()
{
	// Setting up the membership functions for the input variables.
	// In this example, 2 membership functions for 'Food' and 3 for 'Service'.
	double foodRancidFunc[] = { 0, 0, 1, 5 };
	double foodDeliciFunc[] = { 4, 8, 9 ,9 };
	double servicePoorFunc[] = { 0, 0, 1, 3 };
	double serviceGoodFunc[] = { 1, 3, 5, 7 };
	double serviceExceFunc[] = { 5, 7, 9, 9 };
	
	// Associating each membership function (fuzzy set) to a name. In this case:
	// The 'Food' membership functions are: 'Rancid' and 'Delici'.
	// The 'Service' membership functions are: 'Poor', 'Good' and 'Exce'.
	map<string, FuzzySet> foodMapping = {
		{ "Rancid", FuzzySet(foodRancidFunc)},
		{ "Delici", FuzzySet(foodDeliciFunc)},
	};
	map<string, FuzzySet> serviceMapping = {
		{ "Poor", FuzzySet(servicePoorFunc) },
		{ "Good", FuzzySet(serviceGoodFunc) },
		{ "Exce", FuzzySet(serviceExceFunc) }
	};

	// Create a linguistic variable for each input. This kind of variables hold the fuzzy sets
	// related to them.
	LinguisticVariable lingFood("Food", foodMapping);
	LinguisticVariable lingService("Service", serviceMapping);

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

	// Initialize values for the input. In real scenarios, these are going to be obtained through
	// some defined method, like sensor readings.
	double inputFood = 4;
	double inputService = 8.5;

	// Fuzzify the input values.
	auto fuzzyFood = lingFood.fuzzify(inputFood);
	auto fuzzyService = lingService.fuzzify(inputService);

	// Separate the labels and the obtained values (in fuzzification) for each label of each
	// linguistic variable.
	auto lingLabelsFood = Util::getKeysFromMap(fuzzyFood);
	auto lingLabelsService = Util::getKeysFromMap(fuzzyService);
	auto lingValuesFood = Util::getValuesFromMap(fuzzyFood);
	auto lingValuesService = Util::getValuesFromMap(fuzzyService);

	// Generate all combinations for the labels of the linguistic variables. Do the same with the
	// obtained values (in fuzzification) of each label. The resulting vectors of combinations of
	// labels and the one of values are consistent in the way they are ordered.
	auto ruleBaseInputLabels = Util::getCombinations(vector<vector<string>>{ lingLabelsFood, lingLabelsService });
	auto ruleBaseInputValues = Util::getCombinations(vector<vector<double>>{ lingValuesFood, lingValuesService });

	// Associate the combinations obtained for the labels and values with the rules in the rule
	// base. Then compute the firing strength of each rule.
	ruleBase.formatLingValuesAsRules(ruleBaseInputLabels, ruleBaseInputValues);
	ruleBase.computeFiringStrengths();
	ruleBase.printFiringStrenghts();

	cin.get();
	
	return 0;
}