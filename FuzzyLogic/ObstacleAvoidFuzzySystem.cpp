#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "ObstacleAvoidFuzzySystem.h"
#include "RuleBase.h"

// Generates and returns a fuzzy system for implementing an obstacle avoidance behaviour in a
// robot.
FuzzySystem ObstacleAvoidFuzzySystem::generateFuzzySystem()
{
	// Setting up the membership functions for the input variables.
	double frontLeftLow[] = { 0, 0, 500, 600 };
	double frontLeftMed[] = { 500, 600, 800, 900 };
	double frontLeftHigh[] = { 800, 900, 7000, 7000 };
	double frontCenterLow[] = { 0, 0, 500, 600 };
	double frontCenterMed[] = { 500, 600, 800, 900 };
	double frontCenterHigh[] = { 800, 900, 7000, 7000 };
	double frontRightLow[] = { 0, 0, 500, 600 };
	double frontRightMed[] = { 500, 600, 800, 900 };
	double frontRightHigh[] = { 800, 900, 7000, 7000 };

	// Setting up the membership functions for the output variables.
	double leftVelLow[] = { 10, 30, 40, 60 };
	double leftVelMed[] = { 60, 80, 120, 140 };
	double leftVelHigh[] = { 140, 160, 360, 380 };
	double rightVelLow[] = { 10, 30, 40, 60 };
	double rightVelMed[] = { 60, 80, 120, 140 };
	double rightVelHigh[] = { 140, 160, 360, 380 };

	// Associating each defined membership function (fuzzy set) to a name for each input and output
	// variables.
	map<string, FuzzySet> frontLeftMapping = {
		{ "Low", FuzzySet(frontLeftLow) },
		{ "Med", FuzzySet(frontLeftMed) },
		{ "High", FuzzySet(frontLeftHigh) },
	};
	map<string, FuzzySet> frontCenterMapping = {
		{ "Low", FuzzySet(frontCenterLow) },
		{ "Med", FuzzySet(frontCenterMed) },
		{ "High", FuzzySet(frontCenterHigh) },
	};
	map<string, FuzzySet> frontRightMapping = {
		{ "Low", FuzzySet(frontRightLow) },
		{ "Med", FuzzySet(frontRightMed) },
		{ "High", FuzzySet(frontRightHigh) },
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
	LinguisticVariable frontLeft(frontLeftMapping);
	LinguisticVariable frontCenter(frontCenterMapping);
	LinguisticVariable frontRight(frontRightMapping);
	LinguisticVariable leftVel(leftVelMapping);
	LinguisticVariable rightVel(rightVelMapping);

	// Create the rule base with all the combinations of linguistic labels for each of the
	// linguistic variables. Associate to each rule the labels that will be produced in the output
	// linguistic variables. The order of labels in each rule is important and should be kept
	// consistent among all rules.
	RuleBase ruleBase(map<vector<string>, vector<string>> {
		{ vector<string>{ "Low", "Low", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "Low", "Low", "Med" }, vector<string>{ "Med", "Low" } },
		{ vector<string>{ "Low", "Low", "High" }, vector<string>{ "High", "Low" } },
		{ vector<string>{ "Low", "Med", "Low" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Low", "Med", "Med" }, vector<string>{ "High", "Med" } },
		{ vector<string>{ "Low", "Med", "High" }, vector<string>{ "High", "Low" } },
		{ vector<string>{ "Low", "High", "Low" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Low", "High", "Med" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Low", "High", "High" }, vector<string>{ "High", "Med" } },
		{ vector<string>{ "Med", "Low", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "Med", "Low", "Med" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "Med", "Low", "High" }, vector<string>{ "High", "Low" } },
		{ vector<string>{ "Med", "Med", "Low" }, vector<string>{ "Low", "Med" } },
		{ vector<string>{ "Med", "Med", "Med" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Med", "Med", "High" }, vector<string>{ "High", "Med" } },
		{ vector<string>{ "Med", "High", "Low" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Med", "High", "Med" }, vector<string>{ "Med", "Med" } },
		{ vector<string>{ "Med", "High", "High" }, vector<string>{ "High", "Med" } },
		{ vector<string>{ "High", "Low", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "High", "Low", "Med" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "High", "Low", "High" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "High", "Med", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "High", "Med", "Med" }, vector<string>{ "Low", "Med" } },
		{ vector<string>{ "High", "Med", "High" }, vector<string>{ "Low", "Med" } },
		{ vector<string>{ "High", "High", "Low" }, vector<string>{ "Low", "High" } },
		{ vector<string>{ "High", "High", "Med" }, vector<string>{ "Med", "High" } },
		{ vector<string>{ "High", "High", "High" }, vector<string>{ "Med", "Med" } },
	});

	// Specify which linguistic variables are to be the inputs and which the outputs and then
	// construct a fuzzy logic system based on them and the defined rule base.
	vector<LinguisticVariable> inputVars{ frontLeft, frontCenter, frontRight };
	vector<LinguisticVariable> outputVars{ leftVel, rightVel };
	return FuzzySystem(inputVars, outputVars, ruleBase);
}
