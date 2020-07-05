
#pragma once

#include "StandardRotary.h"


//StandardRotary::StandardRotary() {}
		

void StandardRotary::init(String name, float min, float max, float value, int x, int y, Image spriteImg) 
{
		

	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	//Listener* pListener;
	mLookAndFeel.addSprite(spriteImg);
	this->setRange(min, max);
	this->setValue(value);
	//this->setAlpha(0.0); // This is what hides the default knob.
	this->setName(name);
	//this->addListener(pListener);
	this->setLookAndFeel(&mLookAndFeel);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	//addAndMakeVisible(*this);
	this->setBounds(x, y, this->width, this->height);
	this->xpos = x;
	this->ypos = y;
	this->sprite = spriteImg;
	//this->setLookAndFeel(&thisLookAndFeel);
/*
	*/

/**/

	return;
}





/*	void StandardRotary::init(float min, float max, float value, int xpos, int ypos, Image sprite)
{
	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	this->setRange(min, max);
	this->setValue(value);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	addAndMakeVisible(this);
	this->setBounds(xpos, ypos, 50, 50);

}*/

