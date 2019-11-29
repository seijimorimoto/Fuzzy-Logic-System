#include <string>
#include "Examples.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc == 1)
		return WallFollowObstacleAvoidRobot::main(argc, argv);

	string argument = argv[1];
	
	if (argc == 2 && argument == "FuzzySystemExample")
		return FuzzySystemExample::main();
}