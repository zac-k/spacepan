/*
  ==============================================================================

    StandardRotary.h
    Created: 2 Jul 2020 12:04:29pm
    Author:  zac

  ==============================================================================
*/

#pragma once

#include "juceheader.h"
#include "CustomLookAndFeel.h"


class StandardRotary : public Slider
{


	// Constructor
	using Slider::Slider;
public:

	// Public methods
	void init(String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg);
	void init(String name, NormalisableRange<double> range, float value, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg);
	void setDim(int width, int height);

private:

	int width = 64;
	int height = 64;
	int xpos;
	int ypos;
	Image sprite;
	CustomLookAndFeel mLookAndFeel;

};


