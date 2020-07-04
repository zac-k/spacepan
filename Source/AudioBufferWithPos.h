#include "juceheader.h"
#include <vector>
//#include "utils.h"

template <typename T>
class AudioBufferWithPos : public AudioBuffer<T>
{
	using AudioBuffer::AudioBuffer;

public:
	void initWritePosition();
	void initReadPosition();
	void moveWritePosition(int channel, int steps);
	void moveReadPosition(int channel, int steps);
	int getWritePosition(int channel);
	int getReadPosition(int channel);
	void setReadPosition(int channel, int val);
	void write(int channel, const AudioBuffer<T>& inputBuffer);
	void read(int channel, AudioBuffer<T>& outputBuffer, float rampGain);
private:
	std::vector<int> mWritePosition;
	std::vector<int> mReadPosition;


};