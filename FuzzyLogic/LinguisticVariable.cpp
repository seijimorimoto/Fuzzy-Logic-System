#include "LinguisticVariable.h"

// Default constructor of a linguistic variable.
LinguisticVariable::LinguisticVariable() {}

// Constructs a linguistic variable with fuzzy sets identified by linguistic labels.
// Params:
// - labelToFuzzySetMapping: A mapping between linguistic labels (strings) and the fuzzy sets each
// label represents.
LinguisticVariable::LinguisticVariable(map<string, FuzzySet> &labelToFuzzySetMapping)
{
	this->labelToFuzzySetMapping = labelToFuzzySetMapping;
}

// Default destructor of a linguistic variable.
LinguisticVariable::~LinguisticVariable()
= default;

// Adds a linguistic label with its corresponding fuzzy set to the linguistic variable.
// Params:
// - label: The linguistic label to add to the linguistic variable.
// - fuzzySet: The fuzzy set that will be identified by the 'label' in this linguistic variable.
void LinguisticVariable::addLabelWithFuzzySet(const string &label, const FuzzySet &fuzzySet)
{
	this->labelToFuzzySetMapping[label] = fuzzySet;
}

// Transforms a crisp value into its equivalent fuzzy representation by this linguistic variable.
// Params:
// - input: The crisp value to fuzzify.
// Returns: A mapping between linguistic labels (of this linguistic variable) and the membership
// value of the crisp value to the fuzzy sets represented by the labels.
map<string, double> LinguisticVariable::fuzzify(double input)
{
	map<string, double> linguisticLabelsToValues;
	for (auto it = this->labelToFuzzySetMapping.begin(); it != this->labelToFuzzySetMapping.end(); ++it)
	{
		const double value = it->second.computeMembershipValue(input);
		linguisticLabelsToValues[it->first] = value;
	}
	return linguisticLabelsToValues;
}

// Returns the center of the fuzzy set identified by a given linguistic label.
// Params:
// - fuzzySetName: The linguistic label that identifies the fuzzy set for which to get the center.
double LinguisticVariable::getCenterOfSet(string & fuzzySetName)
{
	return this->labelToFuzzySetMapping[fuzzySetName].getCenter();
}
