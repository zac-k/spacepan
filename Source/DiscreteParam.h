/*
  ==============================================================================

    DiscreteParam.h
    Created: 20 Jul 2020 4:16:20pm
    Author:  zac

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <vector>
//using namespace std;

template <class T>
class DiscreteParam
{
public:
	//DiscreteParam(int length) {};
	//DiscreteParam(vector<T> vals, vector<String> names) {};
	DiscreteParam<T>(T vals[], String names[], int numParams);

	int getNumParams();

	auto labelsLambda();
private:
	std::vector<T> mVals;
	std::vector<String> mNames;
int mNumParams;
	
};
	