/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils.h"

//==============================================================================
SpacePanAudioProcessor::SpacePanAudioProcessor() : mState(*this, nullptr, "state",
	{ std::make_unique<AudioParameterFloat>("rev_mix", "Reverb Mix", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("pan", "Pan", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
	  std::make_unique<AudioParameterFloat>("delay_feedback", "Delay Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.5f),
	  std::make_unique<AudioParameterFloat>("delay_time", "Delay Time", NormalisableRange<float>(0.0f, 1.0f), 0.5f) })
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


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

		//const float* bufferData = buffer.getReadPointer(channel);
		//const float* delayBufferData = mDelayBuffer.getReadPointer(channel);

		// Copy to delay buffer

		float delaySeconds = mState.getParameter("delay_time")->getValue();
		int delayInSamples = (int) (delaySeconds * sampleRate);
		//delayInSamples = 22050; // remove this
		float mDelayFeedbackGain = mState.getParameter("delay_feedback")->getValue();
		









		//=============================================================================

		

		
		//mWritePosition = mDelayBuffer.getWritePosition(channel);
		//fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);


		float rampGain;
		float newReadPosition = utils::modulo(mDelayBuffer.getWritePosition(channel) - delayInSamples, delayBufferLength);
		rampGain = (mDelayBuffer.getReadPosition(channel) == newReadPosition) ? 1.0f : 0.0f;
		// Read position must be set before .write() is called
		mDelayBuffer.setReadPosition(channel,  newReadPosition);
		mDelayBuffer.write(channel, buffer); // This also updates the write position
		mDelayBuffer.read(channel, buffer, rampGain);


		/* Old function to copy data from delay buffer */
		//getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData, delayInSamples,
		//	mDelayBuffer.getReadPosition(channel));


		
		//=============================================================================



		
		
    }

}



void SpacePanAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, 
												const int delayBufferLength, const float* bufferData, const float* delayBufferData,
												int delaySamples, int readPosition)
{
	
	
	if (delayBufferLength > bufferLength + readPosition)
	{
		buffer.addFromWithRamp(channel, 0, delayBufferData + readPosition, bufferLength, 1.0, 1.0);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - readPosition;
		buffer.addFromWithRamp(channel, 0, delayBufferData + readPosition, bufferRemaining, 1.0, 1.0);
		buffer.addFromWithRamp(channel, bufferRemaining, delayBufferData, delayBufferLength - bufferRemaining, 1.0, 1.0);

	}


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
