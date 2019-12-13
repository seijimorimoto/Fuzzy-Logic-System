#include "FuzzySet.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"
#include "WallFollowFuzzySystem.h"

// Generates and returns a fuzzy system for implementing a right-edge wall following behaviour in a
// robot.
FuzzySystem WallFollowFuzzySystem::generateFuzzySystem()
{
	// Setting up the membership functions for the input variables.
	double rightFrontLow[] = { 0, 0, 250, 350 };
	double rightFrontMed[] = { 250, 350, 500, 600 };
	double rightFrontHigh[] = { 500, 600, 7000, 7000 };
	double rightBackLow[] = { 0, 0, 250, 350 };
	double rightBackMed[] = { 250, 350, 500, 600 };
	double rightBackHigh[] = { 500, 600, 7000, 7000 };

	// Setting up the membership functions for the output variables.
	double leftVelLow[] = { 20, 40, 60, 80 };
	double leftVelMed[] = { 60, 80, 140, 160 };
	double leftVelHigh[] = { 140, 160, 180, 200 };
	double rightVelLow[] = { 20, 40, 60, 80 };
	double rightVelMed[] = { 60, 80, 140, 160 };
	double rightVelHigh[] = { 140, 160, 180, 200 };

	// Associating each defined membership function (fuzzy set) to a name for each input and output
	// variables.
	map<string, FuzzySet> rightFrontMapping = {
		{ "Low", FuzzySet(rightFrontLow) },
		{ "Med", FuzzySet(rightFrontMed) },
		{ "High", FuzzySet(rightFrontHigh) },
	};
	map<string, FuzzySet> rightBackMapping = {
		{ "Low", FuzzySet(rightBackLow) },
		{ "Med", FuzzySet(rightBackMed) },
		{ "High", FuzzySet(rightBackHigh) },
	};
	map<string, FuzzySet> leftVelMapping = {
		{ "Low", FuzzySet(leftVelLow) },
		{ "Med", FuzzySet(leftVelMed) },
		{ "High", FuzzySet(leftVelHigh) },
	};
	map<string, FuzzySet> rightVelMapping = {
		{ "Low", FuzzySet(rightVelLow) },
		{ "Med", FuzzySet(rightVelMed) },
		{ "High", FuzzySet(rightVelHigh) },
	};

	// Create a linguistic variable for each input and output. This kind of variables hold the
	// fuzzy sets related to them.
	LinguisticVariable rightFront(rightFrontMapping);
	LinguisticVariable rightBack(rightBackMapping);
	LinguisticVariable leftVel(leftVelMapping);
	LinguisticVariable rightVel(rightVelMapping);

	// Create the rule base with all the combinations of linguistic labels for each of the
	// linguistic variables. Associate to each rule the labels that will be produced in the output
	// linguistic variables. The order of labels in each rule is important and should be kept
	// consistent among all rules.
	RuleBase ruleBase(map<vector<string>, vector<string>> {
		{ vector<string>{ "Low", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "Low", "Med" }, vector<string>{ "Med", "High" } },
		{ vector<string>{ "Low", "High" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "Med", "Low" }, vector<string>{ "Med", "Low" } },
		{ vector<string>{ "Med", "Med" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Med", "High" }, vector<string>{ "Med", "High" } },
		{ vector<string>{ "High", "Low" }, vector<string>{ "Med", "High" } },
		{ vector<string>{ "High", "Med" }, vector<string>{ "Med", "Low" } },
		{ vector<string>{ "High", "High" }, vector<string>{ "High", "Low" } },
	});

	// Specify which linguistic variables are to be the inputs and which the outputs and then
	// construct a fuzzy logic system based on them and the defined rule base.
	vector<LinguisticVariable> inputVars{ rightFront, rightBack };
	vector<LinguisticVariable> outputVars{ leftVel, rightVel };
	return FuzzySystem(inputVars, outputVars, ruleBase);
}
