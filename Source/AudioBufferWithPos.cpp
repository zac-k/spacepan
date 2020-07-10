

#include "AudioBufferWithPos.h"

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
	for (int i = 0; i < this->getNumChannels(); i++)
	{
		mReadPosition.push_back(0);
	}
	return;
}

template <typename T>
void CircularAudioBuffer<T>::moveWritePosition(int channel, int steps)
{
	mWritePosition[channel] = utils::modulo((mWritePosition[channel] + steps), this->getNumSamples());
	
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
	mReadPosition[channel] = utils::modulo((mReadPosition[channel] + steps), this->getNumSamples());
	return;
}

template <typename T>
int CircularAudioBuffer<T>::getWritePosition(int channel)
{
	return mWritePosition[channel];
}

template <typename T>
int CircularAudioBuffer<T>::getReadPosition(int channel)
{
	return mReadPosition[channel];
}

template <typename T>
void CircularAudioBuffer<T>::setReadPosition(int channel, int val)
{
	mReadPosition[channel] = utils::modulo(val, this->getNumSamples());
}

template <typename T>
void CircularAudioBuffer<T>::setWritePosition(int channel, int val)
{
	mWritePosition[channel] = utils::modulo(val, this->getNumSamples());
}

template <typename T>
void CircularAudioBuffer<T>::write(int channel, const AudioBuffer<T>& inputBuffer)
{
	/* Write data into the circular buffer and update write position */

	int inputBufferLength = inputBuffer.getNumSamples();
	int thisBufferLength = this->getNumSamples();
	int thisBufferRemaining = thisBufferLength - mWritePosition[channel];
	thisBufferRemaining = std::min(thisBufferRemaining, inputBufferLength);
	const float* inputData = inputBuffer.getReadPointer(channel);
	int inputBufferRemaining = std::max(inputBufferLength - thisBufferRemaining, 0);

	
	this->copyFromWithRamp(channel, mWritePosition[channel], inputData, thisBufferRemaining, 1.0, 1.0);
	this->copyFromWithRamp(channel, 0, inputData + thisBufferRemaining, inputBufferRemaining, 1.0, 1.0); // this buffer remaining might need to be clamped at inputBufferLength

	moveWritePosition(channel, inputBufferLength);
}

template <typename T>
void CircularAudioBuffer<T>::read(int channel, AudioBuffer<T>& outputBuffer, float rampGain)
{
	/* Read data from the circular buffer and update read position */
	// TODO: Either this method or the read method does not work and needs to be fixed.


	int outputBufferLength = outputBuffer.getNumSamples();
	int thisBufferLength = this->getNumSamples();
	int thisBufferRemaining = thisBufferLength - mReadPosition[channel];
	thisBufferRemaining = std::min(thisBufferRemaining, outputBufferLength);
	int outputBufferRemaining = std::max(outputBufferLength - thisBufferRemaining, 0);
	const float* thisData = this->getReadPointer(channel);
	//const float* thisDataFromReadPos = this->getReadPointer(channel, mReadPosition[channel]);

	

	outputBuffer.copyFromWithRamp(channel, 0, thisData + mReadPosition[channel], thisBufferRemaining, rampGain, 1.0);
	outputBuffer.copyFromWithRamp(channel, thisBufferRemaining, thisData, outputBufferRemaining, rampGain, 1.0);

	moveReadPosition(channel, outputBufferLength);
}