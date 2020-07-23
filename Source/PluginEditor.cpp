/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/



// TODO: can probably remove class StandardSwitch

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
	mRevSCamountAttachment(p.mState, "rev_sc_amount", mRevSCamountKnob),

	mPanAttachment(p.mState, "pan", mPanKnob),
	mRoomSizeAttachment(p.mState, "room_size", mRoomSizeKnob),
	mHeadWidthAttachment(p.mState, "head_width", mHeadWidthSlider),

	mDelayFeedbackAttachment(p.mState, "delay_feedback", mDelayFeedbackKnob),
	mDelayTempoLockAttachment(p.mState, "delay_tempo_lock", mDelayTempoLockButton),
	mDelayTimeAttachment(p.mState, "delay_time", mDelayTimeKnob),
	mDelayDiscreteTimeAttachment(p.mState, "delay_time_discrete", mDelayDiscreteTimeKnob),
	mNoteDotTripAttachment(p.mState, "delay_modifier", mNoteDotTripSlider),
	mDelayLowPassAttachment(p.mState, "delay_lowpass", mDelayLowPassKnob),
	mDelayLowPassQAttachment(p.mState, "delay_lowpass_Q", mDelayLowPassQKnob),
	mDelayHighPassAttachment(p.mState, "delay_highpass", mDelayHighPassKnob),
	mDelayHighPassQAttachment(p.mState, "delay_highpass_Q", mDelayHighPassQKnob),
	mDelayMixAttachment(p.mState, "delay_mix", mDelayMixKnob),
	mDelaySatAttachment(p.mState, "delay_sat", mDelaySatKnob),
	mDelaySatCharAttachment(p.mState, "delay_sat_char", mDelaySatCharKnob),
	mDelayWidthAttachment(p.mState, "delay_width", mDelayWidthKnob),
	mDelayDiffusionAttachment(p.mState, "delay_diffusion", mDelayDiffusionKnob),
	mDelaySCamountAttachment(p.mState, "delay_sc_amount", mDelaySCamountKnob),

	mSCattackAttachment(p.mState, "sc_attack", mSCattackKnob),
	mSCattackShapeAttachment(p.mState, "sc_attack_shape", mSCattackShapeKnob),
	mSCdecayAttachment(p.mState, "sc_decay", mSCdecayKnob),
	mSCdecayShapeAttachment(p.mState, "sc_decay_shape", mSCdecayShapeKnob),
	mSCsustainLevelAttachment(p.mState, "sc_sustain_level", mSCsustainLevelKnob),
	mSCsustainAttachment(p.mState, "sc_sustain", mSCsustainKnob),
	mSCreleaseAttachment(p.mState, "sc_release", mSCreleaseKnob),
	mSCreleaseShapeAttachment(p.mState, "sc_release_shape", mSCreleaseShapeKnob),
	mSCthresholdAttachment(p.mState, "sc_threshold", mSCthresholdKnob)
	
