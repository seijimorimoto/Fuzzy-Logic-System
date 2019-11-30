#include "FuzzySet.h"

// Default constructor of a fuzzy set.
FuzzySet::FuzzySet() {}

// Constructs a fuzzy set with a triangle/trapezoid membership function.
// Params:
// - membershipFunction: Array of four values representing the x-positions of the vertices of the
// trapezoid. If the second and third vertices are equal, then it represents a triangle. 
FuzzySet::FuzzySet(const double membershipFunction[4])
{
	for (unsigned int i = 0; i < 4; i++)
	{
		this->membershipFunction[i] = membershipFunction[i];
	}
}

// Default destructor of a fuzzy set.
FuzzySet::~FuzzySet()
= default;

// Computes the membership value of the fuzzy set given an input.
// Params:
// - input: Data point for which membership value to the fuzzy set is desired.
// Returns: The membership value of the input to the fuzzy set.
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

// Returns the center of the fuzzy set, assuming its membership function is symmetrical.
double FuzzySet::getCenter()
{
	return (this->membershipFunction[1] + this->membershipFunction[2]) / 2;
}
