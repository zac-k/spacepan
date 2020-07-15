/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils.cpp"

const float PI = 3.1415926535897932384626;
const float HEAD_WIDTH_MAX = 10; //metres
const float ROOM_SIZE_MAX = 100; //metres
const float SOUND_SPEED = 343.0; // m/s
const float FILTER_SKEW = 0.25f;
const float ATTACK_MAX = 0.2f;
const float DECAY_MAX = 0.2f;
const float SUSTAIN_MAX = 0.5f;
const float RELEASE_MAX = 1.0f;


//==============================================================================
SpacePanAudioProcessor::SpacePanAudioProcessor() : mState(*this, nullptr, "state",
	{ std::make_unique<AudioParameterFloat>("rev_mix", "Reverb Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("rev_lowpass", "Reverb Lowpass", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("rev_lowpass_Q", "Reverb Lowpass Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("rev_highpass", "Reverb Highpass", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("rev_sc_amount", "Reverb Sidechain Amount", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("rev_highpass_Q", "Reverb Highpass Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("pan", "Pan", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("room_size", "Room Size", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("head_width", "Head Width", NormalisableRange<float>(0.0f, 10.0f), 0.15f),
	  std::make_unique<AudioParameterFloat>("delay_feedback", "Delay Feedback", NormalisableRange<float>(0.005f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_time", "Delay Time", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_lowpass", "Delay High Cut", utils::frequencyRange(100.0f, 2.0e4f), 2.0e3f),
	  std::make_unique<AudioParameterFloat>("delay_lowpass_Q", "Delay High Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("delay_highpass", "Delay Low Cut", utils::frequencyRange<float>(100.0f, 2.0e4f), 2.0e2f),
	  std::make_unique<AudioParameterFloat>("delay_highpass_Q", "Delay Low Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("delay_allpass", "Delay Diffusion", NormalisableRange<float>(20.0f, 2.0e4f), 20.0f),
	  std::make_unique<AudioParameterFloat>("delay_mix", "Delay Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_sat", "Delay Saturation", NormalisableRange<float>(0.0f, 10.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("delay_sat_char", "Delay Saturation Character", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_width", "Delay Width", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("sc_attack", "Sidechain Attack", NormalisableRange<float>(0.0f, ATTACK_MAX), 0.1f),
	  std::make_unique<AudioParameterFloat>("sc_decay", "Sidechain Decay", NormalisableRange<float>(0.0f, DECAY_MAX), 0.1f),
	  std::make_unique<AudioParameterFloat>("sc_sustain_level", "Sidechain Sustain Level", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("sc_sustain", "Sidechain Sustain", NormalisableRange<float>(0.0f, SUSTAIN_MAX), 0.1f),
	  std::make_unique<AudioParameterFloat>("sc_release", "Sidechain Release", NormalisableRange<float>(0.0f, RELEASE_MAX), 0.1f),
	  std::make_unique<AudioParameterFloat>("sc_threshold", "Sidechain Threshold", NormalisableRange<float>(0.0f, 1.0f), 0.5f) })
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	// TODO: reverse delay lowpass knob so it's a "high cut" knob
}

void SpacePanAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{
	
}

SpacePanAudioProcessor::~SpacePanAudioProcessor()
{
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
    return 0.0;
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

void SpacePanAudioProcessor::setCurrentProgram (int index)
{
}

const String SpacePanAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpacePanAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpacePanAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	
	const int numInputChannels = getTotalNumInputChannels();
	const int delayBufferSize = static_cast<int>(DELAY_MAX * sampleRate + 2 * samplesPerBlock);

	mDelayBuffer.setSize(numInputChannels, delayBufferSize);
	mDelayBuffer.clear();
	mDelayBuffer.initWritePosition();
	mDelayBuffer.initReadPosition();

	const int panBufferSize = static_cast<int>(HEAD_WIDTH_MAX / SOUND_SPEED * sampleRate + 2 * samplesPerBlock);
	mPanBuffer.setSize(numInputChannels, panBufferSize);
	mPanBuffer.clear();
	mPanBuffer.initWritePosition();
	mPanBuffer.initReadPosition();

	const int preverbBufferSize = static_cast<int>(ROOM_SIZE_MAX / SOUND_SPEED * sampleRate + 2 * samplesPerBlock);
	mPreverbBuffer.setSize(numInputChannels, preverbBufferSize);
	mPreverbBuffer.clear();
	mPreverbBuffer.initWritePosition();
	mPreverbBuffer.initReadPosition();

	const int reverbBufferSize = static_cast<int>(ROOM_SIZE_MAX / SOUND_SPEED * sampleRate + 2 * samplesPerBlock);
	mReverbBuffer.setSize(numInputChannels, reverbBufferSize);
	mReverbBuffer.clear();
	mReverbBuffer.initWritePosition();
	mReverbBuffer.initReadPosition();


	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	lowPassFilterL.prepare(spec);
	lowPassFilterR.prepare(spec);

	
	delayLowPassFilter.prepare(spec);
	delayLowPassFilter.reset();
	delayHighPassFilter.prepare(spec);
	delayHighPassFilter.reset();
	delayAllPassFilter.prepare(spec);
	delayAllPassFilter.reset();
	lowPassFilterL.reset();
	lowPassFilterR.reset();

	revLowPassFilter.prepare(spec);
	revLowPassFilter.reset();
	revHighPassFilter.prepare(spec);
	revHighPassFilter.reset();

	preverb.prepare(spec);
	preverb.reset();

	mReverb.prepare(spec);
	mReverb.reset();

	delayverb.prepare(spec);
	delayverb.reset();
	//}

	

	adsrPlot = Image(Image::ARGB, 150, 80, true);

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
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpacePanAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	const float adsrTimeMax = ATTACK_MAX + DECAY_MAX + SUSTAIN_MAX + RELEASE_MAX;
	float tAttack = *mState.getRawParameterValue("sc_attack");
	float tDecay = *mState.getRawParameterValue("sc_decay");
	float tSustain = *mState.getRawParameterValue("sc_sustain");
	float tRelease = *mState.getRawParameterValue("sc_release");
	// TODO: change these shape factors to use GUI
	float shapeAttack = 1.0;
	float shapeDecay = 1.0;
	float sustainGain = *mState.getRawParameterValue("sc_sustain_level");
	float shapeRelease = 1.0;
	Colour traceColour = Colours::lightgreen;
	adsrPlot.clear(adsrPlot.getBounds(), Colours::black);
	// TODO: move the plot drawing code to a parameter changed callback

	int pixTemp = adsrPlot.getHeight();
	for (int i = 0; i < adsrPlot.getWidth(); i++)
	{
		float val;
		float t = i * adsrTimeMax / adsrPlot.getWidth();
		if (t < tAttack)
		{
			val = std::pow(t / tAttack, shapeAttack);
		}
		else if (t <  tAttack + tDecay)
		{
			val = 1 - std::pow((t-tAttack) / tDecay, shapeDecay) * (1 - sustainGain);
		}
		else if (t < tAttack + tDecay + tSustain)
		{
			val = sustainGain;
		}
		else
		{
			val = (1 - std::pow((t-tAttack -tDecay-tSustain) / tRelease, shapeRelease)) * sustainGain;
		}
		int pix = (int)((1 - val) * adsrPlot.getHeight());
		while (std::abs(pix - pixTemp) > 0)
		{
			pixTemp = pixTemp + copysign(1, pix - pixTemp);
			adsrPlot.setPixelAt(i, pixTemp, traceColour);
		}
		adsrPlot.setPixelAt(i, pix, traceColour);
		/*while (std::abs(valPrev - val) > 1/adsrPlot.getHeight() )
		{
			adsrPlot.setPixelAt(i, (int)((1 - val) * adsrPlot.getHeight()), traceColour);
			valPrev = valPrev + copysign(1.0, val - valPrev) / adsrPlot.getHeight();
		}*/
	}
	
	
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	double sampleRate = getSampleRate();
	

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
		
	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{


		revEnvelope[channel].setParams(tAttack,
			shapeAttack, 
			tDecay,
			shapeDecay,
			sustainGain,
			tSustain,
			tRelease,
			shapeRelease,
			*mState.getRawParameterValue("sc_threshold"));
	}
	//=============================================================================

		
	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = mDelayBuffer.getNumSamples();
	//=============================================================================
	pan(buffer, *mState.getRawParameterValue("pan"));
	//=============================================================================

	

	// TODO: Control this via GUI and put inside delay function
	float delayOffsets[] = { 0, 0 };
	delay(buffer, mDelayBuffer, buffer.getNumSamples(), delayOffsets, sampleRate, 0, true);		

	reverb(buffer, *mState.getRawParameterValue("pan"));

		//=============================================================================
		/*
		

		
		//mWritePosition = mDelayBuffer.getWritePosition(channel);
		//fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);


		//float rampGain;
		//float newReadPosition = utils::modulo(mDelayBuffer.getWritePosition(channel) - delayInSamples, delayBufferLength);
		//rampGain = (mDelayBuffer.getReadPosition(channel) == newReadPosition) ? 1.0f : 0.0f;
		//// Read position must be set before .write() is called
		//mDelayBuffer.setReadPosition(channel,  newReadPosition);
		//mDelayBuffer.write(channel, buffer); // This also updates the write position
		//mDelayBuffer.read(channel, buffer, rampGain);


		// Old function to copy data from delay buffer 
		//getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData, delayInSamples,
		//	mDelayBuffer.getReadPosition(channel));


		
		//=============================================================================

		*/

		
		
   

}

void SpacePanAudioProcessor::truePan(AudioBuffer<float> &buffer, float panVal, float maxPan)
{
	// TODO: This is faux pan placeholder. Change to true pan algorithm
	panVal *= maxPan;
	AudioBuffer<float> bufferTemp;
	bufferTemp.makeCopyOf(buffer);
	int bufferLength = buffer.getNumSamples();
	float distanceFactor = *mState.getRawParameterValue("room_size");
	float attenuationFactor = 1 - distanceFactor * std::pow(panVal, 2);
	float attenuationFactorL = panVal < 0 ? attenuationFactor : 1;
	float attenuationFactorR = panVal > 0 ? attenuationFactor : 1;
	for (int i = 0; i < bufferLength; i++)
	{
		buffer.getWritePointer(0)[i] = (1 - panVal)*buffer.getReadPointer(0)[i] * attenuationFactorL;
		buffer.getWritePointer(1)[i] = (1 + panVal)*buffer.getReadPointer(1)[i] * attenuationFactorR;
	}
	
	

}

void SpacePanAudioProcessor::reverb(AudioBuffer<float> &buffer, float panVal)
{
	dsp::Reverb::Parameters reverbParams;
	reverbParams.roomSize = *mState.getRawParameterValue("room_size") * 0.8;
	reverbParams.damping = 0.5f;

	reverbParams.wetLevel = 1.0f;
	reverbParams.dryLevel = 0.0f;
	reverbParams.width = *mState.getRawParameterValue("room_size");
	reverbParams.freezeMode = 0.0f;
	mReverb.setParameters(reverbParams);

	//truePan(buffer, panVal, maxPan);

	float headWidth = *mState.getRawParameterValue("head_width");

	// Make mono
	AudioBuffer<float> reverbWet;
	reverbWet.makeCopyOf(buffer);
	reverbWet.addFrom(0, 0, reverbWet.getReadPointer(1), reverbWet.getNumSamples());
	reverbWet.copyFrom(1, 0, reverbWet.getReadPointer(0), reverbWet.getNumSamples());
	reverbWet.applyGain(0.5);



	dsp::AudioBlock<float> block(reverbWet);
	mReverb.process(dsp::ProcessContextReplacing<float>(block));


	int phaseShiftMaxInSamples = (headWidth / SOUND_SPEED) * getSampleRate();
	float ppdFactor = std::pow(ROOM_SIZE_MAX, *mState.getRawParameterValue("room_size")) / SOUND_SPEED;
	int ppdFactorInSamples = (int)ppdFactor * getSampleRate();
	int reverbPredalay[] = { (int)ppdFactorInSamples * (1 + panVal), (int)ppdFactorInSamples * (1 - panVal) };

	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		int delayedChannel = (panVal < 0) ? 1 : 0;
		// Read position is set before writing because .write() updates writePosition
		if (channel == delayedChannel)
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel) - phaseShiftMaxInSamples * abs(panVal));
		}
		else
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel));
		}
		mPanBuffer.write(channel, buffer);

		mReverbBuffer.setReadPosition(channel, mReverbBuffer.getWritePosition(channel) - reverbPredalay[channel]);
		mReverbBuffer.write(channel, reverbWet);
	}

	float revLPf = *mState.getRawParameterValue("rev_lowpass");
	float revLPQ = *mState.getRawParameterValue("rev_lowpass_Q");
	float revHPf = *mState.getRawParameterValue("rev_highpass");
	float revHPQ = *mState.getRawParameterValue("rev_highpass_Q");


	// TODO: get these filters working
	*revLowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), revLPf, revLPQ);
	*revHighPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), revHPf, revHPQ);

	
	revLowPassFilter.process(dsp::ProcessContextReplacing<float>(block));
	revHighPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	float reverbMix = *mState.getRawParameterValue("rev_mix");
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		mPanBuffer.read(channel, buffer);
		mReverbBuffer.read(channel, reverbWet);

		// Apply sidechain
		// TODO: use the GUI to control level
		for (int i = 0; i < reverbWet.getNumSamples(); i++)
		{
			// TODO: this is placeholder. make stereo and use different envelope for
			// delay (four envelopes total)
			revEnvelope[channel].trigger(buffer.getReadPointer(channel)[i]);
			
			reverbWet.getWritePointer(channel)[i] *= (1 - revEnvelope[channel].getGain() * *mState.getRawParameterValue("rev_sc_amount"));
			// TODO: update envelope at end of process block instead of here
			revEnvelope[channel].update(1.0f / getSampleRate());
		}

		// Mix wet and dry signals
		mixer(buffer, reverbWet, reverbMix, channel, 1.0f);

	}

	buffer = reverbWet;


	return;
}

