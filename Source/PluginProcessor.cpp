/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils.h"

namespace
{
	const float PI = MathConstants<float>::pi;
	const float HEAD_WIDTH_MAX = 10; //metres
	const float ROOM_SIZE_MAX = 20; //metres
	const float SOUND_SPEED = 343.0; // m/s
	const float PREDELAY_DISTANCE_MAX = 3 * ROOM_SIZE_MAX; // metres, see reverb()
	const float WIDEN_DELAY_MAX = 1.0f / 50.0f; // seconds, see delay()
	const double DEFAULT_BPM = 120.0;

	// Keeps a filter cutoff safely below Nyquist
	float clampFrequency(float frequency, double sampleRate)
	{
		return jlimit(10.0f, (float)(0.45 * sampleRate), frequency);
	}

	// Transparent below 0 dBFS, and smoothly limits larger values to below 2.0
	// so that high feedback settings can't run away
	float softLimit(float x)
	{
		const float magnitude = std::abs(x);
		return magnitude <= 1.0f ? x : std::copysign(1.0f + std::tanh(magnitude - 1.0f), x);
	}

	// Sets a biquad from coefficients computed without allocating
	template <typename Filter>
	void setCoefficients(Filter& filter, const std::array<float, 6>& coefficients)
	{
		*filter.state = coefficients;
	}
}


//==============================================================================
SpacePanAudioProcessor::SpacePanAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
	mState(*this, nullptr, "state", createParameterLayout())
{
	mParams.revMix = mState.getRawParameterValue("rev_mix");
	mParams.revLowpass = mState.getRawParameterValue("rev_lowpass");
	mParams.revLowpassQ = mState.getRawParameterValue("rev_lowpass_Q");
	mParams.revHighpass = mState.getRawParameterValue("rev_highpass");
	mParams.revHighpassQ = mState.getRawParameterValue("rev_highpass_Q");
	mParams.revScAmount = mState.getRawParameterValue("rev_sc_amount");
	mParams.revOnOff = mState.getRawParameterValue("rev_on_off");

	mParams.pan = mState.getRawParameterValue("pan");
	mParams.roomSize = mState.getRawParameterValue("room_size");
	mParams.headWidth = mState.getRawParameterValue("head_width");

	mParams.delayFeedback = mState.getRawParameterValue("delay_feedback");
	mParams.delayTime = mState.getRawParameterValue("delay_time");
	mParams.delayTimeDiscrete = mState.getRawParameterValue("delay_time_discrete");
	mParams.delayModifier = mState.getRawParameterValue("delay_modifier");
	mParams.delayFilterType = mState.getRawParameterValue("delay_filter_type");
	mParams.delayStereoType = mState.getRawParameterValue("delay_stereo_type");
	mParams.delayLowpass = mState.getRawParameterValue("delay_lowpass");
	mParams.delayLowpassQ = mState.getRawParameterValue("delay_lowpass_Q");
	mParams.delayHighpass = mState.getRawParameterValue("delay_highpass");
	mParams.delayHighpassQ = mState.getRawParameterValue("delay_highpass_Q");
	mParams.delayDiffusion = mState.getRawParameterValue("delay_diffusion");
	mParams.delayScAmount = mState.getRawParameterValue("delay_sc_amount");
	mParams.delayTempoLock = mState.getRawParameterValue("delay_tempo_lock");
	mParams.delayOnOff = mState.getRawParameterValue("delay_on_off");
	mParams.delayMix = mState.getRawParameterValue("delay_mix");
	mParams.delaySat = mState.getRawParameterValue("delay_sat");
	mParams.delaySatChar = mState.getRawParameterValue("delay_sat_char");
	mParams.delayWidth = mState.getRawParameterValue("delay_width");

	mParams.scAttack = mState.getRawParameterValue("sc_attack");
	mParams.scAttackShape = mState.getRawParameterValue("sc_attack_shape");
	mParams.scDecay = mState.getRawParameterValue("sc_decay");
	mParams.scDecayShape = mState.getRawParameterValue("sc_decay_shape");
	mParams.scSustainLevel = mState.getRawParameterValue("sc_sustain_level");
	mParams.scSustain = mState.getRawParameterValue("sc_sustain");
	mParams.scRelease = mState.getRawParameterValue("sc_release");
	mParams.scReleaseShape = mState.getRawParameterValue("sc_release_shape");
	mParams.scThreshold = mState.getRawParameterValue("sc_threshold");
	mParams.scOnOff = mState.getRawParameterValue("sc_on_off");

	mState.addParameterListener("rev_on_off", this);
	mState.addParameterListener("delay_on_off", this);
}

