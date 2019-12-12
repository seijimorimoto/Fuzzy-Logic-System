#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "ObstacleAvoidFuzzySystem.h"
#include "RuleBase.h"

FuzzySystem ObstacleAvoidFuzzySystem::generateFuzzySystem()
{
	double frontLeftLow[] = { 0, 0, 500, 600 };
	double frontLeftMed[] = { 500, 600, 800, 900 };
	double frontLeftHigh[] = { 800, 900, 7000, 7000 };
	double frontCenterLow[] = { 0, 0, 500, 600 };
	double frontCenterMed[] = { 500, 600, 800, 900 };
	double frontCenterHigh[] = { 800, 900, 7000, 7000 };
	double frontRightLow[] = { 0, 0, 500, 600 };
	double frontRightMed[] = { 500, 600, 800, 900 };
	double frontRightHigh[] = { 800, 900, 7000, 7000 };

	double leftVelLow[] = { 10, 30, 40, 60 };
	double leftVelMed[] = { 60, 80, 120, 140 };
	double leftVelHigh[] = { 140, 160, 360, 380 };
	double rightVelLow[] = { 10, 30, 40, 60 };
	double rightVelMed[] = { 60, 80, 120, 140 };
	double rightVelHigh[] = { 140, 160, 360, 380 };

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

	LinguisticVariable frontLeft(frontLeftMapping);
	LinguisticVariable frontCenter(frontCenterMapping);
	LinguisticVariable frontRight(frontRightMapping);
	LinguisticVariable leftVel(leftVelMapping);
	LinguisticVariable rightVel(rightVelMapping);

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

	vector<LinguisticVariable> inputVars{ frontLeft, frontCenter, frontRight };
	vector<LinguisticVariable> outputVars{ leftVel, rightVel };
	return FuzzySystem(inputVars, outputVars, ruleBase);
}
