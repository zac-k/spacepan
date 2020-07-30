#include "juceheader.h"
#include <vector>
//#include "utils.h"

template <typename T>
class CircularAudioBuffer : public AudioBuffer<T>
{
	using AudioBuffer::AudioBuffer;
	/*CircularAudioBuffer();
	CircularAudioBuffer(int numChannels, int numSamples);*/

public:
	void setSize(int numChannels, int numSamples);
	void initWritePosition();
	void initReadPosition();
	void initLoopSize();
	void setLoopSize(int channel, int loopSize);
	void moveWritePosition(int channel, int steps);
	void moveWritePosition(int channel, int steps, int loopSize);
	void moveReadPosition(int channel, int steps);
	void moveReadPosition(int channel, int steps, int loopSize);
	int getWritePosition(int channel);
	int getReadPosition(int channel);
	int getLoopSize(int channel);
	void setWritePosition(int channel, int val);
	void setReadPosition(int channel, int val);
	void write(int channel, const AudioBuffer<T>& inputBuffer, bool isAdd=false);
	void read(int channel, AudioBuffer<T>& outputBuffer, bool isOverrideSmoothing=false, bool isAdd=false, float gain = 1.0f);
	void write(const AudioBuffer<T>& inputBuffer, bool isAdd = false);
	void read(AudioBuffer<T>& outputBuffer, bool isOverrideSmoothing = false, bool isAdd = false, float gain = 1.0f);
private:
	std::vector<int> mWritePosition;
	std::vector<int> mReadPosition;
	std::vector<int> mExpectedReadPosition;
	std::vector<int> mLoopSize;

};
