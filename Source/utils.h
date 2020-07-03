/*
  ==============================================================================

    utils.h
    Created: 3 Jul 2020 4:54:28pm
    Author:  zac

  ==============================================================================
*/

#pragma once


namespace utils
{

	int modulo(int val1, int val2)
	{
		int mod = val1 % val2;
		if (mod < 0)
		{
			mod += val2;
		}
		return mod;
	}

}