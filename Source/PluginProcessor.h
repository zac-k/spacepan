/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularAudioBuffer.h"
#include "Envelope.h"
#include "DiscreteParam.h"


//==============================================================================
/**
*/
class SpacePanAudioProcessor  : public AudioProcessor,
								public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    SpacePanAudioProcessor();
    ~SpacePanAudioProcessor() override;


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
    using AudioProcessor::processBlock;

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
	const float DELAY_MAX = 60.0; // seconds

	// Keep this public so the labels can be displayed in GUI
	float delayInBars[9]{ 0.03125f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f };
	String delayInBarsStr[9]{ "1/32", "1/16", "1/8", "1/4", "1/2", "1", "2", "4", "8" };
	DiscreteParam<float> delayInBarsDP{ delayInBars, delayInBarsStr, 9 };

	float delayModifierVals[3]{ 2.0f / 3.0f, 1.5f, 1.0f };
	String delayModifierStr[3]{"Triplets", "Dotted", "Note" };
	DiscreteParam<float> delayModifierDP{ delayModifierVals, delayModifierStr, 3 };

	int delayFilterTypeVals[3]{ 0, 1, 2 };
	String delayFilterTypeStr[3]{ "Post filter", "Recursive filter", "Pre filter" };
	DiscreteParam<int> delayFilterTypeDP{ delayFilterTypeVals, delayFilterTypeStr, 3 };

	int delayStereoTypeVals[3]{ 0, 1, 2 };
	String delayStereoTypeStr[3]{ "Swing-pong", "Ping-pong", "Widen" };
	DiscreteParam<int> delayStereoTypeDP{ delayStereoTypeVals, delayStereoTypeStr, 3 };
	//==============================================================================
	AudioProcessorValueTreeState mState;

private:
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	// Processing stages, applied in this order to each block
	void pan(AudioBuffer<float> &buffer, float pan);
	void truePan(AudioBuffer<float> &buffer, float panVal, float maxPan);
	void delay(AudioBuffer<float> &samples);
	void reverb(AudioBuffer<float> &buffer, float panVal);

	void mixer(AudioBuffer<float> &dry, AudioBuffer<float> &wet, float mix, int channel, float vol);
	void saturate(AudioBuffer<float> &samples, float gain, float character);
	float getDelaySeconds() const;
	dsp::Reverb::Parameters getReverbParameters() const;
	dsp::Reverb::Parameters getDelayverbParameters() const;

	// Parameter values, looked up once so the audio thread doesn't search by name
	struct Parameters
	{
		std::atomic<float>* revMix;
		std::atomic<float>* revLowpass;
		std::atomic<float>* revLowpassQ;
		std::atomic<float>* revHighpass;
		std::atomic<float>* revHighpassQ;
		std::atomic<float>* revScAmount;
		std::atomic<float>* revOnOff;

		std::atomic<float>* pan;
		std::atomic<float>* roomSize;
		std::atomic<float>* headWidth;

		std::atomic<float>* delayFeedback;
		std::atomic<float>* delayTime;
		std::atomic<float>* delayTimeDiscrete;
		std::atomic<float>* delayModifier;
		std::atomic<float>* delayFilterType;
		std::atomic<float>* delayStereoType;
		std::atomic<float>* delayLowpass;
		std::atomic<float>* delayLowpassQ;
		std::atomic<float>* delayHighpass;
		std::atomic<float>* delayHighpassQ;
		std::atomic<float>* delayDiffusion;
		std::atomic<float>* delayScAmount;
		std::atomic<float>* delayTempoLock;
		std::atomic<float>* delayOnOff;
		std::atomic<float>* delayMix;
		std::atomic<float>* delaySat;
		std::atomic<float>* delaySatChar;
		std::atomic<float>* delayWidth;

		std::atomic<float>* scAttack;
		std::atomic<float>* scAttackShape;
		std::atomic<float>* scDecay;
		std::atomic<float>* scDecayShape;
		std::atomic<float>* scSustainLevel;
		std::atomic<float>* scSustain;
		std::atomic<float>* scRelease;
		std::atomic<float>* scReleaseShape;
		std::atomic<float>* scThreshold;
		std::atomic<float>* scOnOff;
	};
	Parameters mParams;

	// Low pass filters for pan
	dsp::IIR::Filter<float> lowPassFilterL;
	dsp::IIR::Filter<float> lowPassFilterR;

	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayLowPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > delayHighPassFilter;

	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > revLowPassFilter;
	dsp::ProcessorDuplicator< dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float> > revHighPassFilter;

	dsp::Reverb delayverb;
	dsp::Reverb mReverb;
	Envelope revEnvelope[2];
	Envelope delEnvelope[2];

	int mMaxBlockSize{ 512 };
	std::atomic<double> mBpm{ 120.0 }; // read by getTailLengthSeconds() on other threads

	// Requests from other threads, handled at the start of the next block
	std::atomic<bool> mClearDelayRequested{ false };
	std::atomic<bool> mResetReverbRequested{ false };

	// Buffers
	CircularAudioBuffer<float> mDelayBuffer;
	CircularAudioBuffer<float> mPanBuffer;
	CircularAudioBuffer<float> mReverbBuffer;

	// Scratch buffers, allocated in prepareToPlay()
	AudioBuffer<float> mDelayDryBuffer;
	AudioBuffer<float> mDelayWetBuffer;
	AudioBuffer<float> mDelayTempBuffer;
	AudioBuffer<float> mReverbWetBuffer;

    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpacePanAudioProcessor)
};
