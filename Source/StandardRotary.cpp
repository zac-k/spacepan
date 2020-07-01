#include "juceheader.h"

class StandardRotary : public Slider
{
	public:
		StandardRotary::StandardRotary() {}

		void StandardRotary::init(float min, float max, float value, int xpos, int ypos, Image sprite) 
		{
			int width = 50;
			int height = 50;

			this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
			this->setRange(min, max);
			this->setValue(value);
			this->hideTextBox(true);
			this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
			//addAndMakeVisible(*this);
			this->setBounds(xpos, ypos, width, height);
/*
			*/

		/*

			const double rotation = (this->getValue()
				- this->getMinimum())
				/ (this->getMaximum()
					- this->getMinimum());

			const int frames = sprite.getHeight() / sprite.getWidth();
			const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
			const float radius = jmin(width / 2.0f, height / 2.0f);
			const float centerX = xpos + width * 0.5f;
			const float centerY = ypos + height * 0.5f;
			const float rx = centerX - radius - 1.0f;
			const float ry = centerY - radius;*/

			return;
		}

		/*void StandardRotary::draw(Graphics& g)
		{

			g.drawImage(sprite,
				(int)rx,
				(int)ry,
				2 * (int)radius,
				2 * (int)radius,
				0,
				frameId*sprite.getWidth(),
				sprite.getWidth(),
				sprite.getWidth());



		}*/

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

};