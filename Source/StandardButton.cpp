/*
  ==============================================================================

    StandardButton.cpp
    Created: 20 Jul 2020 9:44:41am
    Author:  zac

  ==============================================================================
*/

#include "StandardButton.h"

void StandardButton::setOnOffImages(Image onImage, Image offImage)
{
	this->mOnImage = onImage;
	this->mOffImage = offImage;
}

void StandardButton::setOnOffImages(Image onOffImage, bool isVertical)
{
	// TODO: implement isVertical == false
	Rectangle<int> onArea = Rectangle<int>(0, 0, onOffImage.getWidth(), onOffImage.getHeight() / 2);
	Rectangle<int> offArea = Rectangle<int>(0, onOffImage.getHeight() / 2 - 1, onOffImage.getWidth(), onOffImage.getHeight() / 2);
	this->mOnImage = onOffImage.getClippedImage(onArea);
	this->mOffImage = onOffImage.getClippedImage(offArea);
}

void StandardButton::displayAsOn(bool isOn)
{
	Image *pImage;
	if (isOn)
	{
		pImage = &(this->mOnImage);
	}
	else
	{
		pImage = &(this->mOffImage);
	}

	this->setImages(true, true, true, *pImage,
		1.0f, Colours::transparentBlack, *pImage, 1.0f, Colours::transparentBlack,
		*pImage, 1.0f, Colours::transparentBlack);
	
}
