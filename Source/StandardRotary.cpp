
#pragma once

#include "StandardRotary.h"


//StandardRotary::StandardRotary() {}
		

void StandardRotary::init(String name, float min, float max, float value, int x, int y, Image spriteImg) 
{
		

	this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	//Listener* pListener;

	CustomLookAndFeel thisLookAndFeel(spriteImg);
	this->setRange(min, max);
	this->setValue(value);
	this->setAlpha(0.0);
	this->setName(name);
	//this->addListener(pListener);

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

void StandardRotary::draw(Graphics& g)
{



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
	/*	*/


}



CustomLookAndFeel::CustomLookAndFeel(Image spriteImg)
{
	this->sprite = spriteImg;
}

//==============================================================================
void CustomLookAndFeel::drawRotarySlider(Graphics& g,
	int x, int y, int width, int height, float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{

	if (sprite.isValid())
	{
		const double rotation = (slider.getValue()
			- slider.getMinimum())
			/ (slider.getMaximum()
				- slider.getMinimum());

		const int frames = sprite.getHeight() / sprite.getWidth();
		const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
		const float radius = jmin(width / 2.0f, height / 2.0f);
		const float centerX = x + width * 0.5f;
		const float centerY = y + height * 0.5f;
		const float rx = centerX - radius - 1.0f;
		const float ry = centerY - radius;

		g.drawImage(sprite,
			(int)rx,
			(int)ry,
			2 * (int)radius,
			2 * (int)radius,
			0,
			frameId*sprite.getWidth(),
			sprite.getWidth(),
			sprite.getWidth());
	}
	else
	{
		static const float textPpercent = 0.35f;
		Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f,
			0.5f * height, width * textPpercent, 0.5f * height);

		g.setColour(Colours::white);

		g.drawFittedText(String("No Image"), text_bounds.getSmallestIntegerContainer(),
			Justification::horizontallyCentred | Justification::centred, 1);
	}
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

