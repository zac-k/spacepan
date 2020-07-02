
#pragma once

#include "StandardRotary.h"


//StandardRotary::StandardRotary() {}
		

void StandardRotary::init(float min, float max, float value, int x, int y, Image spriteImg) 
{
			

	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	this->setRange(min, max);
	this->setValue(value);
	this->hideTextBox(true);
	this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	//addAndMakeVisible(*this);
	this->setBounds(x, y, this->width, this->height);
	this->xpos = x;
	this->ypos = y;
	//this->sprite = spriteImg;
/*
	*/

/**/

	return;
}

void StandardRotary::draw(Graphics& g)
{

/*

	const double rotation = (this->getValue()
		- this->getMinimum())
		/ (this->getMaximum()
			- this->getMinimum());

	const int frames = this->sprite.getHeight() / this->sprite.getWidth();
	const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
	const float radius = jmin(this->width / 2.0f, this->height / 2.0f);
	const float centerX = this->xpos + this->width * 0.5f;
	const float centerY = this->ypos + this->height * 0.5f;
	const float rx = centerX - radius - 1.0f;
	const float ry = centerY - radius;


			
	g.drawImage((this->sprite),
		(int)rx,
		(int)ry,
		2 * (int)radius,
		2 * (int)radius,
		0,
		frameId*this->sprite.getWidth(),
		this->sprite.getWidth(),
		this->sprite.getWidth());
		*/


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

