/*
  ==============================================================================

    utils.h
    Created: 3 Jul 2020 4:54:28pm
    Author:  zac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace utils
{
	// Wraps val into the range [0, divisor), including for negative values
	inline int modulo(int val, int divisor)
	{
		int mod = val % divisor;
		return mod < 0 ? mod + divisor : mod;
	}

	// Range with a skew that spreads audible frequencies evenly over a knob
	template <typename T>
	NormalisableRange<T> frequencyRange(T min, T max, T interval = 0)
	{
		return { min, max, interval, (T)1 / std::log2((T)1 + std::sqrt(max / min)) };
	}

	inline void openHyperlink(const String& url)
	{
		URL(url).launchInDefaultBrowser();
	}
}
