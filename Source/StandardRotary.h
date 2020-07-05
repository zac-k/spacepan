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
	void init(String name, float min, float max, float value, int x, int y, Image spriteImg);
	void draw(Graphics& g);

private:

	int width = 65;
	int height = 65;
	int xpos;
	int ypos;
	Image sprite;
	CustomLookAndFeel mLookAndFeel;

};


