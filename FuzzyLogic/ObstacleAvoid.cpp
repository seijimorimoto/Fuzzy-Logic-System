#include <iostream>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySystem.h"
#include "ObstacleAvoidFuzzySystem.h"

using namespace std;

namespace ObstacleAvoid
{
	int main(int argc, char * argv[])
	{
		cout << "OBSTACLE AVOIDANCE EXAMPLE" << endl;
		
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
		auto fuzzySystem = ObstacleAvoidFuzzySystem::generateFuzzySystem();

		// Main loop of the program. Keep it running indefinitely.
		while (true)
		{
			// Get the readings from the sonar sensors of the robot.
			for (unsigned int i = 0; i < 8; i++)
			{
				sonarSensors[i] = robot.getSonarReading(i);
			}

			// Calculate the front-left, front and front-right distances to the objects closest to
			// the robot by using the readings from the sensors.
			frontLeftInput = min(sonarSensors[2]->getRange(), sonarSensors[3]->getRange());
			frontCenterInput = min(sonarSensors[3]->getRange(), sonarSensors[4]->getRange());
			frontRightInput = min(sonarSensors[4]->getRange(), sonarSensors[5]->getRange());

			// Pass the calculated and retrieved distances to the obstacle avoidance fuzzy system.
			vector<double> inputs{ frontLeftInput, frontCenterInput, frontRightInput };
			auto outputs = fuzzySystem.execute(inputs);

			// Print the inputs gotten from the sensor readings and the outputs returned by the
			// fuzzy system.
			cout << "Inputs: " << inputs[0] << " " << inputs[1] << " " << inputs[2] << endl;
			cout << "Outputs: " << outputs[0] << " " << outputs[1] << endl;

			// Set the outputs of the fuzzy system as the speed of the left and right wheel of the
			// robot.
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