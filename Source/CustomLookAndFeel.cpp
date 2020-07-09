/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 5 Jul 2020 2:41:46pm
    Author:  zac

  ==============================================================================
*/

#include "CustomLookAndFeel.h"


CustomLookAndFeel::CustomLookAndFeel()
{
}


CustomLookAndFeel::CustomLookAndFeel(Image spriteImg)
{
	this->sprite = spriteImg;
}

void CustomLookAndFeel::addSprite(Image spriteImg)
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
		const double rotation = std::pow((slider.getValue()
			- slider.getMinimum())
			/ (slider.getMaximum()
				- slider.getMinimum()), slider.getSkewFactor());


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