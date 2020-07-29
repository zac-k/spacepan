/*
  ==============================================================================

    utils.h
    Created: 3 Jul 2020 4:54:28pm
    Author:  zac

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
//
//
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
namespace utils
{


	int modulo(int val1, int val2);


	template <typename T>
	T frequencySkew(T min, T max, T interval = 0.0);

	template <typename T>
	NormalisableRange<T> frequencyRange(T min, T max, T interval = 0.0);

	void openHyperlink(const char *url);

};