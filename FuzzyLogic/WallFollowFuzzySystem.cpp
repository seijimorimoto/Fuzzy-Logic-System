#include "FuzzySet.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"
#include "WallFollowFuzzySystem.h"


FuzzySystem WallFollowFuzzySystem::generateFuzzySystem()
{
	double rightFrontLow[] = { 0, 0, 250, 350 };
	double rightFrontMed[] = { 250, 350, 500, 600 };
	double rightFrontHigh[] = { 500, 600, 7000, 7000 };
	double rightBackLow[] = { 0, 0, 250, 350 };
	double rightBackMed[] = { 250, 350, 500, 600 };
	double rightBackHigh[] = { 500, 600, 7000, 7000 };

	double leftVelLow[] = { 20, 40, 60, 80 };
	double leftVelMed[] = { 60, 80, 140, 160 };
	double leftVelHigh[] = { 140, 160, 180, 200 };
	double rightVelLow[] = { 20, 40, 60, 80 };
	double rightVelMed[] = { 60, 80, 140, 160 };
	double rightVelHigh[] = { 140, 160, 180, 200 };

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

	LinguisticVariable rightFront(rightFrontMapping);
	LinguisticVariable rightBack(rightBackMapping);
	LinguisticVariable leftVel(leftVelMapping);
	LinguisticVariable rightVel(rightVelMapping);

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

	vector<LinguisticVariable> inputVars{ rightFront, rightBack };
	vector<LinguisticVariable> outputVars{ leftVel, rightVel };
	return FuzzySystem(inputVars, outputVars, ruleBase);
}
