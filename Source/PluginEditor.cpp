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
	


	// Initialise knobs
	//mRevMixKnob.init( "RevMixKnob", getBounds(), 0.6, 0.45, knobImg);
	addControl(this, mRevMixKnob, "RevMixKnob", getBounds(), 0.6, 0.45, knobImg, "Mix");

	mRevLowPassKnob.init("RevLowPassKnob", getBounds(), 0.8, 0.5, knobImgPan);
	mRevLowPassQKnob.setDim(32, 32);
	mRevLowPassQKnob.init("RevLowPassQKnob", getBounds(), 0.8, 0.5, knobImg);
	mRevHighPassKnob.init("RevHighPassKnob", getBounds(), 0.87, 0.5, knobImgPan);
	mRevHighPassQKnob.setDim(32, 32);
	mRevHighPassQKnob.init("RevHighPassQKnob", getBounds(), 0.87, 0.5, knobImg);

	mPanKnob.init("PanKnob", getBounds(), 0.5, 0.15, knobImgPan);
	mRoomSizeKnob.init("RoomSizeKnob", getBounds(), 0.7, 0.5, knobImg);
	mHeadWidthSlider.init("HeadWidthKnob", getBounds(), 0.6, 0.15, knobImgPan);
	mDelayFeedbackKnob.init("DelayFeedbackKnob", getBounds(), 0.25, 0.5, knobImg);
	mDelayTimeKnob.init("DelayTimeKnob", getBounds(), 0.15, 0.5, knobImg);

	mDelayLowPassKnob.init("DelayLowPassKnob", getBounds(), 0.33, 0.5, knobImgPan);
	mDelayLowPassQKnob.setDim(32, 32);
	mDelayLowPassQKnob.init("DelayLowPassQKnob", getBounds(), 0.33, 0.5, knobImg);	
	mDelayHighPassKnob.init("DelayHighPassKnob", getBounds(), 0.4, 0.5, knobImgPan);
	mDelayHighPassQKnob.setDim(32, 32);
	mDelayHighPassQKnob.init("DelayHighPassQKnob", getBounds(), 0.4, 0.5, knobImg);
	
	mDelayTimeText.setBounds(0, 0, 80, 20);
	//mDelayTimeKnob.hideTextBox(false);
	//mDelayTimeKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
	mDelayMixKnob.init("DelayMixKnob", getBounds(), 0.11, 0.4, knobImg);
	mDelaySatKnob.init("DelaySatKnob", getBounds(), 0.33, 0.6, knobImg);
	mDelaySatCharKnob.init("DelaySatCharKnob", getBounds(), 0.4, 0.6, knobImg);
	mDelayWidthKnob.init("DelayWidthKnob", getBounds(), 0.15, 0.6, knobImg);


	// Buttons
	//mDelayOnButton.setBounds(100, 100, 20, 80);
	//mDelayOnButton.setImages(false, false, true,);
	//addAndMakeVisible(mDelayOnButton);


	// TODO: finish adding tooltips
	// Set tooltips
	//mRevMixKnob.setTooltip("Mix");
	mPanKnob.setTooltip("Pan");
	mRoomSizeKnob.setTooltip("Room size");
	mDelayFeedbackKnob.setTooltip("Feedback amount");
	mDelayTimeKnob.setTooltip("Time");
	mDelayMixKnob.setTooltip("Mix");
	mDelayWidthKnob.setTooltip("Stereo width");

	//mRevMixKnob.addListener(this);
	mPanKnob.addListener(this);
	mRoomSizeKnob.addListener(this);
	mDelayFeedbackKnob.addListener(this);
	mDelayTimeKnob.addListener(this);
	mDelayMixKnob.addListener(this);
	mDelayWidthKnob.addListener(this);

	
	

	// Add knobs to GUI
	addAndMakeVisible(mRevMixKnob);
	addAndMakeVisible(mRevLowPassKnob);
	addAndMakeVisible(mRevLowPassQKnob);
	addAndMakeVisible(mRevHighPassKnob);
	addAndMakeVisible(mRevHighPassQKnob);
	addAndMakeVisible(mPanKnob);
	addAndMakeVisible(mRoomSizeKnob);
	addAndMakeVisible(mHeadWidthSlider);
	addAndMakeVisible(mDelayFeedbackKnob);
	addAndMakeVisible(mDelayTimeKnob);
	addAndMakeVisible(mDelayLowPassKnob);
	addAndMakeVisible(mDelayLowPassQKnob);
	addAndMakeVisible(mDelayHighPassKnob);
	addAndMakeVisible(mDelayHighPassQKnob);
	addAndMakeVisible(mDelayMixKnob);
	addAndMakeVisible(mDelaySatKnob);
	addAndMakeVisible(mDelaySatCharKnob);
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

void SpacePanAudioProcessorEditor::addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, Rectangle<int> parentBounds, float relX, float relY, Image spriteImg, String tooltipText)
{
	control.init(name, parentBounds, relX, relY, spriteImg);
	control.setTooltip(tooltipText);
	control.addListener(editor);
	editor->addAndMakeVisible(control);

}
