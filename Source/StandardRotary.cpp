
#pragma once

#include "StandardRotary.h"


//StandardRotary::StandardRotary() {}
		

void StandardRotary::init(String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg) 
{
		

	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	//Listener* pListener;
	mLookAndFeel.addSprite(spriteImg);
	//this->setRange(min, max);
	//this->setValue(value);
	//this->setAlpha(0.0); // This is what hides the default knob.
	this->setName(name);
	//this->addListener(pListener);
	this->setLookAndFeel(&mLookAndFeel);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	//addAndMakeVisible(*this);

	this->xpos = (int)parentBounds.getWidth()*relX - this->width / 2;
	this->ypos = (int)parentBounds.getHeight()*relY - this->height / 2;
	this->setBounds(this->xpos, this->ypos, this->width, this->height);
	this->sprite = spriteImg;
	//this->setLookAndFeel(&thisLookAndFeel);
/*
	*/

/**/

	return;
}

void StandardRotary::init(String name, NormalisableRange<double> range, float value, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg)
{


	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mLookAndFeel.addSprite(spriteImg);
	this->setNormalisableRange(range);
	this->setValue(value);
	this->setName(name);
	this->setLookAndFeel(&mLookAndFeel);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	this->xpos = (int)parentBounds.getWidth()*relX - this->width/2;
	this->ypos = (int)parentBounds.getHeight()*relY - this->height/2;
	this->setBounds(this->xpos, this->ypos, this->width, this->height);
	
	this->sprite = spriteImg;
	//this->setLookAndFeel(&thisLookAndFeel);
/*
	*/

	/**/

	return;
}

void StandardRotary::setDim(int width, int height)
{
	this->width = width;
	this->height = height;
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

