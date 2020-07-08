/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SpacePanAudioProcessorEditor::SpacePanAudioProcessorEditor(SpacePanAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	mRevMixAttachment(p.mState, "rev_mix", mRevMixKnob),
	mPanAttachment(p.mState, "pan", mPanKnob),
	mHeadWidthAttachment(p.mState, "head_width", mHeadWidthSlider),
	mDelayFeedbackAttachment(p.mState, "delay_feedback", mDelayFeedbackKnob),
	mDelayTimeAttachment(p.mState, "delay_time", mDelayTimeKnob),
	mDelayLowPassAttachment(p.mState, "delay_lowpass", mDelayLowPassKnob),
	mDelayLowPassQAttachment(p.mState, "delay_lowpass_Q", mDelayLowPassQKnob),
	mDelayHighPassAttachment(p.mState, "delay_highpass", mDelayHighPassKnob),
	mDelayHighPassQAttachment(p.mState, "delay_highpass_Q", mDelayHighPassQKnob),
	mDelayMixAttachment(p.mState, "delay_mix", mDelayMixKnob),
	mDelayWidthAttachment(p.mState, "delay_width", mDelayWidthKnob)
	
{
    setSize (852, 600);

	// Debug textbox
	/*debugText.setBounds(0, 0, 100, 50);
	addAndMakeVisible(debugText);
	debugText.setText("test");*/
	//debugText.setText((String)(utils::modulo(-5, 7)));

	/*mHeadWidthSlider.setRange(0.0, 10.0);
	mHeadWidthSlider.setValue(0.15);
	mHeadWidthSlider.setBounds(600, 200, 20, 100);
	//mHeadWidthSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);*/
	


	// Initialise knobs
	mRevMixKnob.init( "RevMixKnob", 0.0f, 1.0f, 0.5f, getBounds(), 0.5, 0.5, knobImg);
	mPanKnob.init("PanKnob", -1.0f, 1.0f, 0.0f, getBounds(), 0.5, 0.15, knobImgPan);
	mHeadWidthSlider.init("HeadWidthKnob", 0.0f, 10.0f, 0.15f, getBounds(), 0.6, 0.15, knobImgPan);
	mDelayFeedbackKnob.init("DelayFeedbackKnob", 0.0f, 1.0f, 0.5f, getBounds(), 0.25, 0.5, knobImg);
	mDelayTimeKnob.init("DelayTimeKnob", 0.0f, 1.0f, 0.5f, getBounds(), 0.15, 0.5, knobImg);

	mDelayLowPassKnob.init("DelayLowPassKnob", 100.0f, 2.0e4f, 2.0e3f, getBounds(), 0.33, 0.5, knobImgPan);
	mDelayLowPassQKnob.setDim(32, 32);
	mDelayLowPassQKnob.init("DelayLowPassQKnob", 1.0f, 5.0f, 1.0f, getBounds(), 0.33, 0.5, knobImg);

	mDelayHighPassKnob.init("DelayHighPassKnob", 100.0f, 2.0e4f, 2.0e2f, getBounds(), 0.4, 0.5, knobImgPan);
	mDelayHighPassQKnob.setDim(32, 32);
	mDelayHighPassQKnob.init("DelayHighPassQKnob", 1.0f, 5.0f, 1.0f, getBounds(), 0.4, 0.5, knobImg);
	
	mDelayTimeText.setBounds(0, 0, 80, 20);
	//mDelayTimeKnob.hideTextBox(false);
	//mDelayTimeKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
	mDelayMixKnob.init("DelayMixKnob", 0.0f, 1.0f, 0.5f, getBounds(), 0.11, 0.4, knobImg);
	mDelayWidthKnob.init("DelayWidthKnob", 0.0f, 1.0f, 0.5f, getBounds(), 0.15, 0.6, knobImg);


	// Buttons
	//mDelayOnButton.setBounds(100, 100, 20, 80);
	//mDelayOnButton.setImages(false, false, true,);
	//addAndMakeVisible(mDelayOnButton);


	// TODO: finish adding tooltips
	// Set tooltips
	mRevMixKnob.setTooltip("Mix");
	mPanKnob.setTooltip("Pan");
	mDelayFeedbackKnob.setTooltip("Feedback amount");
	mDelayTimeKnob.setTooltip("Delay time");
	mDelayMixKnob.setTooltip("Mix");
	mDelayWidthKnob.setTooltip("Stereo width");

	mRevMixKnob.addListener(this);
	mPanKnob.addListener(this);
	mDelayFeedbackKnob.addListener(this);
	mDelayTimeKnob.addListener(this);
	mDelayMixKnob.addListener(this);
	mDelayWidthKnob.addListener(this);

	
	

	// Add knobs to GUI
	addAndMakeVisible(mRevMixKnob);
	addAndMakeVisible(mPanKnob);
	addAndMakeVisible(mHeadWidthSlider);
	addAndMakeVisible(mDelayFeedbackKnob);
	addAndMakeVisible(mDelayTimeKnob);
	addAndMakeVisible(mDelayLowPassKnob);
	addAndMakeVisible(mDelayLowPassQKnob);
	addAndMakeVisible(mDelayHighPassKnob);
	addAndMakeVisible(mDelayHighPassQKnob);
	addAndMakeVisible(mDelayMixKnob);
	addAndMakeVisible(mDelayWidthKnob);

	addAndMakeVisible(mDelayTimeText);




}



SpacePanAudioProcessorEditor::~SpacePanAudioProcessorEditor()
{
}

//template<Slider T>
//void SpacePanAudioProcessorEditor::mouseMove(const MouseEvent &)
//{
//	T::mouseMove(const MouseEvent &)
//}

void SpacePanAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	
}

//==============================================================================
void SpacePanAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
	g.drawImageAt(backgroundImage, 0, 0);
}

void SpacePanAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//void Slider::mouseEnter(MouseEvent&)
