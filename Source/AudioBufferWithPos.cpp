

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
	mReadPosition[channel] = val;
}

template <typename T>
void CircularAudioBuffer<T>::setWritePosition(int channel, int val)
{
	mWritePosition[channel] = val;
}

template <typename T>
void CircularAudioBuffer<T>::write(int channel, const AudioBuffer<T>& inputBuffer)
{
	/* Write data into the circular buffer and update write position*/


	if (this->getNumSamples() > inputBuffer.getNumSamples() + mWritePosition[channel])
	{
		this->copyFromWithRamp(channel, this->getWritePosition(channel), inputBuffer.getReadPointer(channel), inputBuffer.getNumSamples(), 1.0, 1.0);
	}
	else
	{
		const int bufferRemaining = this->getNumSamples() - this->getWritePosition(channel);
		this->copyFromWithRamp(channel, mWritePosition[channel], inputBuffer.getReadPointer(channel), bufferRemaining, 1.0, 1.0);

		this->copyFromWithRamp(channel, 0, inputBuffer.getReadPointer(channel), inputBuffer.getNumSamples() - bufferRemaining, 1.0, 1.0);
	}

	moveWritePosition(channel, inputBuffer.getNumSamples());
}

template <typename T>
void CircularAudioBuffer<T>::read(int channel, AudioBuffer<T>& outputBuffer, float rampGain)
{
	

	if (this->getNumSamples() > outputBuffer.getNumSamples() + mReadPosition[channel])
	{
		outputBuffer.addFromWithRamp(channel, 0, this->getReadPointer(channel, mReadPosition[channel]), outputBuffer.getNumSamples(), rampGain, 1.0);
	}
	else
	{
		const int bufferRemaining = this->getNumSamples() - this->getReadPosition(channel);
		outputBuffer.addFromWithRamp(channel, 0, this->getReadPointer(channel, mReadPosition[channel]), bufferRemaining, rampGain, 1.0);
		outputBuffer.addFromWithRamp(channel, bufferRemaining, this->getReadPointer(channel), this->getNumSamples() - bufferRemaining, rampGain, 1.0);

	}

	moveReadPosition(channel, outputBuffer.getNumSamples());
}