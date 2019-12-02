#include <iostream>
#include <cmath>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySystem.h"
#include "WallFollowFuzzySystem.h"
#define PI 3.14159265

using namespace std;

namespace WallFollow
{
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
		auto fuzzySystem = WallFollowFuzzySystem::generateFuzzySystem();

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
}