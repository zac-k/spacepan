/*
  ==============================================================================

    CircularAudioBuffer.h
    Author:  zac

    An AudioBuffer used as a ring buffer, with independent read and write
    positions per channel. The ring wraps at a per-channel loop size, which
    can be smaller than the allocated buffer.

    All methods except setSize() are real-time safe.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "utils.h"

template <typename T>
class CircularAudioBuffer : public AudioBuffer<T>
{
public:
	using AudioBuffer<T>::AudioBuffer;

	// Allocates the buffer and resets all positions. Not real-time safe.
	void setSize(int newNumChannels, int newNumSamples)
	{
		AudioBuffer<T>::setSize(newNumChannels, newNumSamples);
		this->clear();
		mWritePosition.assign((size_t)newNumChannels, 0);
		mReadPosition.assign((size_t)newNumChannels, 0);
		mExpectedReadPosition.assign((size_t)newNumChannels, noPosition);
		mLoopSize.assign((size_t)newNumChannels, newNumSamples);
	}

	// Sets where the ring wraps. Changing it restarts the positions at zero.
	void setLoopSize(int channel, int loopSize)
	{
		loopSize = jlimit(1, this->getNumSamples(), loopSize);
		if (loopSize != mLoopSize[(size_t)channel])
		{
			mExpectedReadPosition[(size_t)channel] = 1;
			mWritePosition[(size_t)channel] = 0;
			mReadPosition[(size_t)channel] = 0;
			mLoopSize[(size_t)channel] = loopSize;
		}
	}

	int getLoopSize(int channel) const { return mLoopSize[(size_t)channel]; }
	int getWritePosition(int channel) const { return mWritePosition[(size_t)channel]; }
	int getReadPosition(int channel) const { return mReadPosition[(size_t)channel]; }

	void setWritePosition(int channel, int val) { mWritePosition[(size_t)channel] = wrap(channel, val); }
	void setReadPosition(int channel, int val) { mReadPosition[(size_t)channel] = wrap(channel, val); }

	void moveWritePosition(int channel, int steps) { setWritePosition(channel, mWritePosition[(size_t)channel] + steps); }
	void moveReadPosition(int channel, int steps) { setReadPosition(channel, mReadPosition[(size_t)channel] + steps); }

	// Writes (or adds) one channel of inputBuffer at the write position, then advances it
	void write(int channel, const AudioBuffer<T>& inputBuffer, bool isAdd = false)
	{
		const T* input = inputBuffer.getReadPointer(channel);
		T* data = this->getWritePointer(channel);
		const int loopSize = mLoopSize[(size_t)channel];
		int pos = mWritePosition[(size_t)channel];

		for (int i = 0; i < inputBuffer.getNumSamples(); i++)
		{
			data[pos] = isAdd ? data[pos] + input[i] : input[i];
			if (++pos >= loopSize)
				pos = 0;
		}

		mWritePosition[(size_t)channel] = pos;
	}

	// Reads one channel into outputBuffer from the read position, then advances it.
	// If the read position was moved since the last read, the old and new positions
	// are crossfaded over the block to avoid a click, unless isOverrideSmoothing.
	void read(int channel, AudioBuffer<T>& outputBuffer, bool isOverrideSmoothing = false, bool isAdd = false, T gain = 1)
	{
		T* output = outputBuffer.getWritePointer(channel);
		const T* data = this->getReadPointer(channel);
		const int loopSize = mLoopSize[(size_t)channel];
		const int numSamples = outputBuffer.getNumSamples();

		int newPos = mReadPosition[(size_t)channel];
		const int expectedPos = mExpectedReadPosition[(size_t)channel];
		int oldPos = expectedPos == noPosition ? newPos : wrap(channel, expectedPos);
		const bool isCrossfade = oldPos != newPos && !isOverrideSmoothing;

		for (int i = 0; i < numSamples; i++)
		{
			T sample = data[newPos];
			if (isCrossfade)
			{
				const T fadeIn = (T)i / (T)numSamples;
				sample = fadeIn * sample + ((T)1 - fadeIn) * data[oldPos];
				if (++oldPos >= loopSize)
					oldPos = 0;
			}
			sample *= gain;
			output[i] = isAdd ? output[i] + sample : sample;
			if (++newPos >= loopSize)
				newPos = 0;
		}

		mReadPosition[(size_t)channel] = newPos;
		mExpectedReadPosition[(size_t)channel] = newPos;
	}

private:
	// Expected read position before the first read, when there is nothing to crossfade from
	static constexpr int noPosition = -1;

	int wrap(int channel, int val) const { return utils::modulo(val, mLoopSize[(size_t)channel]); }

	std::vector<int> mWritePosition;
	std::vector<int> mReadPosition;
	std::vector<int> mExpectedReadPosition;
	std::vector<int> mLoopSize;
};
