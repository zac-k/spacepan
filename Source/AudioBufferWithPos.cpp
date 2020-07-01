

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
	writePosition[channel] = (writePosition[channel] + steps) % this.getNumSamples();
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
	int bufferRemaining = this.getNumSamples() - this.getWritePosition(channel);
	int partialCopyLength = std::min(bufferRemaining, inputBuffer.getNumSamples());
	this.copyFrom(channel, this.getWritePosition(channel), inputBuffer, 0
		partialCopyLength);
	this.copyFrom(channel, this.getWritePosition(channel), inputBuffer, partialCopyLength,
		inputBuffer.getNumSamples() - partialCopyLength);
	moveWritePosition(channel, inputBuffer.getNumSamples());


}