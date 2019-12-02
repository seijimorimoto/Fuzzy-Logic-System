#include <iostream>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySystem.h"
#include "ObstacleAvoidFuzzySystem.h"
#include "WallFollowFuzzySystem.h"
#define PI 3.14159265

using namespace std;

namespace WallFollowObstacleAvoidSubsumption
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
		double frontLeftInput, frontCenterInput, frontRightInput, rightFrontInput, rightBackInput;
		auto obstacleAvoidSystem = ObstacleAvoidFuzzySystem::generateFuzzySystem();
		auto wallFollowSystem = WallFollowFuzzySystem::generateFuzzySystem();
		vector<double> outputs;

		while (true)
		{
			for (unsigned int i = 0; i < 8; i++)
			{
				sonarSensors[i] = robot.getSonarReading(i);
			}

			frontLeftInput = min(sonarSensors[2]->getRange(), sonarSensors[3]->getRange());
			frontLeftInput = min(frontLeftInput, sonarSensors[1]->getRange());
			frontCenterInput = min(sonarSensors[3]->getRange(), sonarSensors[4]->getRange());			
			frontRightInput = min(sonarSensors[4]->getRange(), sonarSensors[5]->getRange());
			frontRightInput = min(frontRightInput, sonarSensors[6]->getRange());

			if (frontLeftInput < 450 || frontCenterInput < 450 || frontRightInput < 450)
			{
				vector<double> inputs{ frontLeftInput, frontCenterInput, frontRightInput };
				outputs = obstacleAvoidSystem.execute(inputs);
			}

			else
			{
				rightFrontInput = sonarSensors[6]->getRange();
				rightFrontInput = rightFrontInput * cos(40 * PI / 180);
				rightBackInput = sonarSensors[7]->getRange();
				vector<double> inputs{ rightFrontInput, rightBackInput };
				outputs = wallFollowSystem.execute(inputs);
			}
			
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