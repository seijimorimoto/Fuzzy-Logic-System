#pragma once
class FuzzySet
{
public:
	FuzzySet(const double membershipFunction[4]);
	~FuzzySet();
	double computeMembershipValue(double input);
	double membershipFunction[4]{};
};