void SpacePanAudioProcessor::atmoPan(AudioBuffer<float> &buffer, float panVal, float maxPan)
{
	dsp::Reverb::Parameters preverbParams;
	preverbParams.roomSize = *mState.getRawParameterValue("room_size") * 0.8;
	preverbParams.damping = 0.5f;
	
	preverbParams.wetLevel = 1.0f;
	preverbParams.dryLevel = 0.0f;
	preverbParams.width = *mState.getRawParameterValue("room_size");
	preverbParams.freezeMode = 0.0f;
	preverb.setParameters(preverbParams);

	truePan(buffer, panVal, maxPan);
	
	float headWidth = *mState.getRawParameterValue("head_width");

	// Make mono
	AudioBuffer<float> preverbWet;
	preverbWet.makeCopyOf(buffer);	
	preverbWet.addFrom(0, 0, preverbWet.getReadPointer(1), preverbWet.getNumSamples());
	preverbWet.copyFrom(1, 0, preverbWet.getReadPointer(0), preverbWet.getNumSamples());
	preverbWet.applyGain(0.5);



	dsp::AudioBlock<float> block(preverbWet);
	//dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
	//dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
	preverb.process(dsp::ProcessContextReplacing<float>(block));
	//preverbR.process(dsp::ProcessContextReplacing<float>(blockR));


	int phaseShiftMaxInSamples = (headWidth / SOUND_SPEED) * getSampleRate();
	// TODO: The writing loop should be outside this function so the buffer is always written to, or
	// there should be a delay before the atmoPan 'kicks in' to allow the buffer to be filled
	float ppdFactor = std::pow(ROOM_SIZE_MAX, *mState.getRawParameterValue("room_size")) / SOUND_SPEED;
	int ppdFactorInSamples = (int)ppdFactor * getSampleRate();
	int preverbPredalay[] = { (int)ppdFactorInSamples * (1 + panVal), (int)ppdFactorInSamples * (1 - panVal) };

	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		int delayedChannel = (panVal < 0) ? 1 : 0;
		// Read position is set before writing because .write() updates writePosition
		if (channel == delayedChannel)
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel) - phaseShiftMaxInSamples * abs(panVal));
		}
		else
		{
			mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel));
		}
		mPanBuffer.write(channel, buffer);

		mPreverbBuffer.setReadPosition(channel, mPreverbBuffer.getWritePosition(channel) - preverbPredalay[channel]);
		mPreverbBuffer.write(channel, preverbWet);
	}	


	float preverbMix = *mState.getRawParameterValue("rev_mix");
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		mPanBuffer.read(channel, buffer);
		mPreverbBuffer.read(channel, preverbWet);
		// Mix wet and dry signals
		mixer(buffer, preverbWet, preverbMix, channel, 1.0f);

	}

	buffer = preverbWet;

	
	return;
}

