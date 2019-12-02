#pragma once
#include <vector>
#include "FuzzySystem.h"

using namespace std;

class HierarchicalFuzzySystem
{
public:
	HierarchicalFuzzySystem(vector<FuzzySystem> behaviourFuzzySystems, vector<FuzzySet> behaviourFuzzySets);
	~HierarchicalFuzzySystem();
	vector<double> execute(vector<vector<double>> &inputs);
private:
	vector<FuzzySystem> behaviourFuzzySystems;
	vector<FuzzySet> behaviourFuzzySets;
};

