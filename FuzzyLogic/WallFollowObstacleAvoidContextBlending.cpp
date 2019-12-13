#include <iostream>
#include "Aria.h"
#include "Examples.h"
#include "FuzzySet.h"
#include "FuzzySystem.h"
#include "HierarchicalFuzzySystem.h"
#include "ObstacleAvoidFuzzySystem.h"
#include "WallFollowFuzzySystem.h"
#define PI 3.14159265

using namespace std;

namespace WallFollowObstacleAvoidContextBlending
{
	HierarchicalFuzzySystem generateHierarchicalFuzzySystem();

	int main(int argc, char * argv[])
	{
		cout << "CONTEXT BLENDING EXAMPLE" << endl;
		
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
		auto hierarchicalFuzzySystem = generateHierarchicalFuzzySystem();

		// Main loop of the program. Keep it running indefinitely.
		while (true)
		{
			// Get the readings from the sonar sensors of the robot.
			for (unsigned int i = 0; i < 8; i++)
			{
				sonarSensors[i] = robot.getSonarReading(i);
			}

			// Calculate the front-left, front, front-right, right-front and right-back distances
			// to the objects closest to the robot by using the readings from the sensors.
			frontLeftInput = min(sonarSensors[2]->getRange(), sonarSensors[3]->getRange());
			frontLeftInput = min(frontLeftInput, sonarSensors[1]->getRange());
			frontCenterInput = min(sonarSensors[3]->getRange(), sonarSensors[4]->getRange());
			frontRightInput = min(sonarSensors[4]->getRange(), sonarSensors[5]->getRange());
			frontRightInput = min(frontRightInput, sonarSensors[6]->getRange());
			rightFrontInput = sonarSensors[6]->getRange();
			rightFrontInput = rightFrontInput * cos(40 * PI / 180);
			rightBackInput = sonarSensors[7]->getRange();

			// Separate the readings into two groups of inputs, the ones related to the front
			// readings and the ones related to the right readings.
			vector<vector<double>> inputs{
				vector<double>{ frontLeftInput, frontCenterInput, frontRightInput },
				vector<double>{ rightFrontInput, rightBackInput }
			};
			
			// Pass the vector containing all groups of inputs to the hierarchical fuzzy system.
			// Then print the returned output values.
			auto outputs = hierarchicalFuzzySystem.execute(inputs);
			cout << "Outputs: " << outputs[0] << " " << outputs[1] << endl;

			// Set the outputs of the hierarchical fuzzy system as the speed of the left and right
			// wheel of the robot.
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

	// Generates and returns a hierachical fuzzy system combining an obstacle-avoidance and a wall
	// following behaviour.
	HierarchicalFuzzySystem generateHierarchicalFuzzySystem()
	{
		// Setting up the membership functions for each behaviour to be used in the hierarchical
		// fuzzy system.
		double obstacleAvoidFunc[] = { 0, 0, 300, 700 };
		double wallFollowFunc[] = { 300, 700, 7000, 7000 };

		// Create the fuzzy sets for encapsulating the defined membership functions.
		FuzzySet obstacleAvoid(obstacleAvoidFunc);
		FuzzySet wallFollow(wallFollowFunc);
		vector<FuzzySet> behaviourFuzzySets{ obstacleAvoid, wallFollowFunc };

		// Generate the obstacle avoidance and the wall following fuzzy systems.
		auto obstacleAvoidSystem = ObstacleAvoidFuzzySystem::generateFuzzySystem();
		auto wallFollowSystem = WallFollowFuzzySystem::generateFuzzySystem();
		vector<FuzzySystem> fuzzySystems{ obstacleAvoidSystem, wallFollowSystem };
		
		// Construct and return a hierarchical fuzzy system built from the passed-in fuzzy systems
		// and using the passed-in fuzzy sets to apply them over the results of the fuzzy systems.
		return HierarchicalFuzzySystem(fuzzySystems, behaviourFuzzySets);
	}
}