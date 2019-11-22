#include <iostream>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"

using namespace std;

namespace WallFollowObstacleAvoidRobot
{
	FuzzySystem generateFuzzySystem();

	int main(int argc, char * argv[])
	{
		// Create instances.
		Aria::init();
		ArRobot robot;

		// Parse command line arguments.
		ArArgumentParser argParser(&argc, argv);
		argParser.loadDefaultArguments();

		// Establish a connection to the robot.
		ArRobotConnector robotConnector(&argParser, &robot);
		if (robotConnector.connectRobot()) cout << "Robot connected!" << endl;

		// Enable the motors.
		robot.runAsync(false);
		robot.lock();
		robot.enableMotors();
		robot.unlock();

		// Declare / initialize variables.
 		ArSensorReading *sonarSensors[8];
		double rightFrontInput, rightBackInput;
		auto fuzzySystem = generateFuzzySystem();

		while (true)
		{
			for (unsigned int i = 0; i < 8; i++)
			{
				sonarSensors[i] = robot.getSonarReading(i);
			}

			rightFrontInput = sonarSensors[5]->getRange();
			rightBackInput = sonarSensors[7]->getRange();

			vector<double> inputs{ rightFrontInput, rightBackInput };
			auto outputs = fuzzySystem.execute(inputs);

			robot.setVel2(outputs[0], outputs[1]);
			ArUtil::sleep(100);
		}

		// Stop the robot.
		robot.lock();
		robot.stop();
		robot.unlock();

		// Terminate all threads and exit.
		Aria::exit();
		return 0;
	}

	FuzzySystem generateFuzzySystem()
	{
		double rightFrontLow[] = { 0, 0, 300, 500 };
		double rightFrontMed[] = { 300, 500, 500, 800 };
		double rightFrontHigh[] = { 500, 800, 5000, 5000 };
		double rightBackLow[] = { 0, 0, 300, 500 };
		double rightBackMed[] = { 300, 500, 500, 800 };
		double rightBackHigh[] = { 500, 800, 5000, 5000 };

		double leftVelLow[] = { 60, 80, 80, 100 };
		double leftVelMed[] = { 90, 120, 120, 150 };
		double leftVelHigh[] = { 130, 150, 150, 170 };
		double rightVelLow[] = { 60, 80, 80, 100 };
		double rightVelMed[] = { 90, 120, 120, 150 };
		double rightVelHigh[] = { 130, 150, 150, 170 };

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
			{ vector<string>{ "Low", "Low" }, vector<string>{ "Low", "Med" } },
			{ vector<string>{ "Low", "Med" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "Low", "High" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "Med", "Low" }, vector<string>{ "Med", "Low" } },
			{ vector<string>{ "Med", "Med" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Med", "High" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "High", "Low" }, vector<string>{ "Med", "Low" } },
			{ vector<string>{ "High", "Med" }, vector<string>{ "Med", "Low" } }, // High Med
			{ vector<string>{ "High", "High" }, vector<string>{ "High", "Med" } }, // High Low
		});

		vector<LinguisticVariable> inputVars{ rightFront, rightBack };
		vector<LinguisticVariable> outputVars{ leftVel, rightVel };
		return FuzzySystem(inputVars, outputVars, ruleBase);
	}
}