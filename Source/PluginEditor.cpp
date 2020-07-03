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
    : AudioProcessorEditor (&p), processor (p),
	mRevMixAttachment(p.mState, "rev_mix", mRevMixKnob)
{
    setSize (852, 600);

	// Debug textbox
	debugText.setBounds(0, 0, 100, 50);
	addAndMakeVisible(debugText);
	debugText.setText("test");

	


	// Initialise knobs
	mRevMixKnob.init( "RevMixKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 2 - 35), (int)(getHeight() / 2), knobImg);
	mPanKnob.init("PanKnob", -1.0f, 1.0f, 0.0f, (int)(getWidth() / 2 - 35), (int)(getHeight() / 6 - 7), knobImg);
	mDelayFeedbackKnob.init("DelayFeedbackKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 4 - 25), (int)(getHeight() / 6), knobImg);
	mDelayTimeKnob.init("DelayTimeKnob", 0.0f, 1.0f, 0.5f, (int)(getWidth() / 6 - 25), (int)(getHeight() / 6), knobImg);

	mRevMixKnob.addListener(this);
	mPanKnob.addListener(this);
	mDelayFeedbackKnob.addListener(this);
	mDelayTimeKnob.addListener(this);

	//CustomLookAndFeel thisLookAndFeel(knobImg);
	//mPanKnob.setLookAndFeel(&thisLookAndFeel);
	//mRevMixKnob.setLookAndFeel(LookAndFeel_V4* x);

	

	// Add knobs to GUI
	addAndMakeVisible(mRevMixKnob);
	addAndMakeVisible(mPanKnob);
	addAndMakeVisible(mDelayFeedbackKnob);
	addAndMakeVisible(mDelayTimeKnob);
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
	/*
	debugText.setText(slider->getName());

	
	if (slider->getName() == "RevMixKnob")
	{
		*(processor.mRevMixParam) = slider->getValue();
	}
	else if (slider->getName() == "PanKnob")
	{
		*(processor.mPanParam) = slider->getValue();
	}
	else if (slider->getName() == "DelayFeedbackKnob")
	{
		*(processor.mDelayFeedbackParam) = slider->getValue();
	}
	else if (slider->getName() == "DelayTimeKnob")
	{
		*(processor.mDelayTimeParam) = slider->getValue();
	}*/
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
	mRevMixKnob.draw(g);
	mDelayFeedbackKnob.draw(g);
	mPanKnob.draw(g);
	mDelayTimeKnob.draw(g);
}

void SpacePanAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//void Slider::mouseEnter(MouseEvent&)
