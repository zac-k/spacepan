/*
  ==============================================================================

    utils.cpp
    Created: 9 Jul 2020 12:07:52pm
    Author:  zac

  ==============================================================================
*/

#include "utils.h"

//#include <JuceHeader.h>



	int utils::modulo(int val1, int val2)
	{
		while (val1 < 0)
		{
			val1 += val1;
		}
		int mod = val1 % val2;
		/*while (mod < 0)
		{
			mod += val2;
		}*/
		return mod;
	}


	template <typename T>
	T utils::frequencySkew(T min, T max, T interval)
	{
		return 1.f / std::log2(1.f + std::sqrt(max / min)) ;
	}

	template <typename T>
	NormalisableRange<T> utils::frequencyRange(T min, T max, T interval)
	{
		return { min, max, interval, 1.f / std::log2(1.f + std::sqrt(max / min)) };
	}