{
    setSize (852, 600);
	/*LookAndFeel *editorLookAndFeel = &this->getLookAndFeel();
	*editorLookAndFeel.setDefaultSansSerifTypeface(BPdotsTypeface);*/
	LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(BPdotsTypeface);

	// Debug textbox
	debugText.setBounds(0, 0, 150, 100);
	addAndMakeVisible(debugText);
	debugText.setButtonText("Debug\nText");
	//debugText.setText((String)(utils::modulo(-5, 7)));

	/*mHeadWidthSlider.setRange(0.0, 10.0);
	mHeadWidthSlider.setValue(0.15);
	mHeadWidthSlider.setBounds(600, 200, 20, 100);
	//mHeadWidthSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);*/
	/*mDelayDiscreteTimeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	mDelayDiscreteTimeKnob.setBounds(20, 20, 200, 200);
	addAndMakeVisible(mDelayDiscreteTimeKnob);*/

	
	// Initialise and add knobs to GUI
	addControl(this, mRevMixKnob, "RevMixKnob", 0.6f, 0.46f, knobImg, "Mix");
	addControl(this, mRevLowPassKnob, "RevLowPassKnob", 0.8f, 0.5f, knobImgPan, "Lowpass");
	mRevLowPassQKnob.setDim(32, 32);
	addControl(this, mRevLowPassQKnob, "RevLowPassQKnob", 0.8f, 0.5f, knobImg, "Lowpass Q");
	addControl(this, mRevHighPassKnob, "RevHighPassKnob", 0.87f, 0.5f, knobImgPan, "Highpass");
	mRevHighPassQKnob.setDim(32, 32);
	addControl(this, mRevHighPassQKnob, "RevHighPassQKnob", 0.87f, 0.5f, knobImg, "Highpass Q");
	addControl(this, mRevSCamountKnob, "RevSCamountKnob", 0.9f, 0.6f, knobImgPan, "Sidechain Amount");

	addControl(this, mDelaySatKnob, "DelaySatKnob", 0.35f, 0.6f, knobImg, "Saturation");
	addControl(this, mPanKnob, "PanKnob", 0.5f, 0.15f, knobImgPan, "Pan");
	addControl(this, mRoomSizeKnob, "RoomSizeKnob", 0.7f, 0.5f, knobImg, "Room size");
	addControl(this, mHeadWidthSlider, "HeadWidthKnob", 0.6f, 0.15f, knobImgPan, "Head width");
	addControl(this, mDelayFeedbackKnob, "DelayFeedbackKnob", 0.28f, 0.5f, knobImg, "Feedback");
	addControl(this, mDelayTimeKnob, "DelayTimeKnob", 0.18f, 0.5f, knobImg, "Time");
	addControl(this, mDelayDiscreteTimeKnob, "DelayDiscreteTimeKnob", 0.18f, 0.5f, knobImg, "Time");
	
	addControl(this, mDelayLowPassKnob, "DelayLowPassKnob", 0.35f, 0.5f, knobImgPan, "Lowpass");
	mDelayLowPassQKnob.setDim(32, 32);
	addControl(this, mDelayLowPassQKnob, "DelayLowPassQKnob", 0.35f, 0.5f, knobImg, "Lowpass Q");
	addControl(this, mDelayHighPassKnob, "DelayHighPassKnob", 0.42f, 0.5f, knobImgPan, "Highpass");
	mDelayHighPassQKnob.setDim(32, 32);
	addControl(this, mDelayHighPassQKnob, "DelayHighPassQKnob", 0.42f, 0.5f, knobImg, "Highpass Q");
	addControl(this, mDelayMixKnob, "DelayMixKnob", 0.11f, 0.46f, knobImg, "Mix");
	addControl(this, mDelaySatCharKnob, "DelaySatCharKnob", 0.42f, 0.6f, knobImg, "Saturation character");
	addControl(this, mDelayWidthKnob, "DelayWidthKnob", 0.18f, 0.6f, knobImg, "Width");
	addControl(this, mDelayDiffusionKnob, "DelayDiffusionKnob", 0.28f, 0.6f, knobImg, "Diffusion");
	addControl(this, mDelaySCamountKnob, "DelaySCamountKnob", 0.5f, 0.6f, knobImgPan, "Sidechain Amount");


	mNoteDotTripSlider.setDim(32, 32);
	addControl(this, mNoteDotTripSlider, "noteDotTripSlider", 0.23f, 0.5f, switchImg, "Delay modifier", Slider::SliderStyle::LinearVertical);


	// Sidechain controls
	mSCattackKnob.setDim(50, 50);
	addControl(this, mSCattackKnob, "SCattackKnob", 0.22f, 0.89f, knobImgPan, "Attack");
	mSCattackShapeKnob.setDim(25, 25);
	addControl(this, mSCattackShapeKnob, "SCattackShapeKnob", 0.22f, 0.89f, knobImg, "Attack Shape");
	mSCdecayKnob.setDim(50, 50);
	addControl(this, mSCdecayKnob, "SCdecayKnob", 0.31f, 0.89f, knobImgPan, "Decay");
	mSCdecayShapeKnob.setDim(25, 25);
	addControl(this, mSCdecayShapeKnob, "SCdecayShapeKnob", 0.31f, 0.89f, knobImg, "Decay Shape");
	mSCsustainKnob.setDim(50, 50);
	addControl(this, mSCsustainKnob, "SCsustainKnob", 0.4f, 0.89f, knobImgPan, "Sustain");
	mSCsustainLevelKnob.setDim(25, 25);
	addControl(this, mSCsustainLevelKnob, "SCsustainLevelKnob", 0.4f, 0.89f, knobImgPan, "Sustain Level");
	mSCreleaseKnob.setDim(50, 50);
	addControl(this, mSCreleaseKnob, "SCreleaseKnob", 0.49f, 0.89f, knobImgPan, "Release");
	mSCreleaseShapeKnob.setDim(25, 25);
	addControl(this, mSCreleaseShapeKnob, "SCreleaseShapeKnob", 0.49f, 0.89f, knobImg, "release Shape");
	mSCthresholdKnob.setDim(50, 50);
	addControl(this, mSCthresholdKnob, "SCthresholdKnob", 0.58f, 0.89f, knobImgPan, "Threshold");


	

	//mDelayTimeKnob.hideTextBox(false);
	//mDelayTimeKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
	
	if (p.mIsTempoLocked)
	{
		mDelayTimeKnob.setVisible(false);
	}
	else
	{
		mDelayDiscreteTimeKnob.setVisible(false);
	}

	// Buttons
	//mDelayOnButton.setBounds(100, 100, 20, 80);
	//mDelayOnButton.setImages(false, false, true,);
	//addAndMakeVisible(mDelayOnButton);
	mDelayTempoLockButton.setName("delayTempoLockButton");

	mDelayTempoLockButton.setOnOffImages(delayTempoLockButtonImg);
	
	mDelayTempoLockButton.displayAsOn(true);
	mDelayTempoLockButton.setCentrePosition(200, 100);

	
	mDelayTempoLockButton.addListener(this);
	addAndMakeVisible(mDelayTempoLockButton);



	
	
	// Build CRO display
	adsrPlotImage = Image(Image::ARGB, 70, 70, true);
	adsrPlot.setImage(adsrPlotImage);
	constructADSRplot();
	adsrPlot.setBounds(550, 480, adsrPlotImage.getWidth(), adsrPlotImage.getHeight());
	addAndMakeVisible(adsrPlot);

	croGlass.setImage(croGlassImage);
	croGlass.setAlpha(0.5);
	croGlass.setBounds(540, 470, croGlassImage.getWidth(), croGlassImage.getHeight());
	addAndMakeVisible(croGlass);

	// Add signature button
	mSigButton.setName("sigButton");
	mSigButton.setImages(true, true, true, sigImg,
		1.0f, Colours::transparentBlack, sigImg, 1.0f, Colours::transparentBlack,
		sigDownImg, 1.0f, Colours::transparentBlack);
	mSigButton.setCentrePosition(770, 100);
	mSigButton.setSize(150, 50);
	mSigButton.addListener(this);
	addAndMakeVisible(mSigButton);


	Rectangle<int> timeTextBounds = Rectangle<int>(137, 227, 60, 41);
	mDelayTimeUnitsText.setBounds(timeTextBounds);
	mDelayTimeUnitsText.setEditable(false);
	mDelayTimeUnitsText.setColour(mDelayTimeUnitsText.textColourId, Colours::lightgreen);	
	mDelayTimeUnitsText.setJustificationType(Justification::centredBottom);
	mDelayTimeUnitsText.setFont(22);

	mDelayTimeText.setBounds(timeTextBounds);
	mDelayTimeText.setEditable(true);
	mDelayTimeText.setColour(mDelayTimeText.textColourId, Colours::lightgreen);
	mDelayTimeText.setJustificationType(Justification::centredTop);
	mDelayTimeText.setFont(22);
	
	mDelayTimeText.setText("testing", NotificationType::sendNotification);
	addAndMakeVisible(mDelayTimeUnitsText);
	addAndMakeVisible(mDelayTimeText);
	Image timeTextImage = croGlassImage.rescaled(timeTextBounds.getWidth(), timeTextBounds.getHeight());
	timeTextGlass.setImage(timeTextImage);
	timeTextGlass.setAlpha(0.5);
	timeTextGlass.setBounds(timeTextBounds);
	addAndMakeVisible(timeTextGlass);

	if (mDelayTempoLockButton.getToggleState())
	{
		
		int index = (int)mNoteDotTripSlider.getValue();
		mDelayDiscreteTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, processor.delayInBarsDP.getNames(), mDelayModifierChar.substring(index, index +1));
		
	}
	else
	{
		mDelayTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, 1000.0f);
	}

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
	bool isSCcontrol = (slider == &mSCattackKnob) || (slider == &mSCattackShapeKnob) ||
		(slider == &mSCdecayKnob) || (slider == &mSCdecayShapeKnob) ||
		(slider == &mSCsustainKnob) || (slider == &mSCsustainLevelKnob) ||
		(slider == &mSCreleaseKnob) || (slider == &mSCreleaseShapeKnob);
	if (isSCcontrol)
	{
		constructADSRplot();
		adsrPlot.repaint();
	}
	
	if (slider == &mDelayTimeKnob)
	{
		mDelayTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, 1000.0f);
	}
	else if ((slider == &mDelayDiscreteTimeKnob || slider == &mNoteDotTripSlider) && mDelayTempoLockButton.getToggleState())
	{
		// TODO: limit discrete time knob based on bpm and maximum buffer length		
		
		int index = (int)mNoteDotTripSlider.getValue();
		mDelayDiscreteTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, processor.delayInBarsDP.getNames(), mDelayModifierChar.substring(index, index + 1));
	}
}

