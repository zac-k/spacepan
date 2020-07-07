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
class SpacePanAudioProcessorEditor  : public AudioProcessorEditor,
									  private StandardRotary::Listener
{
public:
    SpacePanAudioProcessorEditor (SpacePanAudioProcessor&);
    ~SpacePanAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;


	void sliderValueChanged(Slider* slider) override;

	//TextEditor debugText;
	//

	TooltipWindow mToolTipWindow;

	// Parameters

	Label mDelayTimeText;

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
	Image knobImgPan = ImageCache::getFromMemory(BinaryData::animation_knob2_png,
		BinaryData::animation_knob2_pngSize);

	// Load a switch image for testing

	Image delayOnButtonOnImg = ImageCache::getFromMemory(BinaryData::delayOnButtonOn_png,
		BinaryData::delayOnButtonOn_pngSize);
	Image delayOnButtonOffImg = ImageCache::getFromMemory(BinaryData::delayOnButtonOff_png,
		BinaryData::delayOnButtonOff_pngSize);


	// Global controls
	StandardRotary mPanKnob;
	AudioProcessorValueTreeState::SliderAttachment mPanAttachment;
	// TODO: Change the head width slider to a linear one
	StandardRotary mHeadWidthSlider;
	AudioProcessorValueTreeState::SliderAttachment mHeadWidthAttachment;

	// Delay controls
	//ImageButton mDelayOnButton;
	StandardRotary mDelayFeedbackKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayFeedbackAttachment;
	StandardRotary mDelayTimeKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayTimeAttachment;
	StandardRotary mDelayMixKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayMixAttachment;
	StandardRotary mDelayWidthKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayWidthAttachment;

	
	//Slider mDelayFeedbackKnob;
	
	// Reverb controls
	StandardRotary mRevMixKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevMixAttachment;
	//Slider mRevMixKnob;

	// Sidechain controls


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacePanAudioProcessorEditor)
};
