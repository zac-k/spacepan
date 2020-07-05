/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 5 Jul 2020 2:41:46pm
    Author:  zac

  ==============================================================================
*/

#pragma once

#include "juceheader.h"

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
	CustomLookAndFeel();

	CustomLookAndFeel(Image spriteImg);

	void addSprite(Image spriteImg);

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
	Image sprite;

};