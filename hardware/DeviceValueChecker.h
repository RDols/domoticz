//
#pragma once

#include <vector>

struct SValueCheckerItem
{
	time_t mTimestamp = 0;
	double mValue = 0.00;

	bool operator<(const SValueCheckerItem& other)
	{
		return mValue < other.mValue;
	};
};


class CValueChecker
{
public:
	CValueChecker();
	
	bool ValidateValue(const double& value, const double& maxRate);

protected:
	SValueCheckerItem& FindOldest();
	bool AddValue(const double& value);

private:
	std::vector<SValueCheckerItem> mValues;
	const int32_t mMedianPos = 4;
	SValueCheckerItem mLastEntry;
};
