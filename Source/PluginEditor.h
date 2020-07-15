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
//#include "utils.h"

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
	ImageComponent adsrPlot;
	Image* pAdsrPlot;
	//Graphics adsrPlot = Graphics(backgroundImage);
	

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
	StandardRotary mDelayLowPassKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayLowPassAttachment;
	StandardRotary mDelayLowPassQKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayLowPassQAttachment;
	StandardRotary mDelayHighPassKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayHighPassAttachment;
	StandardRotary mDelayHighPassQKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayHighPassQAttachment;
	StandardRotary mDelayMixKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayMixAttachment;
	StandardRotary mDelaySatKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelaySatAttachment;
	StandardRotary mDelaySatCharKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelaySatCharAttachment;
	StandardRotary mDelayWidthKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayWidthAttachment;

	
	//Slider mDelayFeedbackKnob;
	
	// Reverb controls
	StandardRotary mRevMixKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevMixAttachment;
	StandardRotary mRoomSizeKnob;
	AudioProcessorValueTreeState::SliderAttachment mRoomSizeAttachment;
	StandardRotary mRevLowPassKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevLowPassAttachment;
	StandardRotary mRevLowPassQKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevLowPassQAttachment;
	StandardRotary mRevHighPassKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevHighPassAttachment;
	StandardRotary mRevSCamountKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevSCamountAttachment;
	StandardRotary mRevHighPassQKnob;
	AudioProcessorValueTreeState::SliderAttachment mRevHighPassQAttachment;
	//Slider mRevMixKnob;

	// Sidechain controls
	StandardRotary mSCattackKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCattackAttachment;
	StandardRotary mSCdecayKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCdecayAttachment;
	StandardRotary mSCsustainLevelKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCsustainLevelAttachment;
	StandardRotary mSCsustainKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCsustainAttachment;
	StandardRotary mSCreleaseKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCreleaseAttachment;
	StandardRotary mSCthresholdKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCthresholdAttachment;


	void addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, float relX, float relY, Image spriteImg, String tooltipText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacePanAudioProcessorEditor)
};