AudioProcessorValueTreeState::ParameterLayout SpacePanAudioProcessor::createParameterLayout()
{
	auto discreteAttributes = [](const auto& discreteParam)
	{
		return AudioParameterIntAttributes().withStringFromValueFunction(discreteParam.labelsLambda());
	};

	return { std::make_unique<AudioParameterFloat>(ParameterID{ "rev_mix", 1 }, "Reverb Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "rev_lowpass", 1 }, "Reverb Lowpass", utils::frequencyRange(100.0f, 2.0e4f), 2.0e3f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "rev_lowpass_Q", 1 }, "Reverb Lowpass Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "rev_highpass", 1 }, "Reverb Highpass", utils::frequencyRange<float>(100.0f, 2.0e4f), 2.0e2f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "rev_sc_amount", 1 }, "Reverb Sidechain Amount", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "rev_highpass_Q", 1 }, "Reverb Highpass Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
		  std::make_unique<AudioParameterBool>(ParameterID{ "rev_on_off", 1 }, "Enable Reverb", true),

		  std::make_unique<AudioParameterFloat>(ParameterID{ "pan", 1 }, "Pan", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "room_size", 1 }, "Room Size", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "head_width", 1 }, "Head Width", NormalisableRange<float>(0.0f, 10.0f), 0.15f),

		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_feedback", 1 }, "Delay Feedback", NormalisableRange<float>(0.005f, 1.0f), 0.5f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_time", 1 }, "Delay Time", NormalisableRange<float>(0.0f, 1.0f), 0.5f),

		  std::make_unique<AudioParameterInt>(ParameterID{ "delay_time_discrete", 1 }, "Delay Time", 0, delayInBarsDP.getNumParams() - 1, 1, discreteAttributes(delayInBarsDP)),
		  std::make_unique<AudioParameterInt>(ParameterID{ "delay_modifier", 1 }, "Delay Modifier", 0, delayModifierDP.getNumParams() - 1, 1, discreteAttributes(delayModifierDP)),
		  std::make_unique<AudioParameterInt>(ParameterID{ "delay_filter_type", 1 }, "Delay Filter Type", 0, delayFilterTypeDP.getNumParams() - 1, 1, discreteAttributes(delayFilterTypeDP)),
		  std::make_unique<AudioParameterInt>(ParameterID{ "delay_stereo_type", 1 }, "Delay Stereo Type", 0, delayStereoTypeDP.getNumParams() - 1, 1, discreteAttributes(delayStereoTypeDP)),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_lowpass", 1 }, "Delay High Cut", utils::frequencyRange(100.0f, 2.0e4f), 2.0e3f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_lowpass_Q", 1 }, "Delay High Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_highpass", 1 }, "Delay Low Cut", utils::frequencyRange<float>(100.0f, 2.0e4f), 2.0e2f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_highpass_Q", 1 }, "Delay Low Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_diffusion", 1 }, "Delay Diffusion", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_sc_amount", 1 }, "Delay Sidechain Amount", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		  std::make_unique<AudioParameterBool>(ParameterID{ "delay_tempo_lock", 1 }, "Delay Tempo Lock", true),
		  std::make_unique<AudioParameterBool>(ParameterID{ "delay_on_off", 1 }, "Enable Delay", true),

		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_mix", 1 }, "Delay Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_sat", 1 }, "Delay Saturation", NormalisableRange<float>(0.0f, 10.0f), 0.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_sat_char", 1 }, "Delay Saturation Character", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "delay_width", 1 }, "Delay Width", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),

		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_attack", 1 }, "Sidechain Attack", NormalisableRange<float>(0.01f, ATTACK_MAX), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_attack_shape", 1 }, "Sidechain Attack Shape", NormalisableRange<float>(0.2f, 10.0f), 0.2f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_decay", 1 }, "Sidechain Decay", NormalisableRange<float>(0.01f, DECAY_MAX), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_decay_shape", 1 }, "Sidechain Decay Shape", NormalisableRange<float>(0.1f, 5.0f), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_sustain_level", 1 }, "Sidechain Sustain Level", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_sustain", 1 }, "Sidechain Sustain", NormalisableRange<float>(0.0f, SUSTAIN_MAX), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_release", 1 }, "Sidechain Release", NormalisableRange<float>(0.01f, RELEASE_MAX), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_release_shape", 1 }, "Sidechain release Shape", NormalisableRange<float>(0.1f, 10.0f), 0.1f),
		  std::make_unique<AudioParameterFloat>(ParameterID{ "sc_threshold", 1 }, "Sidechain Threshold", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
		  std::make_unique<AudioParameterBool>(ParameterID{ "sc_on_off", 1 }, "Enable Sidechain", false) };
}

void SpacePanAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{
	// This can be called on any thread, so the audio thread does the clearing.
	// Reset reverb when it is turned off
	if (parameterID == "rev_on_off" && newValue < 0.5f)
	{
		mResetReverbRequested = true;
	}
	else if (parameterID == "delay_on_off" && newValue < 0.5f)
	{
		mClearDelayRequested = true;
	}

}

SpacePanAudioProcessor::~SpacePanAudioProcessor()
{
	mState.removeParameterListener("rev_on_off", this);
	mState.removeParameterListener("delay_on_off", this);
}

//==============================================================================
const String SpacePanAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpacePanAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpacePanAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}



bool SpacePanAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpacePanAudioProcessor::getTailLengthSeconds() const
{
	double tail = 0.0;

	if (mParams.delayOnOff->load() > 0.5f)
	{
		const double feedback = mParams.delayFeedback->load();
		if (feedback >= 0.999)
		{
			return std::numeric_limits<double>::infinity();
		}

		// Time for the repeats to decay by 60 dB
		const double repeats = std::log(0.001) / std::log(feedback);
		tail += getDelaySeconds() * (repeats + 1.0);
	}

	if (mParams.revOnOff->load() > 0.5f)
	{
		tail += PREDELAY_DISTANCE_MAX / SOUND_SPEED + 5.0;
	}

	return tail;
}

int SpacePanAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpacePanAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpacePanAudioProcessor::setCurrentProgram (int)
{
}

const String SpacePanAudioProcessor::getProgramName (int)
{
    return {};
}

void SpacePanAudioProcessor::changeProgramName (int, const String&)
{
}

//==============================================================================
void SpacePanAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	// The processing is always stereo; a mono input is copied to both channels
	const int numChannels = 2;
	mMaxBlockSize = jmax(1, samplesPerBlock);

	const int delayBufferSize = static_cast<int>((DELAY_MAX + WIDEN_DELAY_MAX) * sampleRate + 2 * mMaxBlockSize);
	mDelayBuffer.setSize(numChannels, delayBufferSize);

	const int panBufferSize = static_cast<int>(HEAD_WIDTH_MAX / SOUND_SPEED * sampleRate + 2 * mMaxBlockSize);
	mPanBuffer.setSize(numChannels, panBufferSize);

	const int reverbBufferSize = static_cast<int>(PREDELAY_DISTANCE_MAX / SOUND_SPEED * sampleRate + 2 * mMaxBlockSize);
	mReverbBuffer.setSize(numChannels, reverbBufferSize);

	mDelayDryBuffer.setSize(numChannels, mMaxBlockSize);
	mDelayWetBuffer.setSize(numChannels, mMaxBlockSize);
	mDelayTempBuffer.setSize(numChannels, mMaxBlockSize);
	mReverbWetBuffer.setSize(numChannels, mMaxBlockSize);

	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = (uint32)mMaxBlockSize;
	spec.numChannels = (uint32)numChannels;

	// Give every filter biquad coefficients before preparing it, so that its
	// state is allocated here rather than on the first processed block
	const auto passAll = dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, clampFrequency(2.0e4f, sampleRate));
	*lowPassFilterL.coefficients = passAll;
	*lowPassFilterR.coefficients = passAll;
	setCoefficients(delayLowPassFilter, passAll);
	setCoefficients(delayHighPassFilter, passAll);
	setCoefficients(revLowPassFilter, passAll);
	setCoefficients(revHighPassFilter, passAll);

	lowPassFilterL.prepare(spec);
	lowPassFilterR.prepare(spec);
	delayLowPassFilter.prepare(spec);
	delayHighPassFilter.prepare(spec);
	revLowPassFilter.prepare(spec);
	revHighPassFilter.prepare(spec);
	lowPassFilterL.reset();
	lowPassFilterR.reset();
	delayLowPassFilter.reset();
	delayHighPassFilter.reset();
	revLowPassFilter.reset();
	revHighPassFilter.reset();

	// Set the reverb parameters before preparing, so that their smoothing
	// starts at these values rather than ramping from the defaults
	mReverb.setParameters(getReverbParameters());
	mReverb.prepare(spec);
	mReverb.reset();

	delayverb.setParameters(getDelayverbParameters());
	delayverb.prepare(spec);
	delayverb.reset();

	mClearDelayRequested = false;
	mResetReverbRequested = false;
}

void SpacePanAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpacePanAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
	// The output is always stereo. The input can be mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	return layouts.getMainInputChannelSet() == AudioChannelSet::mono()
		|| layouts.getMainInputChannelSet() == AudioChannelSet::stereo();
  #endif
}
#endif

void SpacePanAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples();

	if (buffer.getNumChannels() < 2 || numSamples == 0)
	{
		return;
	}

	// Copy a mono input to both channels, or clear any outputs without inputs
	if (totalNumInputChannels == 1)
	{
		buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
	}
	else
	{
		for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
			buffer.clear(i, 0, numSamples);
	}

	if (mClearDelayRequested.exchange(false))
	{
		mDelayBuffer.clear();
	}
	if (mResetReverbRequested.exchange(false))
	{
		mReverb.reset();
		mReverbBuffer.clear();
	}

	double hostBpm = DEFAULT_BPM;
	if (auto* hostPlayHead = getPlayHead())
	{
		if (auto position = hostPlayHead->getPosition())
		{
			if (auto bpm = position->getBpm(); bpm && *bpm > 0.0)
			{
				hostBpm = *bpm;
			}
		}
	}
	mBpm = hostBpm;

	for (int channel = 0; channel < 2; channel++)
	{
		for (Envelope* envelope : { &revEnvelope[channel], &delEnvelope[channel] })
		{
			envelope->setParams(mParams.scAttack->load(),
				mParams.scAttackShape->load(),
				mParams.scDecay->load(),
				mParams.scDecayShape->load(),
				mParams.scSustainLevel->load(),
				mParams.scSustain->load(),
				mParams.scRelease->load(),
				mParams.scReleaseShape->load(),
				mParams.scThreshold->load());
		}
	}
	//=============================================================================

	// Hosts may send more samples than prepareToPlay() promised, so process in
	// chunks that fit the scratch buffers
	const float panVal = mParams.pan->load();
	for (int start = 0; start < numSamples; start += mMaxBlockSize)
	{
		AudioBuffer<float> chunk(buffer.getArrayOfWritePointers(), 2, start, jmin(mMaxBlockSize, numSamples - start));

		pan(chunk, panVal);

		if (mParams.delayOnOff->load() > 0.5f)
		{
			delay(chunk);
		}
		if (mParams.revOnOff->load() > 0.5f)
		{
			// TODO: fade this out
			reverb(chunk, panVal);
		}
	}
}

