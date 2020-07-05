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
	mDelayFeedbackAttachment(p.mState, "delay_feedback", mDelayFeedbackKnob),
	mDelayTimeAttachment(p.mState, "delay_time", mDelayTimeKnob),
	mDelayMixAttachment(p.mState, "delay_mix", mDelayMixKnob),
	mDelayWidthAttachment(p.mState, "delay_width", mDelayWidthKnob)
	
{
    setSize (852, 600);

	// Debug textbox
	debugText.setBounds(0, 0, 100, 50);
	addAndMakeVisible(debugText);
	debugText.setText("test");
	//debugText.setText((String)(utils::modulo(-5, 7)));

	


	// Initialise knobs
	mRevMixKnob.init( "RevMixKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 2 - 35), (int)(getHeight() / 2), knobImg);
	mPanKnob.init("PanKnob", -1.0f, 1.0f, 0.0f, (int)(getWidth() / 2 - 35), (int)(getHeight() / 6 - 7), knobImg);
	mDelayFeedbackKnob.init("DelayFeedbackKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 4 - 25), (int)(getHeight() / 2), knobImg);
	mDelayTimeKnob.init("DelayTimeKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 6 - 25), (int)(getHeight() / 2), knobImg);
	mDelayMixKnob.init("DelayMixKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 9 - 25), (int)(getHeight() / 2.5), knobImg);
	mDelayWidthKnob.init("DelayWidthKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 6 - 25), (int)(getHeight() / 1.5), knobImg);

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

	//CustomLookAndFeel thisLookAndFeel(knobImg);
	//mPanKnob.setLookAndFeel(&thisLookAndFeel);
	//mRevMixKnob.setLookAndFeel(LookAndFeel_V4* x);

	

	// Add knobs to GUI
	addAndMakeVisible(mRevMixKnob);
	addAndMakeVisible(mPanKnob);
	addAndMakeVisible(mDelayFeedbackKnob);
	addAndMakeVisible(mDelayTimeKnob);
	addAndMakeVisible(mDelayMixKnob);
	addAndMakeVisible(mDelayWidthKnob);
	//mRevMixKnob.setAlpha(0.0);




	//StandardRotary mDelayFeedbackKnob(0.0f, 1.0f, 0.5f, (int)(getWidth() / 4 - 25), (int)(getHeight() / 6), knobImg);
	
	//Slider mDelayFeedbackKnob;
	/*mDelayFeedbackKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDelayFeedbackKnob.setRange(0.0f, 0.9f);
	mDelayFeedbackKnob.setValue(0.5f);
	mDelayFeedbackKnob.hideTextBox(true);
	mDelayFeedbackKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	addAndMakeVisible(mDelayFeedbackKnob);
	mDelayFeedbackKnob.setBounds(getWidth() / 4 - 25, getHeight() / 6, 50, 50);
	*/
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
