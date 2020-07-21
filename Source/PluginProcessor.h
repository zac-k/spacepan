/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioBufferWithPos.cpp"
#include "Envelope.h"
#include "DiscreteParam.cpp"






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


	const float ATTACK_MAX = 0.2f;
	const float DECAY_MAX = 0.2f;
	const float SUSTAIN_MAX = 0.5f;
	const float RELEASE_MAX = 1.0f;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength,
		const int delayBufferLength, const float* bufferData, const float* delayBufferData, int delaySamples, int readPosition);

	void pan(AudioBuffer<float> &buffer, float pan);

	void delay(AudioBuffer<float> &samples, CircularAudioBuffer<float> &buffer, int numSamples,
		float* delayOffsets, float sampleRate, int32 comb, bool fb);

	float* combFilter(float* samples, float** buffer, int channel, int numSamples,
		int32 delayInSamples, float** delayOffsets, float decay,
		float sampleRate, int32 comb, bool fb);
	void truePan(AudioBuffer<float> &buffer, float panVal, float maxPan);
	void reverb(AudioBuffer<float> &buffer, float panVal);
	void atmoPan(AudioBuffer<float> &buffer, float panVal, float maxPan);

	void mixer(AudioBuffer<float> &dry, AudioBuffer<float> &wet, float mix, int channel, float vol);
	/*void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength,
		const float* bufferData, const float* delayBufferData);*/
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
	const float DELAY_MAX = 10.0; // seconds
	
	// Keep this public so the labels can be displayed in GUI
	float delayInBars[9]{ 0.03125f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f };
	String delayInBarsStr[9]{ "1/32", "1/16", "1/8", "1/4", "1/2", "1", "2", "4", "8" };
	DiscreteParam<float> delayInBarsDP{ delayInBars, delayInBarsStr, 9 };
	float delayModifierVals[3]{ 2.0f / 3.0f, 1.5f, 1.0f };
	String delayModifierStr[3]{"Triplets", "Dotted", "Note" };
	DiscreteParam<float> delayModifierDP{ delayModifierVals, delayModifierStr, 3 };

	//==============================================================================
	AudioProcessorValueTreeState mState;
	bool mIsTempoLocked;
	
		//{ 0.03125f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f }
private:

	//StringArray::StringArray("1/32", "1/16", "1/8", "1/4", "1/2", "1", "2")

	

	void saturate(float &sample, float gain);
	void saturate(AudioBuffer<float> &samples, float gain, int type);
	void saturate(AudioBuffer<float> &samples, float gain, float character);
	//float mHeadWidth{ 2 }; // metres

	// Low pass filters for atmopan
	dsp::IIR::Filter<float> lowPassFilterL;
	dsp::IIR::Filter<float> lowPassFilterR;

	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayLowPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayHighPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayAllPassFilter;


	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > revLowPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > revHighPassFilter;

	dsp::Reverb preverb;
	dsp::Reverb delayverb;
	dsp::Reverb mReverb;
	//dsp::Reverb preverbR;
	Envelope revEnvelope[2];
	Envelope delEnvelope[2];

	double mPanFcR{ 0 };
	double mPanFcL{ 0 };


	int mBufferPosArr[2] = { 0, 0 };

	// Buffers
	CircularAudioBuffer<float> mDelayBuffer;
	CircularAudioBuffer<float> mPanBuffer;
	CircularAudioBuffer<float> mPreverbBuffer;
	CircularAudioBuffer<float> mReverbBuffer;

	//==============================================================================

	/*void SpacePanAudioProcessor::fillBuffer(int channel, const int delayBufferLength,
		const int inputBufferLength,
		const float* bufferData);*/
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpacePanAudioProcessor)


		//int mWritePosition{ 0 };


	UndoManager mUndoManager;
};