void SpacePanAudioProcessor::pan(AudioBuffer<float> &buffer, float panVal)
{
	

	// TODO: maybe change this back to atmoPan(). Delete reverb stuff from atmoPan() and move the filtering stuff
	// from this (pan()) function into atmoPan().
	truePan(buffer, panVal, 0.8);

	dsp::AudioBlock<float> block(buffer);
	dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
	dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
	double nyquist = getSampleRate() / 2;
	double fmax = 20.0e3;
	double rangeMin = 2000;
	double fmin = 4000;
	double regParam = 0.1;
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


	fcL = std::min(fcL, nyquist);
	fcR = std::min(fcR, nyquist);


	*lowPassFilterL.coefficients = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), fcL, 0.5);
	*lowPassFilterR.coefficients = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), fcR, 0.5);
	
	int bufferLength = buffer.getNumSamples();

	mPanFcL = fcL;
	mPanFcR = fcR;

	
	lowPassFilterL.process(dsp::ProcessContextReplacing<float>(blockL));
	lowPassFilterR.process(dsp::ProcessContextReplacing<float>(blockR));

		
	//}

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

void SpacePanAudioProcessor::delay(AudioBuffer<float> &samples, CircularAudioBuffer<float> &delayBuffer, int numSamples,
	float* delayOffsets, float sampleRate, int32 comb, bool fb)
{

	AudioBuffer<float> delayDry;
	delayDry.makeCopyOf(samples);
	float delaySeconds = *mState.getRawParameterValue("delay_time");

	// TODO: This is for testing tempo-locked delay
	float delayInBars = 3.0 / 8.0;
	AudioPlayHead::CurrentPositionInfo cpi;
	//delaySeconds = delayInBars * 60.0 * 4 / cpi.bpm;

	int delayInSamples = std::max<int>(1, delaySeconds * sampleRate); // minimum 1 sample delay
	float mDelayFeedbackGain = *mState.getRawParameterValue("delay_feedback");
	// TODO: This should be controlled via GUI!
	String delayStereoType = "ping_pong";

	float delayLPf = *mState.getRawParameterValue("delay_lowpass");
	float delayLPQ = *mState.getRawParameterValue("delay_lowpass_Q");
	float delayHPf = *mState.getRawParameterValue("delay_highpass");
	float delayHPQ = *mState.getRawParameterValue("delay_highpass_Q");
	float delayAPf = *mState.getRawParameterValue("delay_allpass");


	// TODO: filtering needs to be done on signal added to delay buffer - maybe as separate option?

	*delayLowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), delayLPf, delayLPQ);
	*delayHighPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), delayHPf, delayHPQ);
	//*delayAllPassFilter.state = *dsp::IIR::Coefficients<float>::makeAllPass(getSampleRate(), delayAPf, 40.0);



	for (int channel = 0; channel < samples.getNumChannels(); ++channel)
	{





		// TODO: For ping pong delay, input pan should be modulated, and the modulated input passed to delay. Extraction of
		// the wet signal requires the modulated input, and the mixer should mix the unmodulated signal with the delay ouput.
		// Use the global pan algorithm for input modulation.


		//



		int32 delayOffsetInSamples = (int32)(delayOffsets[channel] * sampleRate);
		int32 tempDelayInSamples = std::max<int32>(1, delayInSamples + delayOffsetInSamples);
		/*float* arr;
		arr = (float*)malloc(sizeof(float)*numSamples);

		memcpy(arr, samples.getWritePointer(channel), sizeof(float)*numSamples);*/
		AudioBuffer<float> arr = AudioBuffer<float>::AudioBuffer(samples);
		//float* arr_temp;
		int32 tempBufferPos = delayBuffer.getWritePosition(channel);// mBufferPosArr[channel];

		// Use wet signal for feedback, or dry signal for feedforward delay
		//arr_temp = fb ? arr : samples.getReadPointer(channel);

		

		for (int32 i = 0; i < numSamples; i++)
		{
			arr.getWritePointer(channel)[i] += (delayBuffer.getReadPointer(channel)[tempBufferPos] * mDelayFeedbackGain);
			
			delayBuffer.getWritePointer(channel)[tempBufferPos] = arr.getReadPointer(channel)[i];
			//buffer.getWritePointer(channel)[tempBufferPos + tempDelayInSamples] = arr.getReadPointer(channel)[i];
			tempBufferPos++;
			if (tempBufferPos >= tempDelayInSamples)
			{
				tempBufferPos = 0;
			}



		}
		

		samples.copyFrom(channel, 0, arr.getReadPointer(channel), numSamples);

		delayBuffer.moveWritePosition(channel, numSamples, delayInSamples);
	}
	
	//========================================================================
		// TODO: delayverb (diffusion) and pre-filter not working
	dsp::AudioBlock<float> preDelayBlock(delayBuffer);
	dsp::Reverb::Parameters delayverbParams;
	delayverbParams.roomSize = 0.05;
	delayverbParams.damping = 1.0f;

	delayverbParams.wetLevel = 0.0f;
	delayverbParams.dryLevel = 0.95f;
	delayverbParams.width = 0.0;
	delayverbParams.freezeMode = 0.0f;
	delayverb.setParameters(delayverbParams);
	//delayverb.process(dsp::ProcessContextReplacing<float>(preDelayBlock));

	
	//delayLowPassFilter.process(dsp::ProcessContextReplacing<float>(preDelayBlock));
	//delayHighPassFilter.process(dsp::ProcessContextReplacing<float>(preDelayBlock));
	//========================================================================


	float delayWidth = *mState.getRawParameterValue("delay_width");
	AudioBuffer<float> delayWet;
	delayWet.makeCopyOf(samples);




	

	for (int channel = 0; channel < samples.getNumChannels(); ++channel)
	{
		// Extract pure wet signal from delay buffer
		FloatVectorOperations::subtract(delayWet.getWritePointer(channel), delayDry.getReadPointer(channel), samples.getNumSamples());

		// Compensate for gain drop on first echo
		delayWet.applyGain(channel, 0, samples.getNumSamples(), 1.0f / mDelayFeedbackGain);

	}

	// Add saturation to wet delay signal
	if (*mState.getRawParameterValue("delay_sat") > 0.1)
	{
		saturate(delayWet, *mState.getRawParameterValue("delay_sat"), *mState.getRawParameterValue("delay_sat_char"));
	}

	// Apply filters to wet delay signal
	dsp::AudioBlock<float> delayBlock(delayWet);
	delayLowPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
	delayHighPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));
	//delayAllPassFilter.process(dsp::ProcessContextReplacing<float>(delayBlock));



	for (int channel = 0; channel < samples.getNumChannels(); ++channel)
	{
		// Mix wet and dry signals
		mixer(delayDry, delayWet, *mState.getRawParameterValue("delay_mix"), channel, 1.0f);

	}


	samples.makeCopyOf(delayWet); // This might not be best way.

}

