
#pragma once

#include "StandardRotary.h"


//StandardRotary::StandardRotary() {}
		

void StandardRotary::init(String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg, Slider::SliderStyle) 
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

void StandardRotary::reverse()
{
	mReverse = !mReverse;
}


void StandardRotary::setDim(int width, int height)
{
	this->width = width;
	this->height = height;
}

void StandardRotary::sendToDisplay(Label &dataLabel, Label &unitsLabel, float multiplier)
{
	
	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) << this->getValue() * multiplier;
	String text = ss.str();
	dataLabel.setText(text, NotificationType::sendNotification);
	unitsLabel.setText("ms", NotificationType::dontSendNotification);
	
}

void StandardRotary::sendToDisplay(Label &dataLabel, Label &unitsLabel, std::vector<String> names, String modifier)
{
	
	String text = names[this->getValue()];
	dataLabel.setText(text+modifier, NotificationType::sendNotification);
	unitsLabel.setText("bars", NotificationType::dontSendNotification);
	
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

