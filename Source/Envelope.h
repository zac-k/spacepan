/*
  ==============================================================================

    Envelope.h
    Created: 15 Jul 2020 8:23:11am
    Author:  zac

  ==============================================================================
*/

#pragma once
#include "juceheader.h"


class Envelope
{
public:
	Envelope();

	float getGain();
	void trigger();
	void update(float deltaTime);
	void setParams(float attack, float aShape, float decay, float dShape, float sustainGain, float sustain, float release, float rShape, float thresh);
	float getThresh();

private:
	int section;
	float tSection{ 0.0f };
	float values[5];
	float sustainGain;
	float shapeFactors[5];
	float threshold;
	void incrementSection();


};