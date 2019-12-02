#include <string>
#include "Examples.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc == 1)
		return WallFollowObstacleAvoidContextBlending::main(argc, argv);

	string argument = argv[1];
	
	if (argc == 2 && argument == "FuzzySystemExample")
		return FuzzySystemExample::main();

	if (argc == 2 && argument == "ObstacleAvoid")
		return ObstacleAvoid::main(argc, argv);

	if (argc == 2 && argument == "WallFollow")
		return WallFollow::main(argc, argv);

	if (argc == 2 && argument == "WallFollowObstacleAvoidContextBlending")
		return WallFollowObstacleAvoidContextBlending::main(argc, argv);
}