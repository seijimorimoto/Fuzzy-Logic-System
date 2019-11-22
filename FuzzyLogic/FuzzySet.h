#pragma once

class FuzzySet
{
public:
	FuzzySet();
	FuzzySet(const double membershipFunction[4]);
	~FuzzySet();
	double computeMembershipValue(double input);
	double getCenter();
private:
	double membershipFunction[4]{};
};

