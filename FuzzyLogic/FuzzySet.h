#pragma once

class FuzzySet
{
public:
	FuzzySet();
	FuzzySet(const double membershipFunction[4]);
	~FuzzySet();
	double computeMembershipValue(double input);
private:
	double membershipFunction[4]{};
};

