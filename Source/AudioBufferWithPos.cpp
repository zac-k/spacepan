

#include "AudioBufferWithPos.h"

//template <typename T>
//CircularAudioBuffer<T>::CircularAudioBuffer() : AudioBuffer<T>()
//{
//
//}
//
//template <typename T>
//CircularAudioBuffer<T>::CircularAudioBuffer(int numChannels, int numSamples) : AudioBuffer<T>(numChannels, numSamples)
//{
//
//	initWritePosition();
//	initReadPosition();
//	initLoopSize();
//}

template<typename T>
void CircularAudioBuffer<T>::setSize(int numChannels, int numSamples)
{
	AudioBuffer<T>::setSize(numChannels, numSamples);
	clear();
	initWritePosition();
	initReadPosition();
	initLoopSize();
}


template <typename T>
void CircularAudioBuffer<T>::initWritePosition()
{
	mWritePosition.clear();
	for (int i = 0; i < this->getNumChannels(); i++)
	{
		mWritePosition.push_back(0);
	}
	return;
}

template <typename T>
void CircularAudioBuffer<T>::initReadPosition()
{
	mReadPosition.clear();
	mExpectedReadPosition.clear();
	for (int i = 0; i < this->getNumChannels(); i++)
	{
		mReadPosition.push_back(0);
		mExpectedReadPosition.push_back(0);
	}
	return;
}

template <typename T>
void CircularAudioBuffer<T>::initLoopSize()
{
	mLoopSize.clear();
	for (int i = 0; i < this->getNumChannels(); i++)
	{
		mLoopSize.push_back(this->getNumSamples());
	}
	return;
}

template <typename T>
void CircularAudioBuffer<T>::setLoopSize(int channel, int loopSize)
{
	mLoopSize[channel] = loopSize;
	return;
}

template <typename T>
void CircularAudioBuffer<T>::moveWritePosition(int channel, int steps)
{
	mWritePosition[channel] = utils::modulo((mWritePosition[channel] + steps), this->mLoopSize[channel]);
	
	return;
}

template <typename T>
void CircularAudioBuffer<T>::moveWritePosition(int channel, int steps, int loopSize)
{

	mWritePosition[channel] += steps;
	while (loopSize && mWritePosition[channel] >= loopSize)
	{
		mWritePosition[channel] -= loopSize;
	}

	return;
}

template <typename T>
void CircularAudioBuffer<T>::moveReadPosition(int channel, int steps)
{
	mReadPosition[channel] = utils::modulo((mReadPosition[channel] + steps), this->mLoopSize[channel]);
	return;
}

template <typename T>
void CircularAudioBuffer<T>::moveReadPosition(int channel, int steps, int loopSize)
{
	mReadPosition[channel] += steps;
	while (loopSize && mReadPosition[channel] >= loopSize)
	{
		mReadPosition[channel] -= loopSize;
	}

	return;
}

template <typename T>
int CircularAudioBuffer<T>::getWritePosition(int channel)
{
	return mWritePosition[channel];
}

template <typename T>
int CircularAudioBuffer<T>::getLoopSize(int channel)
{
	return mLoopSize[channel];
}

template <typename T>
int CircularAudioBuffer<T>::getReadPosition(int channel)
{
	return mReadPosition[channel];
}

template <typename T>
void CircularAudioBuffer<T>::setReadPosition(int channel, int val)
{
	mReadPosition[channel] = utils::modulo(val, this->mLoopSize[channel]);
}

template <typename T>
void CircularAudioBuffer<T>::setWritePosition(int channel, int val)
{
	mWritePosition[channel] = utils::modulo(val, this->mLoopSize[channel]);
}

template <typename T>
void CircularAudioBuffer<T>::write(int channel, const AudioBuffer<T>& inputBuffer, bool isAdd)
{
	/* Write data into the circular buffer and update write position */

	int inputBufferLength = inputBuffer.getNumSamples();
	int thisBufferLength = this->mLoopSize[channel];
	int thisBufferRemaining = thisBufferLength - mWritePosition[channel];
	thisBufferRemaining = std::min(thisBufferRemaining, inputBufferLength);
	const float* inputData = inputBuffer.getReadPointer(channel);
	int inputBufferRemaining = std::max(inputBufferLength - thisBufferRemaining, 0);

	if (isAdd)
	{
		this->addFromWithRamp(channel, mWritePosition[channel], inputData, thisBufferRemaining, 1.0, 1.0);
		this->addFromWithRamp(channel, 0, inputData + thisBufferRemaining, inputBufferRemaining, 1.0, 1.0);
	}
	else
	{
	this->copyFromWithRamp(channel, mWritePosition[channel], inputData, thisBufferRemaining, 1.0, 1.0);
	this->copyFromWithRamp(channel, 0, inputData + thisBufferRemaining, inputBufferRemaining, 1.0, 1.0); 
	}
	moveWritePosition(channel, inputBufferLength);
}