void SpacePanAudioProcessor::truePan(AudioBuffer<float> &buffer, float panVal, float maxPan)
{
	// TODO: This is faux pan placeholder. Change to true pan algorithm
	panVal *= maxPan;
	float headWidth = mParams.headWidth->load();
	int bufferLength = buffer.getNumSamples();
	float distanceFactor = mParams.roomSize->load();
	float attenuationFactor = 1 - distanceFactor * std::pow(panVal, 2.0f);
	float attenuationFactorL = panVal < 0 ? attenuationFactor : 1;
	float attenuationFactorR = panVal > 0 ? attenuationFactor : 1;
	for (int i = 0; i < bufferLength; i++)
	{
		buffer.getWritePointer(0)[i] = (1 - panVal)*buffer.getReadPointer(0)[i] * attenuationFactorL;
		buffer.getWritePointer(1)[i] = (1 + panVal)*buffer.getReadPointer(1)[i] * attenuationFactorR;
	}

	// Delay the channel furthest from the source by the time sound takes to cross the head
	const float phaseShiftMaxInSamples = (headWidth / SOUND_SPEED) * (float)getSampleRate();
	const int delayedChannel = (panVal < 0) ? 1 : 0;
	for (int channel = 0; channel < 2; channel++)
	{
		// Read position is set before writing because .write() updates writePosition
		if (channel == delayedChannel)
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel) - (int)(phaseShiftMaxInSamples * std::abs(panVal)));
		}
		else
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel));
		}
		mPanBuffer.write(channel, buffer);
		mPanBuffer.read(channel, buffer);
	}
}

void SpacePanAudioProcessor::reverb(AudioBuffer<float> &buffer, float panVal)
{
	const double sampleRate = getSampleRate();
	const int numSamples = buffer.getNumSamples();
	const float roomSize = mParams.roomSize->load();

	mReverb.setParameters(getReverbParameters());


	//=============================================================================================
	// Apply reverb and filters

	AudioBuffer<float>& reverbWet = mReverbWetBuffer;
	reverbWet.setSize(2, numSamples, false, false, true);
	for (int channel = 0; channel < 2; channel++)
	{
		reverbWet.copyFrom(channel, 0, buffer, channel, 0, numSamples);
	}

	float revLPf = clampFrequency(mParams.revLowpass->load(), sampleRate);
	float revLPQ = mParams.revLowpassQ->load();
	float revHPf = clampFrequency(mParams.revHighpass->load(), sampleRate);
	float revHPQ = mParams.revHighpassQ->load();

	setCoefficients(revLowPassFilter, dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, revLPf, revLPQ));
	setCoefficients(revHighPassFilter, dsp::IIR::ArrayCoefficients<float>::makeHighPass(sampleRate, revHPf, revHPQ));

	dsp::AudioBlock<float> block(reverbWet);
	mReverb.process(dsp::ProcessContextReplacing<float>(block));
	revLowPassFilter.process(dsp::ProcessContextReplacing<float>(block));
	revHighPassFilter.process(dsp::ProcessContextReplacing<float>(block));
	//=============================================================================================


	// Pre-delay the reverb by the time to reach a wall and return. The far ear
	// hears it later than the near ear.
	for (int channel = 0; channel < 2; channel++)
	{
		float dist;
		if ((channel == 0 && panVal < 0) || (channel == 1 && panVal >0))
		{
			dist = roomSize * ROOM_SIZE_MAX * (2.0f - std::abs(panVal));
		}
		else
		{
			dist = roomSize * ROOM_SIZE_MAX * (2.0f + std::abs(panVal));
		}
		float ppdFactor = dist / SOUND_SPEED;
		int ppdFactorInSamples = static_cast<int>(ppdFactor * sampleRate);


		mReverbBuffer.setReadPosition(channel, mReverbBuffer.getWritePosition(channel) - ppdFactorInSamples);
		mReverbBuffer.write(channel, reverbWet);
		mReverbBuffer.read(channel, reverbWet);
	}



	float reverbMix = mParams.revMix->load();
	const float scAmount = mParams.revScAmount->load();
	const bool isSidechainOn = mParams.scOnOff->load() > 0.5f;
	for (int channel = 0; channel < 2; ++channel)
	{

		// Apply sidechain
		if (scAmount > 0.01f && isSidechainOn)
		{
			float* wet = reverbWet.getWritePointer(channel);
			const float* dry = buffer.getReadPointer(channel);
			for (int i = 0; i < numSamples; i++)
			{
				revEnvelope[channel].trigger(dry[i]);
				wet[i] *= (1 - revEnvelope[channel].getGain() * scAmount);
				revEnvelope[channel].update(1.0f / (float)sampleRate);
			}
		}

		// Mix wet and dry signals
		mixer(buffer, reverbWet, reverbMix, channel, 1.0f);
		buffer.copyFrom(channel, 0, reverbWet, channel, 0, numSamples);
	}
}



