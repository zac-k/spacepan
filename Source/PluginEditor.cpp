/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
//#include "PluginProcessor.cpp"
#include "PluginEditor.h"
//#include "utils.h"


//==============================================================================
SpacePanAudioProcessorEditor::SpacePanAudioProcessorEditor(SpacePanAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	mRevMixAttachment(p.mState, "rev_mix", mRevMixKnob),
	mRevLowPassAttachment(p.mState, "rev_lowpass", mRevLowPassKnob),
	mRevLowPassQAttachment(p.mState, "rev_lowpass_Q", mRevLowPassQKnob),
	mRevHighPassAttachment(p.mState, "rev_highpass", mRevHighPassKnob),
	mRevHighPassQAttachment(p.mState, "rev_highpass_Q", mRevHighPassQKnob),
	mPanAttachment(p.mState, "pan", mPanKnob),
	mRoomSizeAttachment(p.mState, "room_size", mRoomSizeKnob),
	mHeadWidthAttachment(p.mState, "head_width", mHeadWidthSlider),
	mDelayFeedbackAttachment(p.mState, "delay_feedback", mDelayFeedbackKnob),
	mDelayTimeAttachment(p.mState, "delay_time", mDelayTimeKnob),
	mDelayLowPassAttachment(p.mState, "delay_lowpass", mDelayLowPassKnob),
	mDelayLowPassQAttachment(p.mState, "delay_lowpass_Q", mDelayLowPassQKnob),
	mDelayHighPassAttachment(p.mState, "delay_highpass", mDelayHighPassKnob),
	mDelayHighPassQAttachment(p.mState, "delay_highpass_Q", mDelayHighPassQKnob),
	mDelayMixAttachment(p.mState, "delay_mix", mDelayMixKnob),
	mDelaySatAttachment(p.mState, "delay_sat", mDelaySatKnob),
	mDelaySatCharAttachment(p.mState, "delay_sat_char", mDelaySatCharKnob),
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
	


	
	// Initialise and add knobs to GUI
	addControl(this, mRevMixKnob, "RevMixKnob", 0.6, 0.45, knobImg, "Mix");
	addControl(this, mRevLowPassKnob, "RevLowPassKnob", 0.8, 0.5, knobImgPan, "Lowpass");
	mRevLowPassQKnob.setDim(32, 32);
	addControl(this, mRevLowPassQKnob, "RevLowPassQKnob", 0.8, 0.5, knobImg, "Lowpass Q");
	addControl(this, mRevHighPassKnob, "RevHighPassKnob", 0.87, 0.5, knobImgPan, "Highpass");
	mRevHighPassQKnob.setDim(32, 32);
	addControl(this, mRevHighPassQKnob, "RevHighPassQKnob", 0.87, 0.5, knobImg, "Highpass Q");
	addControl(this, mDelaySatKnob, "DelaySatKnob", 0.33, 0.6, knobImg, "Saturation");
	addControl(this, mPanKnob, "PanKnob", 0.5, 0.15, knobImgPan, "Pan");
	addControl(this, mRoomSizeKnob, "RoomSizeKnob", 0.7, 0.5, knobImg, "Room size");
	addControl(this, mHeadWidthSlider, "HeadWidthKnob", 0.6, 0.15, knobImgPan, "Head width");
	addControl(this, mDelayFeedbackKnob, "DelayFeedbackKnob", 0.25, 0.5, knobImg, "Feedback");
	addControl(this, mDelayTimeKnob, "DelayTimeKnob", 0.15, 0.5, knobImg, "Time");
	addControl(this, mDelayLowPassKnob, "DelayLowPassKnob", 0.33, 0.5, knobImgPan, "Lowpass");
	mDelayLowPassQKnob.setDim(32, 32);
	addControl(this, mDelayLowPassQKnob, "DelayLowPassQKnob", 0.33, 0.5, knobImg, "Lowpass Q");
	addControl(this, mDelayHighPassKnob, "DelayHighPassKnob", 0.4, 0.5, knobImgPan, "Highpass");
	mDelayHighPassQKnob.setDim(32, 32);
	addControl(this, mDelayHighPassQKnob, "DelayHighPassQKnob", 0.4, 0.5, knobImg, "Highpass Q");
	addControl(this, mDelayMixKnob, "DelayMixKnob", 0.11, 0.4, knobImg, "Mix");
	addControl(this, mDelaySatCharKnob, "DelaySatCharKnob", 0.4, 0.6, knobImg, "Saturation character");
	addControl(this, mDelayWidthKnob, "DelayWidthKnob", 0.15, 0.6, knobImg, "Width");

	mDelayTimeText.setBounds(0, 0, 80, 20);
	//mDelayTimeKnob.hideTextBox(false);
	//mDelayTimeKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
	


	// Buttons
	//mDelayOnButton.setBounds(100, 100, 20, 80);
	//mDelayOnButton.setImages(false, false, true,);
	//addAndMakeVisible(mDelayOnButton);




	

	
	



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

void SpacePanAudioProcessorEditor::addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, float relX, float relY, Image spriteImg, String tooltipText)
{
	control.init(name, editor->getBounds(), relX, relY, spriteImg);
	control.setTooltip(tooltipText);
	control.addListener(editor);
	editor->addAndMakeVisible(control);

}
