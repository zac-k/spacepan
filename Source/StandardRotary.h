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
	void init(String name, juce::Rectangle<int> parentBounds, float relX, float relY, Image spriteImg, Slider::SliderStyle = Slider::SliderStyle::RotaryVerticalDrag);
	void setDim(int width, int height);
	void sendToDisplay(Label &dataLabel, Label &unitsLabel, float multiplier=1.0f);
	void sendToDisplay(Label &dataLabel, Label &unitsLabel, std::vector<String>, String modifier="");
	void reverse();

private:

	int width = 64;
	int height = 64;
	int xpos;
	int ypos;
	Image sprite;
	CustomLookAndFeel mLookAndFeel;
	bool mReverse{ false };

};