void SpacePanAudioProcessor::pan(AudioBuffer<float> &buffer, float panVal)
{

	truePan(buffer, panVal, 0.8f);

	dsp::AudioBlock<float> block(buffer);
	dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
	dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
	double fmax = 20.0e3;
	double rangeMin = 2000;
	double fmin = 4000;

	// Roll off high frequencies in the ear shadowed by the head
	double fcL = std::max(fmax - (fmax - rangeMin)*log2(std::abs(panVal) + 1), fmin);
	double fcR = std::max(fmax - (fmax - rangeMin)*log2(std::abs(panVal) + 1), fmin);
	if (panVal <= 0)
	{
		fcL = fmax;
	}

	if (panVal >= 0)
	{
		fcR = fmax;
	}

	const double sampleRate = getSampleRate();
	*lowPassFilterL.coefficients = dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, clampFrequency((float)fcL, sampleRate), 0.5f);
	*lowPassFilterR.coefficients = dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, clampFrequency((float)fcR, sampleRate), 0.5f);

	lowPassFilterL.process(dsp::ProcessContextReplacing<float>(blockL));
	lowPassFilterR.process(dsp::ProcessContextReplacing<float>(blockR));
}

void SpacePanAudioProcessor::mixer(AudioBuffer<float> &dry, AudioBuffer<float> &wet, float mix, int channel, float vol)
{



	// This might be faster using FloatVectorOperations!
	for (int i = 0; i < dry.getNumSamples(); i++)
	{
		wet.getWritePointer(channel)[i] = (1 - mix) *dry.getReadPointer(channel)[i] + mix * wet.getReadPointer(channel)[i];
		wet.getWritePointer(channel)[i] *= vol;
	}

	return;
}

float SpacePanAudioProcessor::getDelaySeconds() const
{
	float delaySeconds;
	if (mParams.delayTempoLock->load() > 0.5f)
	{
		const float modifier = delayModifierDP.getValue((int)mParams.delayModifier->load());
		const float delayInBarsTemp = delayInBarsDP.getValue((int)mParams.delayTimeDiscrete->load());
		delaySeconds = (float)(delayInBarsTemp * modifier * 60.0 * 4.0 / mBpm.load());
	}
	else
	{
		delaySeconds = mParams.delayTime->load();
	}

	// Long note values at slow tempos can exceed the buffer
	return jlimit(0.0f, DELAY_MAX, delaySeconds);
}

dsp::Reverb::Parameters SpacePanAudioProcessor::getReverbParameters() const
{
	const float roomSize = mParams.roomSize->load();

	dsp::Reverb::Parameters reverbParams;
	reverbParams.roomSize = roomSize * 0.8f;
	reverbParams.damping = 0.5f;
	reverbParams.wetLevel = 1.0f;
	reverbParams.dryLevel = 0.0f;
	reverbParams.width = roomSize;
	reverbParams.freezeMode = 0.0f;
	return reverbParams;
}

dsp::Reverb::Parameters SpacePanAudioProcessor::getDelayverbParameters() const
{
	// Diffuses the repeats. dsp::Reverb scales wet by 3 and dry by 2 internally,
	// so compensate to keep the repeats at unity gain. Without this, every repeat
	// was doubled.
	const float diffusion = mParams.delayDiffusion->load();

	dsp::Reverb::Parameters delayverbParams;
	delayverbParams.roomSize = 0.0f;
	delayverbParams.damping = 1.0f;
	delayverbParams.wetLevel = diffusion / 3.0f;
	delayverbParams.dryLevel = (1 - diffusion) / 2.0f;
	delayverbParams.width = mParams.delayWidth->load();
	delayverbParams.freezeMode = 0.0f;
	return delayverbParams;
}

