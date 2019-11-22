#include "FuzzySet.h"

FuzzySet::FuzzySet() {}

FuzzySet::FuzzySet(const double membershipFunction[4])
{
	for (unsigned int i = 0; i < 4; i++)
	{
		this->membershipFunction[i] = membershipFunction[i];
	}
}

FuzzySet::~FuzzySet()
= default;

double FuzzySet::computeMembershipValue(double input)
{
	if (input <= membershipFunction[0] || input >= membershipFunction[3])
		return 0.0;
	if (input >= membershipFunction[1] && input <= membershipFunction[2])
		return 1.0;
	if (input < membershipFunction[1])
		return (input - membershipFunction[0]) / (membershipFunction[1] - membershipFunction[0]);
	return (membershipFunction[3] - input) / (membershipFunction[3] - membershipFunction[2]);
}

double FuzzySet::getCenter()
{
	return (this->membershipFunction[1] + this->membershipFunction[2]) / 2;
}
