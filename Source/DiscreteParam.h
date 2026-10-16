/*
  ==============================================================================

    DiscreteParam.h
    Created: 20 Jul 2020 4:16:20pm
    Author:  zac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>

// A fixed list of values with display names, for use with AudioParameterInt
template <class T>
class DiscreteParam
{
public:
	DiscreteParam(const T vals[], const String names[], int numParams)
		: mVals(vals, vals + numParams), mNames(names, names + numParams), mNumParams(numParams)
	{
	}

	int getNumParams() const { return mNumParams; }
	const std::vector<String>& getNames() const { return mNames; }
	const std::vector<T>& getValues() const { return mVals; }

	// Returns the value at index, clamped to the valid range
	T getValue(int index) const { return mVals[(size_t)jlimit(0, mNumParams - 1, index)]; }

	std::function<String(int, int)> labelsLambda() const
	{
		return [this](int val, int) { return mNames[(size_t)jlimit(0, mNumParams - 1, val)]; };
	}

private:
	std::vector<T> mVals;
	std::vector<String> mNames;
	int mNumParams;
};
