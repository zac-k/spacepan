/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioBufferWithPos.cpp"




//==============================================================================
/**
*/
class SpacePanAudioProcessor  : public AudioProcessor,
								public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    SpacePanAudioProcessor();
    ~SpacePanAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength,
		const int delayBufferLength, const float* bufferData, const float* delayBufferData, int delaySamples);
	void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength,
		const float* bufferData, const float* delayBufferData);
	//==============================================================================


	void parameterChanged(const String &parameterID, float newValue) override;


    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	const float DELAY_MAX = 2.0; // seconds

	// Parameters

	juce::AudioParameterFloat* mRevMixParam;
	juce::AudioParameterFloat* mPanParam;
	juce::AudioParameterFloat* mDelayFeedbackParam;
	juce::AudioParameterFloat* mDelayTimeParam;

	//==============================================================================
	AudioProcessorValueTreeState mState;

private:

	

	// Buffers
	AudioBufferWithPos<float> mDelayBuffer;
	AudioBufferWithPos<float> mDelayBufferTemp;

	//==============================================================================

	void SpacePanAudioProcessor::fillBuffer(int channel, const int delayBufferLength,
		const int inputBufferLength,
		const float* bufferData);
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpacePanAudioProcessor)


		int mWritePosition{ 0 };


	UndoManager mUndoManager;
};