void SpacePanAudioProcessorEditor::buttonClicked(Button* button)
{
	if (button->getName() == "delayTempoLockButton")
	{
		
		
		button->setToggleState(!button->getToggleState(), dontSendNotification);
		if (button->getToggleState())
		{
			mDelayDiscreteTimeKnob.setVisible(true);
			mDelayTimeKnob.setVisible(false);
			mDelayTempoLockButton.displayAsOn(true);
			int index = (int)mNoteDotTripSlider.getValue();
			mDelayDiscreteTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, processor.delayInBarsDP.getNames(), (String)mDelayModifierChar.substring(index, index + 1));
		}
		else
		{
			mDelayTempoLockButton.displayAsOn(false);
			mDelayDiscreteTimeKnob.setVisible(false);
			mDelayTimeKnob.setVisible(true);
			mDelayTimeKnob.sendToDisplay(mDelayTimeText, mDelayTimeUnitsText, 1000.0f);
		}
	}
	else if (button->getName() == "sigButton")
	{
		// For opening hyperlink
		
		//
	}

}

//void SpacePanAudioProcessorEditor::buttonStateChanged(Button* button)
//{
//	
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
	//g.drawImageAt(adsrPlot, 550, 450);
	//g.setColour(Colours::lightgreen);
	//g.drawLine(0.0f, 0.0f, 400.0f, 300.0f);
}

void SpacePanAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//void Slider::mouseEnter(MouseEvent&)

void SpacePanAudioProcessorEditor::constructADSRplot()
{

	
	const float adsrTimeMax = processor.ATTACK_MAX + processor.DECAY_MAX + processor.SUSTAIN_MAX + processor.RELEASE_MAX;
	float tAttack = *processor.mState.getRawParameterValue("sc_attack");
	float tDecay = *processor.mState.getRawParameterValue("sc_decay");
	float tSustain = *processor.mState.getRawParameterValue("sc_sustain");
	float tRelease = *processor.mState.getRawParameterValue("sc_release");

	float shapeAttack = *processor.mState.getRawParameterValue("sc_attack_shape");
	float shapeDecay = *processor.mState.getRawParameterValue("sc_decay_shape");
	float sustainGain = *processor.mState.getRawParameterValue("sc_sustain_level");
	float shapeRelease = *processor.mState.getRawParameterValue("sc_release_shape");;
	Colour traceColour = Colours::lightgreen;
	//Colour traceColourHalf = Colour::fromFloatRGBA (124.0, 252.0, 0.0, 0.8);
	Colour bgColour = Colours::transparentBlack;
	adsrPlotImage.clear(adsrPlotImage.getBounds(), bgColour);

	int pixTemp = adsrPlotImage.getHeight();
	//int pixPrev = 0;
	for (int i = 0; i < adsrPlotImage.getWidth(); i++)
	{
		float val;
		float t = i * adsrTimeMax / adsrPlotImage.getWidth();
		if (t < tAttack)
		{
			val = std::pow(t / tAttack, shapeAttack);
		}
		else if (t < tAttack + tDecay)
		{
			val = 1 - std::pow((t - tAttack) / tDecay, shapeDecay) * (1 - sustainGain);
		}
		else if (t < tAttack + tDecay + tSustain)
		{
			val = sustainGain;
		}
		else
		{
			val = (1 - std::pow((t - tAttack - tDecay - tSustain) / tRelease, shapeRelease)) * sustainGain;
		}
		int pix = (int)((1 - val) * adsrPlotImage.getHeight());
		pix = std::max(0, pix);
		pix = std::min(pix, adsrPlotImage.getHeight());


		while (std::abs(pix - pixTemp) > 0)
		{
			pixTemp = pixTemp + (int)copysign(1, pix - pixTemp);
			adsrPlotImage.setPixelAt(i, pixTemp, traceColour);
			/*if (i != adsrPlotImage.getWidth() - 1)
			{
				adsrPlotImage.setPixelAt(i + 1, pixTemp, traceColourHalf);
			}
			if (i != pixPrev && i != 0)
			{
				adsrPlotImage.setPixelAt(i - 1, pixTemp, traceColourHalf);
			}*/
		}
		adsrPlotImage.setPixelAt(i, pix, traceColour);

	}
	
	//adsrPlot.setImage(adsrPlotImage);
}

void SpacePanAudioProcessorEditor::addControl(SpacePanAudioProcessorEditor *const editor, StandardRotary &control, String name, float relX, float relY, Image spriteImg, String tooltipText, Slider::SliderStyle style)
{
	control.init(name, editor->getBounds(), relX, relY, spriteImg, style);
	control.setTooltip(tooltipText);
	control.addListener(editor);
	editor->addAndMakeVisible(control);

}
