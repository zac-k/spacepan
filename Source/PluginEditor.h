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
#include "StandardButton.h"
#include "StandardSwitch.h"
#include "utils.h"

//==============================================================================
/**
*/
class SpacePanAudioProcessorEditor : public AudioProcessorEditor,
	private StandardRotary::Listener,
	private Button::Listener
{
public:
	SpacePanAudioProcessorEditor(SpacePanAudioProcessor&);
	~SpacePanAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;


	void sliderValueChanged(Slider* slider) override;
	//virtual void buttonStateChanged(Button* button) override;
	TextButton debugText;

	//
	void buttonClicked(Button* button) override;
	//void buttonStateChanged(Button* button) override;

	TooltipWindow mToolTipWindow;

	// Parameters
	Label mDelayTimeUnitsText;
	Label mDelayTimeText;

	//TextEditor delayTimeTextBox;


private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SpacePanAudioProcessor& processor;
	Image adsrPlotImage;

	void constructADSRplot();
	// Load all_sprite array
	Image mAllSpriteArray[2] = { ImageCache::getFromMemory(BinaryData::all_sprite_0_png,
													  BinaryData::all_sprite_0_pngSize),
								 ImageCache::getFromMemory(BinaryData::all_sprite_63_png,
													  BinaryData::all_sprite_63_pngSize) };

	// Load background image	
	/*Image backgroundImage = ImageCache::getFromMemory(BinaryData::background_png,
													  BinaryData::background_pngSize);*/
	Image backgroundImage{ mAllSpriteArray[0] };
	Image croGlassImage = ImageCache::getFromMemory(BinaryData::cro_glass_png,
		BinaryData::cro_glass_pngSize);
	ImageComponent adsrPlot;
	Image* pAdsrPlot;

	ImageComponent croGlass;
	ImageComponent timeTextGlass;
	//Graphics adsrPlot = Graphics(backgroundImage);
	

	// Load a knob image for testing
	Image knobImg = ImageCache::getFromMemory(BinaryData::animation_knob_png,
											  BinaryData::animation_knob_pngSize);
	Image subKnobImg = ImageCache::getFromMemory(BinaryData::subknob_png,
		BinaryData::subknob_pngSize);
	Image knobImgPan = ImageCache::getFromMemory(BinaryData::animation_knob2_png,
		BinaryData::animation_knob2_pngSize);
	Image switchImg = ImageCache::getFromMemory(BinaryData::three_pos_switch_png,
		BinaryData::three_pos_switch_pngSize);


	Image sigImg = ImageCache::getFromMemory(BinaryData::sig_png,
		BinaryData::sig_pngSize);
	Image sigDownImg = ImageCache::getFromMemory(BinaryData::sigDown_png,
		BinaryData::sigDown_pngSize);

	ImageButton mSigButton;

	// Load a switch image for testing

	Image delayOnButtonOnImg = ImageCache::getFromMemory(BinaryData::delayOnButtonOn_png,
		BinaryData::delayOnButtonOn_pngSize);
	Image delayOnButtonOffImg = ImageCache::getFromMemory(BinaryData::delayOnButtonOff_png,
		BinaryData::delayOnButtonOff_pngSize);

	Image delayTempoLockButtonImg = ImageCache::getFromMemory(BinaryData::delayTempoLockOnOff_png,
		BinaryData::delayTempoLockOnOff_pngSize);

	//==============================================================================


	float mControlGridX[19] = { 0.115f, 0.18f, 0.23f, 0.28f, 0.35f, 0.42f, 0.6f, 0.7f, 0.8f, 0.9f, 0.22f, 0.26f, 0.31f, 0.35f , 0.4f, 0.44f , 0.49f, 0.53f , 0.58f };
	float mControlGridY[5] = { 0.445f, 0.5f, 0.6f, 0.89f };

	// Global controls
	StandardRotary mPanKnob;
	AudioProcessorValueTreeState::SliderAttachment mPanAttachment;
	// TODO: Head Width slider is for testing, remove or set range from approx. 0.15-0.2
	StandardRotary mHeadWidthSlider;
	AudioProcessorValueTreeState::SliderAttachment mHeadWidthAttachment;


	//==============================================================================

	// Delay controls
	//ImageButton mDelayOnButton;
	StandardRotary mDelayFeedbackKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayFeedbackAttachment;
	StandardRotary mDelayTempoLockButton;
	AudioProcessorValueTreeState::SliderAttachment mDelayTempoLockAttachment;
	StandardRotary mDelayTimeKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayTimeAttachment;
	StandardRotary mDelayDiscreteTimeKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayDiscreteTimeAttachment;
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
	StandardRotary mDelayDiffusionKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelayDiffusionAttachment;
	StandardRotary mDelaySCamountKnob;
	AudioProcessorValueTreeState::SliderAttachment mDelaySCamountAttachment;
	StandardRotary mNoteDotTripSlider;
	AudioProcessorValueTreeState::SliderAttachment mNoteDotTripAttachment;
	StandardRotary mDelayStereoTypeSlider;
	AudioProcessorValueTreeState::SliderAttachment mDelayStereoTypeAttachment;
	StandardRotary mDelayFilterTypeSwitch;
	AudioProcessorValueTreeState::SliderAttachment mDelayFilterTypeAttachment;
	StandardButton mDelayOnOffButton;
	AudioProcessorValueTreeState::ButtonAttachment mDelayOnOffAttachment;
	
	//==============================================================================
	
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
	StandardButton mRevOnOffButton;
	AudioProcessorValueTreeState::ButtonAttachment mRevOnOffAttachment;
	//Slider mRevMixKnob;

	//==============================================================================

	// Sidechain controls
	StandardRotary mSCattackKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCattackAttachment;
	StandardRotary mSCattackShapeKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCattackShapeAttachment;
	StandardRotary mSCdecayKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCdecayAttachment;
	StandardRotary mSCdecayShapeKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCdecayShapeAttachment;
	StandardRotary mSCsustainLevelKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCsustainLevelAttachment;
	StandardRotary mSCsustainKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCsustainAttachment;
	StandardRotary mSCreleaseKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCreleaseAttachment;
	StandardRotary mSCreleaseShapeKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCreleaseShapeAttachment;
	StandardRotary mSCthresholdKnob;
	AudioProcessorValueTreeState::SliderAttachment mSCthresholdAttachment;
	StandardButton mSCOnOffButton;
	AudioProcessorValueTreeState::ButtonAttachment mSCOnOffAttachment;

	//==============================================================================

	String mDelayModifierChar{ "TDN" };

	Typeface::Ptr BPdotsTypeface = Typeface::createSystemTypefaceFor(BinaryData::BPdots_otf, BinaryData::BPdots_otfSize);

	void addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, float relX, float relY, Image spriteImg, String tooltipText, Slider::SliderStyle = Slider::SliderStyle::RotaryVerticalDrag);
	void addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, int gridPosX, int gridPosY, Image spriteImg, String tooltipText, Slider::SliderStyle = Slider::SliderStyle::RotaryVerticalDrag);

	void addControl(SpacePanAudioProcessorEditor *const editor, StandardButton &control, String name, float relX, float relY, Image spriteImg, String tooltipText, Slider::SliderStyle = Slider::SliderStyle::RotaryVerticalDrag);

	void addControl(SpacePanAudioProcessorEditor *const editor, StandardButton &control, String name, float relX, float relY, float relW, float relH, String tooltipText, Slider::SliderStyle = Slider::SliderStyle::RotaryVerticalDrag);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpacePanAudioProcessorEditor)
};
