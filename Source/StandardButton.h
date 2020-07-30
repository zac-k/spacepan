/*
  ==============================================================================

    StandardButton.h
    Created: 20 Jul 2020 9:44:41am
    Author:  zac

  ==============================================================================
*/

#pragma once
#include "juceheader.h"

class StandardButton : public ImageButton
{
	// Constructor
	using ImageButton::ImageButton;
public:
	void setOnOffImages(Image spriteArray[2]);
	void setOnOffImages(Image onImage, Image offImage);
	void setOnOffImages(Image onOffImage, bool isVertical=true);
	void displayAsOn(bool isOn);

private:
	Image mOnImage;
	Image mOffImage;
};