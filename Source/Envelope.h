/*
  ==============================================================================

    Envelope.h
    Created: 15 Jul 2020 8:23:11am
    Author:  zac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class Envelope
{
public:
	Envelope();

	float getGain();
	void trigger();
	void trigger(float signal);
	void update(float deltaTime);
	void setParams(float attack, float aShape, float decay, float dShape, float sustainGain, float sustain, float release, float rShape, float thresh);
	float getThresh();

private:
	int section{ 0 };
	float tSection{ 0.0f };
	float values[5]{ 0.0f, 0.1f, 0.1f, 0.1f, 0.1f };
	float sustainGain{ 1.0f };
	float shapeFactors[5]{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	float threshold{ 0.5f };
	void incrementSection();


};