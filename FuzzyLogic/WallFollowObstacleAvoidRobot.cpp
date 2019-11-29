#include <iostream>
#include <cmath>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"
#define PI 3.14159265

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

			rightFrontInput = sonarSensors[6]->getRange();
			rightFrontInput = rightFrontInput * cos(40 * PI / 180);
			rightBackInput = sonarSensors[7]->getRange();

			vector<double> inputs{ rightFrontInput, rightBackInput };
			auto outputs = fuzzySystem.execute(inputs);
			cout << "Inputs: " << inputs[0] << " " << inputs[1] << endl;
			cout << "Outputs: " << outputs[0] << " " << outputs[1] << endl;

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
		double rightFrontLow[] = { 0, 0, 500, 700 };
		double rightFrontMed[] = { 600, 800, 800, 1000 };
		double rightFrontHigh[] = { 900, 1100, 7000, 7000 };
		double rightBackLow[] = { 0, 0, 500, 700 };
		double rightBackMed[] = { 600, 800, 800, 1000 };
		double rightBackHigh[] = { 900, 1100, 7000, 7000 };

		double leftVelLow[] = { 90, 100, 110, 120 };
		double leftVelMed[] = { 110, 130, 150, 170 };
		double leftVelHigh[] = { 160, 180, 200, 220 };
		double rightVelLow[] = { 90, 100, 110, 120 };
		double rightVelMed[] = { 110, 130, 150, 170 };
		double rightVelHigh[] = { 160, 180, 200, 220 };

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
			{ vector<string>{ "High", "Low" }, vector<string>{ "High", "Low" } },
			{ vector<string>{ "High", "Med" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "High", "High" }, vector<string>{ "High", "Low" } },
		});

		vector<LinguisticVariable> inputVars{ rightFront, rightBack };
		vector<LinguisticVariable> outputVars{ leftVel, rightVel };
		return FuzzySystem(inputVars, outputVars, ruleBase);
	}
}