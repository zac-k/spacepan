/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
	File f = "C:/Users/zac/Documents/SpacePan/Resource/background.png";
	Image backgroundImage = ImageFileFormat::loadFrom(f);
	//Image backgroundImage = ImageFileFormat::loadFrom(BinaryData);
	Slider mRevMixKnob;
	Slider mPanKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacePanAudioProcessorEditor)
};
