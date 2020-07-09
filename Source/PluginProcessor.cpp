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
const float SOUND_SPEED = 343.0; // m/s
const float FILTER_SKEW = 0.25f;
//==============================================================================
SpacePanAudioProcessor::SpacePanAudioProcessor() : mState(*this, nullptr, "state",
	{ std::make_unique<AudioParameterFloat>("rev_mix", "Reverb Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("pan", "Pan", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("head_width", "Head Width", NormalisableRange<float>(0.0f, 10.0f), 0.15f),
	  std::make_unique<AudioParameterFloat>("delay_feedback", "Delay Feedback", NormalisableRange<float>(0.005f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_time", "Delay Time", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_lowpass", "Delay High Cut", utils::frequencyRange(100.0f, 2.0e4f), 2.0e3f),
	  std::make_unique<AudioParameterFloat>("delay_lowpass_Q", "Delay High Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("delay_highpass", "Delay Low Cut", utils::frequencyRange<float>(100.0f, 2.0e4f), 2.0e2f),
	  std::make_unique<AudioParameterFloat>("delay_highpass_Q", "Delay Low Cut Q", NormalisableRange<float>(1.0f, 5.0f), 1.0f),
	  std::make_unique<AudioParameterFloat>("delay_allpass", "Delay Diffusion", NormalisableRange<float>(20.0f, 2.0e4f), 20.0f),
	  std::make_unique<AudioParameterFloat>("delay_mix", "Delay Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_width", "Delay Width", NormalisableRange<float>(0.0f, 1.0f), 0.5f) })
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
	if (parameterID == "rev_mix")
	{

		//SpacePanAudioProcessorEditor::mRevMixKnob.setValue(mRevMixParam.getValue());
	}
	
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
	// TODO: the factor of 100 here is to reduce the number of glitches from the buffer wraparound.
	// Remove it when the circular buffer methods are fixed.
	const int panBufferSize = static_cast<int>(HEAD_WIDTH_MAX / SOUND_SPEED * sampleRate + 2 * samplesPerBlock)*100;
	mPanBuffer.setSize(numInputChannels, panBufferSize);
	mPanBuffer.clear();
	mPanBuffer.initWritePosition();
	mPanBuffer.initReadPosition();


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
	//}
	

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
		

	//=============================================================================

		
	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = mDelayBuffer.getNumSamples();
	//=============================================================================
	pan(buffer, *mState.getRawParameterValue("pan"));
	//=============================================================================


	// TODO: Control this via GUI and put inside delay function
	float delayOffsets[] = { 0, 0 };
	delay(buffer, mDelayBuffer, buffer.getNumSamples(), delayOffsets, sampleRate, 0, true);		

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
	float distanceFactor = 0.8;
	float attenuationFactor = 1 - distanceFactor * std::pow(panVal, 4);
	for (int i = 0; i < bufferLength; i++)
	{
		buffer.getWritePointer(0)[i] = (1 - panVal)*buffer.getReadPointer(0)[i] * attenuationFactor;
		buffer.getWritePointer(1)[i] = (1 + panVal)*buffer.getReadPointer(1)[i] * attenuationFactor;
	}
	
	

}

void SpacePanAudioProcessor::atmoPan(AudioBuffer<float> &buffer, float panVal, float maxPan)
{
	truePan(buffer, panVal, maxPan);
	
	float headWidth = *mState.getRawParameterValue("head_width");

	int phaseShiftMaxInSamples = (headWidth / SOUND_SPEED) * getSampleRate();
	// TODO: The writing loop should be outside this function so the buffer is always written to, or
	// there should be a delay before the atmoPan 'kicks in' to allow the buffer to be filled

	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		// Read position is set before writing because .write() updates writePosition
		mPanBuffer.setReadPosition(channel, mPanBuffer.getWritePosition(channel) - phaseShiftMaxInSamples * abs(panVal));
		mPanBuffer.write(channel, buffer);
	}


	// Delay right channel if panned left and vice versa
	int channel = (panVal < 0) ? 1 : 0;

	if (panVal != 0)
	{
		mPanBuffer.read(channel, buffer);
	}
	
	return;
}

void SpacePanAudioProcessor::pan(AudioBuffer<float> &buffer, float panVal)
{
	

	atmoPan(buffer, panVal, 0.8);
	dsp::AudioBlock<float> block(buffer);
	dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
	dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
	double nyquist = getSampleRate() / 2;
	double fmax = 20.0e3;
	double rangeMin = 0;
	double fmin = 2000;
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


	*lowPassFilterL.coefficients = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), fcL);
	*lowPassFilterR.coefficients = *dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), fcR);
	
	int bufferLength = buffer.getNumSamples();

	mPanFcL = fcL;
	mPanFcR = fcR;

	// TODO: might be able to get rid of the processcontextreplacing
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
	int delayInSamples = std::max<int>(1, delaySeconds * sampleRate); // minimum 1 sample delay
	float mDelayFeedbackGain = *mState.getRawParameterValue("delay_feedback");
	// TODO: This should be controlled via GUI!
	String delayStereoType = "ping_pong";

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

		mDelayBuffer.moveWritePosition(channel, numSamples, delayInSamples);
	}


	float delayWidth = *mState.getRawParameterValue("delay_width");
	AudioBuffer<float> delayWet;
	delayWet.makeCopyOf(samples);




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
		// Extract pure wet signal from delay buffer
		FloatVectorOperations::subtract(delayWet.getWritePointer(channel), delayDry.getReadPointer(channel), samples.getNumSamples());


		// TODO: Set minimum feedback gain to above zero or regularise this equation
		// Compensate for gain drop on first echo
		delayWet.applyGain(channel, 0, samples.getNumSamples(), 1.0f / mDelayFeedbackGain);
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