void SpacePanAudioProcessor::delay(AudioBuffer<float> &samples)
{
	const int numSamples = samples.getNumSamples();
	const double sampleRate = getSampleRate();
	CircularAudioBuffer<float>& delayBuffer = mDelayBuffer;

	const float delaySeconds = getDelaySeconds();
	int delayInSamples = std::max<int>(1, (int)(delaySeconds * sampleRate)); // minimum 1 sample delay

	AudioBuffer<float>& delayDry = mDelayDryBuffer;
	delayDry.setSize(2, numSamples, false, false, true);
	for (int channel = 0; channel < 2; ++channel)
	{
		delayDry.copyFrom(channel, 0, samples, channel, 0, numSamples);
	}

	float mDelayFeedbackGain = mParams.delayFeedback->load();
	float delayLPf = clampFrequency(mParams.delayLowpass->load(), sampleRate);
	float delayLPQ = mParams.delayLowpassQ->load();
	float delayHPf = clampFrequency(mParams.delayHighpass->load(), sampleRate);
	float delayHPQ = mParams.delayHighpassQ->load();
	const int filterType = (int)mParams.delayFilterType->load();

	setCoefficients(delayLowPassFilter, dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, delayLPf, delayLPQ));
	setCoefficients(delayHighPassFilter, dsp::IIR::ArrayCoefficients<float>::makeHighPass(sampleRate, delayHPf, delayHPQ));

		//========================================================================

	dsp::AudioBlock<float> preDelayBlock(samples);
	float width = mParams.delayWidth->load();
	delayverb.setParameters(getDelayverbParameters());

	int stereoType = (int)mParams.delayStereoType->load();

	// Widen
	int delayOffset[2] = { 0, 0 };
	if (stereoType == 2)
	{
		if (width < -0.01f)
		{
			delayOffset[0] = (int)(-width * WIDEN_DELAY_MAX * sampleRate);
		}
		else if (width > 0.01f)
		{
			delayOffset[1] = (int)(width * WIDEN_DELAY_MAX * sampleRate);
		}
	}

	if (filterType == 2)
	{
		delayLowPassFilter.process(dsp::ProcessContextReplacing<float>(preDelayBlock));
		delayHighPassFilter.process(dsp::ProcessContextReplacing<float>(preDelayBlock));
	}

	//========================================================================


	if (stereoType == 0)
	{
		for (int i = 0; i < numSamples; i++)
		{
			float lcurve = std::sin(PI * ((delayBuffer.getWritePosition(0) + i) / (float)delayBuffer.getLoopSize(0)));
			float rcurve = std::cos(PI * ((delayBuffer.getWritePosition(1) + i) / (float)delayBuffer.getLoopSize(1)));
			lcurve = width * lcurve * lcurve + 1 - std::max(0.0f, width);
			rcurve = width * rcurve * rcurve + 1 - std::max(0.0f, width);
			samples.getWritePointer(0)[i] *= lcurve;
			samples.getWritePointer(1)[i] *= rcurve;
		}
	}

	// Process the section of the delay line that is about to be read
	{
		AudioBuffer<float>& delayBufferTemp = mDelayTempBuffer;
		delayBufferTemp.setSize(2, numSamples, false, false, true);
		for (int channel = 0; channel < 2; ++channel)
		{
			delayBuffer.setLoopSize(channel, delayInSamples + delayOffset[channel]);
			delayBuffer.setReadPosition(channel, delayBuffer.getWritePosition(channel));
			delayBuffer.read(channel, delayBufferTemp);
			delayBuffer.moveReadPosition(channel, -numSamples);
		}
		// Add saturation to wet delay signal
		if (mParams.delaySat->load() > 0.1f)
		{
			saturate(delayBufferTemp, mParams.delaySat->load(), mParams.delaySatChar->load());
		}
		dsp::AudioBlock<float> delayBlock(delayBufferTemp);

		delayverb.process(dsp::ProcessContextReplacing<float>(delayBlock));

		if (filterType == 1)
		{
			setCoefficients(delayLowPassFilter, dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, delayLPf, delayLPQ / 5.0f));
			setCoefficients(delayHighPassFilter, dsp::IIR::ArrayCoefficients<float>::makeHighPass(sampleRate, delayHPf, delayHPQ / 5.0f));
			delayLowPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
			delayHighPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
		}

		for (int channel = 0; channel < 2; ++channel)
		{
			delayBuffer.write(channel, delayBufferTemp);
			delayBuffer.moveWritePosition(channel, -numSamples);
		}
	}

	// Feedback delay. The wet signal is the delayed signal, so the first echo
	// is at unity gain and each further echo is scaled by the feedback.
	AudioBuffer<float>& delayWet = mDelayWetBuffer;
	delayWet.setSize(2, numSamples, false, false, true);
	for (int channel = 0; channel < 2; ++channel)
	{
		float* data = delayBuffer.getWritePointer(channel);
		const float* input = samples.getReadPointer(channel);
		float* wet = delayWet.getWritePointer(channel);
		const int loopSize = delayBuffer.getLoopSize(channel);
		int pos = delayBuffer.getWritePosition(channel);
		for (int i = 0; i < numSamples; i++)
		{
			const float delayed = data[pos];
			wet[i] = delayed;
			data[pos] = softLimit(input[i] + delayed * mDelayFeedbackGain);
			if (++pos >= loopSize)
			{
				pos = 0;
			}
		}

		delayBuffer.moveWritePosition(channel, numSamples);
	}

	// Apply filters to wet delay signal
	if (filterType == 0)
	{
		dsp::AudioBlock<float> delayBlock(delayWet);
		delayLowPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
		delayHighPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
	}

	const float scAmount = mParams.delayScAmount->load();
	const bool isSidechainOn = mParams.scOnOff->load() > 0.5f;
	const float delayMix = mParams.delayMix->load();
	for (int channel = 0; channel < 2; ++channel)
	{
		// TODO: sidechain is expensive. Do it in blocks for performance
		// Apply sidechain
		if (scAmount > 0.01f && isSidechainOn)
		{
			float* wet = delayWet.getWritePointer(channel);
			const float* dry = delayDry.getReadPointer(channel);
			for (int i = 0; i < numSamples; i++)
			{
				delEnvelope[channel].trigger(dry[i]);
				wet[i] *= (1 - delEnvelope[channel].getGain() * scAmount);
				delEnvelope[channel].update(1.0f / (float)sampleRate);
			}
		}

		// Mix wet and dry signals
		mixer(delayDry, delayWet, delayMix, channel, 1.0f);
		samples.copyFrom(channel, 0, delayWet, channel, 0, numSamples);
	}
}


