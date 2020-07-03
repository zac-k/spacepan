

#include "AudioBufferWithPos.h"

template <typename T>
void AudioBufferWithPos<T>::initWritePosition()
{
	writePosition.clear();
	for (int i = 0; i < this->getNumChannels(); i++)
	{
		writePosition.push_back(0);
	}
	return;
}

template <typename T>
void AudioBufferWithPos<T>::moveWritePosition(int channel, int steps)
{
	writePosition[channel] = utils::modulo((writePosition[channel] + steps), this->getNumSamples());
	return;
}

template <typename T>
int AudioBufferWithPos<T>::getWritePosition(int channel)
{
	return writePosition[channel];
}

template <typename T>
void AudioBufferWithPos<T>::write(int channel, const AudioBuffer<T>& inputBuffer)
{
	/* Write data into the circular buffer and update write position*/

	/*int bufferRemaining = this->getNumSamples() - this->getWritePosition(channel);
	int partialCopyLength = std::min(bufferRemaining, inputBuffer.getNumSamples());
	this->copyFrom(channel, this->getWritePosition(channel), inputBuffer.getReadPointer(channel), 0,
		partialCopyLength);
	if (partialCopyLength < inputBuffer.getNumSamples)
	{
		this->copyFrom(channel, 0, inputBuffer.getReadPointer(channel), partialCopyLength,
			inputBuffer.getNumSamples() - partialCopyLength);
	}*/
	//moveWritePosition(channel, inputBuffer.getNumSamples());

	if (this->getNumSamples() > inputBuffer.getNumSamples() + this->getWritePosition(channel))
	{
		this->copyFromWithRamp(channel, this->getWritePosition(channel), inputBuffer.getReadPointer(channel), inputBuffer.getNumSamples(), 1.0, 1.0);
	}
	else
	{
		const int bufferRemaining = this->getNumSamples() - this->getWritePosition(channel);
		this->copyFromWithRamp(channel, this->getWritePosition(channel), inputBuffer.getReadPointer(channel), bufferRemaining, 1.0, 1.0);

		this->copyFromWithRamp(channel, 0, inputBuffer.getReadPointer(channel), inputBuffer.getNumSamples() - bufferRemaining, 1.0, 1.0);
	}


}
