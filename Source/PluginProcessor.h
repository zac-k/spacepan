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
	const float DELAY_MAX = 2.0; // seconds


	//==============================================================================
	AudioProcessorValueTreeState mState;

private:

	void saturate(float &sample, float gain);
	void saturate(AudioBuffer<float> &samples, float gain);

	//float mHeadWidth{ 2 }; // metres

	// Low pass filters for atmopan
	dsp::IIR::Filter<float> lowPassFilterL;
	dsp::IIR::Filter<float> lowPassFilterR;

	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayLowPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayHighPassFilter;
	dsp::ProcessorDuplicator< dsp::FIR::Filter<float>, dsp::FIR::Coefficients<float> > delayAllPassFilter;

	dsp::Reverb preverb;
	dsp::Reverb delayverb;
	dsp::Reverb mReverb;
	//dsp::Reverb preverbR;


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