float* SpacePanAudioProcessor::combFilter(float* samples, float** buffer, int channel, int numSamples,
	int32 delayInSamples, float** delayOffsets, float decay,
	float sampleRate, int32 comb, bool fb)
{

	//int32 offsetInSamples = (int32)(delayOffset * sampleRate);
	int32 delayOffsetInSamples = (int32)(delayOffsets[channel][comb] * sampleRate);
	int32 tempDelayInSamples = std::max<int32>(1, delayInSamples + delayOffsetInSamples);
	float* arr;
	arr = (float*)malloc(sizeof(float)*numSamples);

	memcpy(arr, samples, sizeof(float)*numSamples);

	float* arr_temp;
	int32 tempBufferPos = mBufferPosArr[channel];

	// Use wet signal for feedback, or dry signal for feedforward delay
	arr_temp = fb ? arr : samples;

	for (int32 i = 0; i < numSamples; i++)
	{
		arr[i] += (buffer[channel][tempBufferPos] * decay);
		buffer[channel][tempBufferPos] = arr_temp[i];
		buffer[channel][tempBufferPos + tempDelayInSamples] = arr_temp[i];
		tempBufferPos++;
		if (tempBufferPos >= tempDelayInSamples)
		{
			tempBufferPos = 0;
		}

	}

	return arr;
}




void SpacePanAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, 
												const int delayBufferLength, const float* bufferData, const float* delayBufferData,
												int delaySamples, int readPosition)
{
	const int bufferRemaining = delayBufferLength - readPosition;
	buffer.addFromWithRamp(channel, 0, delayBufferData + readPosition, std::min(bufferLength, bufferRemaining), 1.0, 1.0);
	buffer.addFromWithRamp(channel, bufferRemaining, delayBufferData, std::max(0, delayBufferLength - bufferRemaining), 1.0, 1.0);
	
	

}

//void SpacePanAudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength,
//											 const float* bufferData, const float* delayBufferData)
//{
//
//	if (delayBufferLength > bufferLength + mWritePosition)
//	{
//		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 1.0, 1.0);
//	}
//	else
//	{
//		const int bufferRemaining = delayBufferLength - mWritePosition;
//		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, 1.0, 1.0);
//
//		mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, 1.0, 1.0);
//	}
//
//}



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
}

void SpacePanAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpacePanAudioProcessor();
}

void SpacePanAudioProcessor::saturate(float &sample, float gain)
{
	sample = atan(gain*sample) / gain;
	return;
}

void SpacePanAudioProcessor::saturate(AudioBuffer<float> &samples, float gain, int type)
{

	for (int channel = 0; channel < samples.getNumChannels(); channel++)
	{
		for (int i = 0; i < samples.getNumSamples(); i++)
		{
			float sample = samples.getReadPointer(channel)[i];
			switch (type)
			{
			case 0: // digital
				samples.getWritePointer(channel)[i] = atan(gain*sample) / gain;
				break;
			case 1: // valve
				samples.getWritePointer(channel)[i] = sample - std::pow(sample / 10.0f * gain, 3.0f);
				break;
			case 2: // hard clip
				samples.getWritePointer(channel)[i] = std::min(sample*gain, 1.0f) / gain;
				break;

			}
			
			
		}

	}
	return;
}

void SpacePanAudioProcessor::saturate(AudioBuffer<float> &samples, float gain, float character)
{
	float a = std::max(1.0f - 2.0f * character, 0.0f);
	float b = 2.0f * (0.5f - std::abs(character - 0.5f));
	float c = std::max(2.0f* character - 1.0f, 0.0f);

	for (int channel = 0; channel < samples.getNumChannels(); channel++)
	{
		for (int i = 0; i < samples.getNumSamples(); i++)
		{
			float sample = samples.getReadPointer(channel)[i];
			float x = atan(gain*sample) / gain; //digital
			float y = sample - std::pow(sample / 8.0f * gain, 3.0f); //valve
			float z =  std::min(sample*gain, 1.0f) / gain; //hard clip
			samples.getWritePointer(channel)[i] = a * x + b * y + c * z;
		}

	}
	return;
}