//==============================================================================
bool SpacePanAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpacePanAudioProcessor::createEditor()
{
    return new SpacePanAudioProcessorEditor (*this);
}

//==============================================================================
void SpacePanAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	auto state = mState.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void SpacePanAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(mState.state.getType()))
		{
			mState.replaceState(juce::ValueTree::fromXml(*xmlState));
		}
	}
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpacePanAudioProcessor();
}

void SpacePanAudioProcessor::saturate(AudioBuffer<float> &samples, float gain, float character)
{
	// Blend of three curves: digital (character 0), valve (0.5) and hard clip (1)
	float a = std::max(1.0f - 2.0f * character, 0.0f);
	float b = 2.0f * (0.5f - std::abs(character - 0.5f));
	float c = std::max(2.0f* character - 1.0f, 0.0f);

	// The valve curve turns back down beyond this input, so clamp there to keep
	// it monotonic and bounded
	const float valveLimit = std::sqrt(512.0f / (3.0f * gain * gain * gain));

	for (int channel = 0; channel < samples.getNumChannels(); channel++)
	{
		float* data = samples.getWritePointer(channel);
		for (int i = 0; i < samples.getNumSamples(); i++)
		{
			float sample = data[i];
			float x = std::atan(gain*sample) / gain; //digital
			float valveSample = jlimit(-valveLimit, valveLimit, sample);
			float y = valveSample - std::pow(valveSample / 8.0f * gain, 3.0f); //valve
			float z = jlimit(-1.0f, 1.0f, sample * gain) / gain; //hard clip
			data[i] = a * x + b * y + c * z;
		}

	}
	return;
}
