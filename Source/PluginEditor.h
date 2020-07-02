/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StandardRotary.h"

//==============================================================================
/**
*/
class SpacePanAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SpacePanAudioProcessorEditor (SpacePanAudioProcessor&);
    ~SpacePanAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SpacePanAudioProcessor& processor;

	// Load background image	
	Image backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, 
													  BinaryData::background_pngSize);

	// Load a knob image for testing
	Image knobImg = ImageCache::getFromMemory(BinaryData::animation_knob_png,
											  BinaryData::animation_knob_pngSize);


	// Global controls
	StandardRotary mPanKnob;
	//Slider mPanKnob;

	// Delay controls
	StandardRotary mDelayFeedbackKnob;
	//Slider mDelayFeedbackKnob;
	
	// Reverb controls
	StandardRotary mRevMixKnob;
	//Slider mRevMixKnob;

	// Sidechain controls


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacePanAudioProcessorEditor)
};