// TODO: make these work for multiple channels in one go

template <typename T>
void CircularAudioBuffer<T>::read(int channel, AudioBuffer<T>& outputBuffer, bool isOverrideSmoothing, bool isAdd, float gain)
{
	
	/* Read data from the circular buffer and update read position */


	//int outputBufferLength = outputBuffer.getNumSamples();
	//int thisBufferLength = this->getNumSamples();
	//int thisBufferRemaining = thisBufferLength - mExpectedReadPosition[channel];
	//thisBufferRemaining = std::min(thisBufferRemaining, outputBufferLength);
	//int outputBufferRemaining = std::max(outputBufferLength - thisBufferRemaining, 0);
	//const float* thisData = this->getReadPointer(channel);

	////AudioBuffer<T>& tempBuffer;
	////const float* thisDataFromReadPos = this->getReadPointer(channel, mReadPosition[channel]);
	//if (isAdd)
	//{
	//	outputBuffer.addFromWithRamp(channel, 0, thisData + mExpectedReadPosition[channel], thisBufferRemaining, gain, gain);
	//	outputBuffer.addFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, gain, gain);
	//}
	//else
	//{
	//	outputBuffer.copyFromWithRamp(channel, 0, thisData + mExpectedReadPosition[channel], thisBufferRemaining, gain, gain);
	//	outputBuffer.copyFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, gain, gain);
	//}
	//
	//if (mExpectedReadPosition[channel] != mReadPosition[channel])
	//{
	//	// TODO: not sure if something needs to be done with isAdd in this block
	//	outputBuffer.applyGainRamp(channel, 0, outputBufferLength, 1.0f, 0.0f);
	//	thisBufferRemaining = thisBufferLength - mReadPosition[channel];
	//	thisBufferRemaining = std::min(thisBufferRemaining, outputBufferLength);
	//	outputBufferRemaining = std::max(outputBufferLength - thisBufferRemaining, 0);

	//	float midGain = (float)thisBufferRemaining / outputBufferLength * gain;

	//	outputBuffer.addFromWithRamp(channel, 0, thisData + mReadPosition[channel], thisBufferRemaining, 0.0f, midGain);
	//	outputBuffer.addFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, midGain, gain);
	//	
	//}


	int outputBufferLength = outputBuffer.getNumSamples();
	int thisBufferLength = this->mLoopSize[channel];
	int thisBufferRemaining = thisBufferLength - mExpectedReadPosition[channel];
	thisBufferRemaining = std::min(thisBufferRemaining, outputBufferLength);
	int outputBufferRemaining = std::max(outputBufferLength - thisBufferRemaining, 0);
	const float* thisData = this->getReadPointer(channel);

	AudioBuffer<T>& tempBuffer = AudioBuffer<T>::AudioBuffer(outputBuffer);
	//const float* thisDataFromReadPos = this->getReadPointer(channel, mReadPosition[channel]);

	tempBuffer.copyFromWithRamp(channel, 0, thisData + mExpectedReadPosition[channel], thisBufferRemaining, gain, gain);
	tempBuffer.copyFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, gain, gain);

	if (mExpectedReadPosition[channel] != mReadPosition[channel] && !isOverrideSmoothing)
	{
		// TODO: not sure if something needs to be done with isAdd in this block
		tempBuffer.applyGainRamp(channel, 0, outputBufferLength, 1.0f, 0.0f);
		thisBufferRemaining = thisBufferLength - mReadPosition[channel];
		thisBufferRemaining = std::min(thisBufferRemaining, outputBufferLength);
		outputBufferRemaining = std::max(outputBufferLength - thisBufferRemaining, 0);

		float midGain = (float)thisBufferRemaining / outputBufferLength * gain;

		tempBuffer.addFromWithRamp(channel, 0, thisData + mReadPosition[channel], thisBufferRemaining, 0.0f, midGain);
		tempBuffer.addFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, midGain, gain);

	}

	if (isAdd)
	{
		outputBuffer.addFromWithRamp(channel, 0, tempBuffer.getReadPointer(channel), outputBufferLength, 1.0f, 1.0f);
		//outputBuffer.addFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, gain, gain);
	}
	else
	{
		outputBuffer.copyFromWithRamp(channel, 0, tempBuffer.getReadPointer(channel), outputBufferLength, 1.0f, 1.0f);
		//outputBuffer.copyFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, gain, gain);
	}
	

	moveReadPosition(channel, outputBufferLength);
	mExpectedReadPosition[channel] = mReadPosition[channel];
}




