/*
  ==============================================================================

    StandardSwitch.cpp
    Created: 22 Jul 2020 8:56:38am
    Author:  zac

  ==============================================================================
*/
#pragma once
#include "StandardSwitch.h"

void StandardSwitch::init(String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg)
{


	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mLookAndFeel.addSprite(spriteImg);
	this->setName(name);
	this->setLookAndFeel(&mLookAndFeel);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	this->xpos = (int)parentBounds.getWidth()*relX - this->width / 2;
	this->ypos = (int)parentBounds.getHeight()*relY - this->height / 2;
	this->setBounds(this->xpos, this->ypos, this->width, this->height);
	this->sprite = spriteImg;

	return;
}