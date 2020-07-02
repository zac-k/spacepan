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



	// Initialise knobs
	mRevMixKnob.init(0.0f, 1.0f, 0.5f, (int)(getWidth() / 2), (int)(getHeight() / 2), knobImg);
	mPanKnob.init(-1.0f, 1.0f, 0.0f, (int)(getWidth() / 2), (int)(getHeight() / 6), knobImg);
	mDelayFeedbackKnob.init(0.0f, 1.0f, 0.5f, (int)(getWidth() / 4 - 25), (int)(getHeight() / 6), knobImg);

	// Add knobs to GUI
	addAndMakeVisible(mRevMixKnob);
	addAndMakeVisible(mPanKnob);
	addAndMakeVisible(mDelayFeedbackKnob);

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

//==============================================================================
void SpacePanAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
	g.drawImageAt(backgroundImage, 0, 0);
	//mRevMixKnob.draw(g);
}

void SpacePanAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//void Slider::mouseEnter(MouseEvent&)
