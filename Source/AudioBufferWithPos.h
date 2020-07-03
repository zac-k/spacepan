#include "juceheader.h"
#include <vector>
//#include "utils.h"

template <typename T>
class AudioBufferWithPos : public AudioBuffer<T>
{
	using AudioBuffer::AudioBuffer;

public:
	void initWritePosition();
	void moveWritePosition(int channel, int steps);
	int getWritePosition(int channel);
	void write(int channel, const AudioBuffer<T>& inputBuffer);
private:
	std::vector<int> writePosition;


};