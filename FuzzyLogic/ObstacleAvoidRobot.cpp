#include <iostream>
#include <cmath>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "LinguisticVariable.h"
#include "RuleBase.h"

using namespace std;

namespace ObstacleAvoidRobot
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
		double frontLeftInput, frontCenterInput, frontRightInput;
		auto fuzzySystem = generateFuzzySystem();

		while (true)
		{
			for (unsigned int i = 0; i < 8; i++)
			{
				sonarSensors[i] = robot.getSonarReading(i);
			}

			frontLeftInput = min(sonarSensors[1]->getRange(), sonarSensors[2]->getRange());
			frontCenterInput = min(sonarSensors[3]->getRange(), sonarSensors[4]->getRange());
			frontRightInput = min(sonarSensors[5]->getRange(), sonarSensors[6]->getRange());

			vector<double> inputs{ frontLeftInput, frontCenterInput, frontRightInput };
			auto outputs = fuzzySystem.execute(inputs);
			cout << "Inputs: " << inputs[0] << " " << inputs[1] << " " << inputs[2] << endl;
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
		double frontLeftLow[] = { 0, 0, 300, 500 };
		double frontLeftMed[] = { 300, 500, 500, 800 };
		double frontLeftHigh[] = { 500, 800, 7000, 7000 };
		double frontCenterLow[] = { 0, 0, 300, 500 };
		double frontCenterMed[] = { 300, 500, 500, 800 };
		double frontCenterHigh[] = { 500, 800, 7000, 7000 };
		double frontRightLow[] = { 0, 0, 300, 500 };
		double frontRightMed[] = { 300, 500, 500, 800 };
		double frontRightHigh[] = { 500, 800, 7000, 7000 };

		double leftVelLow[] = { 50, 80, 80, 100 };
		double leftVelMed[] = { 110, 130, 150, 170 };
		double leftVelHigh[] = { 160, 180, 200, 220 };
		double rightVelLow[] = { 50, 80, 80, 100 };
		double rightVelMed[] = { 110, 130, 150, 170 };
		double rightVelHigh[] = { 160, 180, 200, 220 };

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
			{ vector<string>{ "Low", "Low", "High" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "Low", "Med", "Low" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Low", "Med", "Med" }, vector<string>{ "Med", "Low" } },
			{ vector<string>{ "Low", "Med", "High" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "Low", "High", "Low" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Low", "High", "Med" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Low", "High", "High" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "Med", "Low", "Low" }, vector<string>{ "Low", "High" } },
			{ vector<string>{ "Med", "Low", "Med" }, vector<string>{ "Low", "Med" } },
			{ vector<string>{ "Med", "Low", "High" }, vector<string>{ "Med", "Low" } },
			{ vector<string>{ "Med", "Med", "Low" }, vector<string>{ "Low", "Med" } },
			{ vector<string>{ "Med", "Med", "Med" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Med", "Med", "High" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "Med", "High", "Low" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Med", "High", "Med" }, vector<string>{ "Med", "Med" } },
			{ vector<string>{ "Med", "High", "High" }, vector<string>{ "High", "Med" } },
			{ vector<string>{ "High", "Low", "Low" }, vector<string>{ "Low", "High" } },
			{ vector<string>{ "High", "Low", "Med" }, vector<string>{ "Low", "High" } },
			{ vector<string>{ "High", "Low", "High" }, vector<string>{ "Low", "High" } },
			{ vector<string>{ "High", "Med", "Low" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "High", "Med", "Med" }, vector<string>{ "Low", "Med" } },
			{ vector<string>{ "High", "Med", "High" }, vector<string>{ "Low", "Med" } },
			{ vector<string>{ "High", "High", "Low" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "High", "High", "Med" }, vector<string>{ "Med", "High" } },
			{ vector<string>{ "High", "High", "High" }, vector<string>{ "Med", "Med" } },
		});

		vector<LinguisticVariable> inputVars{ frontLeft, frontCenter, frontRight };
		vector<LinguisticVariable> outputVars{ leftVel, rightVel };
		return FuzzySystem(inputVars, outputVars, ruleBase);
	}
}