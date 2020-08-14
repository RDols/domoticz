//
#include "stdafx.h"
#include "DeviceValueChecker.h"

#include <algorithm>
#include <string>

CValueChecker::CValueChecker()
{
	mValues.resize( (mMedianPos*2)+1 );
}

bool CValueChecker::ValidateValue(const double& value, const double& maxRate)
{
	if (false == AddValue(value))
		return false;

	auto& median = mValues[mMedianPos];

	double deltaT = static_cast<double>(mLastEntry.mTimestamp - median.mTimestamp);
	double deltaV = mLastEntry.mValue - median.mValue;
	double rate = (3600 * deltaV) / deltaT;

	if (abs(rate) > maxRate)
	{
		_log.Log(LOG_NORM, "Rejected value. Rate is above maximum.  rate:%f   maxRate:%f", rate, maxRate);
		return false;
	}

	return true;
}


SValueCheckerItem& CValueChecker::FindOldest()
{
	auto& oldest = *mValues.begin();

	for (auto& it : mValues)
	{
		if (it.mTimestamp < oldest.mTimestamp)
		{
			oldest = it;
		}
	}

	return oldest;
}


bool CValueChecker::AddValue(const double& value)
{
	time_t currentTimestamp = mytime(nullptr);

	if (currentTimestamp < mLastEntry.mTimestamp + 5)
	{
		_log.Log(LOG_NORM, "Rejected value. Received a value withing 5 seconds of previous.");
		return false;
	}

	auto& replaced = FindOldest();
	bool fullInitilized = replaced.mTimestamp > 0;

	replaced.mTimestamp = currentTimestamp;
	replaced.mValue = value;

	mLastEntry = replaced;

	std::sort(mValues.begin(), mValues.end());
	return fullInitilized;
}
