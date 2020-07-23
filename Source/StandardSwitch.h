/*
  ==============================================================================

    StandardSwitch.h
    Created: 22 Jul 2020 8:56:38am
    Author:  zac

  ==============================================================================
*/

#pragma once

#include "juceheader.h"
#include "CustomLookAndFeel.h"

class StandardSwitch : public Slider
{
	// Constructor
	using Slider::Slider;
public:
	void init(String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg);
	//void setOnOffImages(Image onImage, Image offImage);
	//void setOnOffImages(Image onOffImage, bool isVertical = true);
	//void displayAsOn(bool isOn);

private:

	int width = 64;
	int height = 64;
	int xpos;
	int ypos;
	Image sprite;
	CustomLookAndFeel mLookAndFeel;
};
