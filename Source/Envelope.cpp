/*
  ==============================================================================

    Envelope.cpp
    Created: 15 Jul 2020 8:23:11am
    Author:  zac

  ==============================================================================
*/

#include "Envelope.h"


Envelope::Envelope()
{

}

float Envelope::getGain()
{
	float val;
	switch (section)
	{
	case 0:
		val = 0.0f;
		break;
	case 1:
		val = std::pow(tSection / values[1], shapeFactors[1]);
		break;
	case 2:
		val = 1 - std::pow(tSection / values[2], shapeFactors[2]) * (1 - sustainGain);
		break;
	case 3:
		val = sustainGain;
		break;
	case 4:
		val = (1 - std::pow(tSection / values[4], shapeFactors[4])) * sustainGain;
		break;
	}

	return val;

}

void Envelope::setParams(float attack, float aShape, float decay, float dShape, float sustainGain, float sustain, float release, float rShape, float thresh)
{
	this->values[1] = attack;
	this->values[2] = decay;
	this->values[3] = sustain;
	this->values[4] = release;

	this->sustainGain = sustainGain;
	this->shapeFactors[1] = aShape;
	this->shapeFactors[2] = dShape;
	this->shapeFactors[4] = rShape;

	this->threshold = thresh;

}

void Envelope::incrementSection()
{
	this->section += 1;
	this->section %= 5;
	this->tSection = 0.0f;
}

float Envelope::getThresh()
{
	return this->threshold;
}

void Envelope::trigger()
{

	tSection = pow(getGain(), 1.0 / shapeFactors[1]) * values[1];
	section = 1;
}

void Envelope::trigger(float signal)
{
	if (signal > this->threshold)
	{
		this->trigger();
	}
}

void Envelope::update(float deltaTime)
{
	if (section > 0)
	{
		tSection += deltaTime;
		if (tSection >= values[section])
		{
			incrementSection();
		}
	}
}
