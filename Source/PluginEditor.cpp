/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
SpacePanAudioProcessorEditor::SpacePanAudioProcessorEditor (SpacePanAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (852, 600);


	mRevMixKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mRevMixKnob.setRange(0.0f, 1.0f);
	mRevMixKnob.setValue(0.5f);
	addAndMakeVisible(mRevMixKnob);
	mRevMixKnob.setBounds(getWidth() / 2, getHeight() / 2, 200, 200);

	mPanKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mPanKnob.setRange(-1.0f, 1.0f);
	mPanKnob.setValue(0.0f);
	mPanKnob.hideTextBox(true);
	mPanKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	addAndMakeVisible(mPanKnob);
	mPanKnob.setBounds(getWidth() / 2 - 25, getHeight() / 6, 50, 50);

	mDelayFeedbackKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDelayFeedbackKnob.setRange(0.0f, 0.9f);
	mDelayFeedbackKnob.setValue(0.5f);
	mDelayFeedbackKnob.hideTextBox(true);
	mDelayFeedbackKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	addAndMakeVisible(mDelayFeedbackKnob);
	mDelayFeedbackKnob.setBounds(getWidth() / 4 - 25, getHeight() / 6, 50, 50);

	//mDelayFeedbackKnob.init(0.0f, 0.9f, 0.5f, getWidth() / 4 - 25, getHeight() / 6);
}

SpacePanAudioProcessorEditor::~SpacePanAudioProcessorEditor()
{
}

//template<Slider T>
//void SpacePanAudioProcessorEditor::mouseMove(const MouseEvent &)
//{
//	T::mouseMove(const MouseEvent &)
//}

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
