/*
  ==============================================================================

    DiscreteParam.cpp
    Created: 20 Jul 2020 4:16:20pm
    Author:  zac

  ==============================================================================
*/

#include "DiscreteParam.h"
//template<typename T>
//DiscreteParam<T>::DiscreteParam(int length)
//{
//	this->vals = T[length];
//	this->names = String[length];
//
//}

//template<typename T>
//DiscreteParam<T>::DiscreteParam(vector<T> vals, vector<String> names)
//{
//	this->mVals = vals;
//	this->mNames = names;
//
//}

template<class T>
DiscreteParam<T>::DiscreteParam(T vals[], String names[], int numParams) 
{
	this->mNumParams = numParams;// sizeof(vals) / sizeof(vals[0]);// std::end(names) - std::begin(names);
	this->mVals = std::vector<T>(vals, vals + mNumParams);
	this->mNames = std::vector<String>(names, names + mNumParams);
}

template<class T>
int DiscreteParam<T>::getNumParams()
{
	return this->mNumParams;// (int)(mVals.size());

}

template<class T>
std::vector<String> DiscreteParam<T>::getNames()
{
	return mNames;
}

template<class T>
std::vector<T> DiscreteParam<T>::getValues()
{
	return mVals;
}

template<class T>
auto DiscreteParam<T>::labelsLambda() 
{
	return [this](int val, int) {return this->mNames[val]; };
}

