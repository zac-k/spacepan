/*
  ==============================================================================

    SpacePanTests.cpp

    Drives SpacePanAudioProcessor directly and checks its output. Run with no
    arguments; the exit code is the number of failed tests.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Counts heap allocations made on the audio thread while processBlock() runs.
// glibc lets an executable replace malloc and forward to its internal
// allocator, so this is only available on Linux.
#if JUCE_LINUX
extern "C"
{
	void* __libc_malloc(size_t);
	void* __libc_calloc(size_t, size_t);
	void* __libc_realloc(void*, size_t);
	void* __libc_memalign(size_t, size_t);
}

namespace
{
	thread_local bool isCountingAllocations = false;
	std::atomic<int> allocationCount{ 0 };

	void countAllocation()
	{
		if (isCountingAllocations)
			++allocationCount;
	}
}

extern "C"
{
	void* malloc(size_t size) { countAllocation(); return __libc_malloc(size); }
	void* calloc(size_t count, size_t size) { countAllocation(); return __libc_calloc(count, size); }
	void* realloc(void* ptr, size_t size) { countAllocation(); return __libc_realloc(ptr, size); }
	void* memalign(size_t alignment, size_t size) { countAllocation(); return __libc_memalign(alignment, size); }
	void* aligned_alloc(size_t alignment, size_t size) { countAllocation(); return __libc_memalign(alignment, size); }

	int posix_memalign(void** ptr, size_t alignment, size_t size)
	{
		countAllocation();
		*ptr = __libc_memalign(alignment, size);
		return *ptr != nullptr ? 0 : ENOMEM;
	}
}

constexpr bool canCountAllocations = true;
#else
namespace
{
	thread_local bool isCountingAllocations = false;
	std::atomic<int> allocationCount{ 0 };
}

constexpr bool canCountAllocations = false;
#endif

namespace
{
	//==============================================================================
	struct FixedTempoPlayHead : public AudioPlayHead
	{
		explicit FixedTempoPlayHead(double tempo) : bpm(tempo) {}

		Optional<PositionInfo> getPosition() const override
		{
			PositionInfo info;
			info.setBpm(bpm);
			info.setIsPlaying(true);
			return info;
		}

		double bpm;
	};

	// A processor prepared for playback, with helpers to set parameters and render audio
	struct TestHost
	{
		TestHost(double sampleRateToUse, int blockSizeToUse, int numInputs = 2)
			: sampleRate(sampleRateToUse), blockSize(blockSizeToUse)
		{
			AudioProcessor::BusesLayout layout;
			layout.inputBuses.add(numInputs == 1 ? AudioChannelSet::mono() : AudioChannelSet::stereo());
			layout.outputBuses.add(AudioChannelSet::stereo());
			jassertquiet(processor.setBusesLayout(layout));

			processor.setRateAndBufferSizeDetails(sampleRate, blockSize);
			processor.prepareToPlay(sampleRate, blockSize);
		}

		~TestHost()
		{
			processor.releaseResources();
		}

		void set(const String& id, float value)
		{
			auto* parameter = processor.mState.getParameter(id);
			jassert(parameter != nullptr);
			parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
		}

		float get(const String& id)
		{
			return processor.mState.getRawParameterValue(id)->load();
		}

		// Isolates one effect by turning the others off and making the wet path clean
		void onlyDelay()
		{
			set("rev_on_off", 0.0f);
			set("sc_on_off", 0.0f);
			set("pan", 0.0f);
			set("delay_on_off", 1.0f);
			set("delay_mix", 1.0f);
			set("delay_diffusion", 0.0f);
			set("delay_sat", 0.0f);
			set("delay_filter_type", 0.0f);
			set("delay_lowpass", 2.0e4f);
			set("delay_highpass", 100.0f);
			set("delay_stereo_type", 1.0f);
			set("delay_width", 0.0f);
		}

		// Processes buffer in place in blocks of blockSize, or random sizes up to
		// twice blockSize if random is given
		void render(AudioBuffer<float>& buffer, Random* random = nullptr)
		{
			MidiBuffer midi;
			for (int start = 0; start < buffer.getNumSamples();)
			{
				int length = random != nullptr ? 1 + random->nextInt(2 * blockSize) : blockSize;
				length = jmin(length, buffer.getNumSamples() - start);
				AudioBuffer<float> block(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), start, length);

				isCountingAllocations = true;
				processor.processBlock(block, midi);
				isCountingAllocations = false;

				start += length;
			}
		}

		double sampleRate;
		int blockSize;
		SpacePanAudioProcessor processor;
	};

	AudioBuffer<float> makeImpulse(int numSamples, int position = 0, float amplitude = 1.0f)
	{
		AudioBuffer<float> buffer(2, numSamples);
		buffer.clear();
		buffer.setSample(0, position, amplitude);
		buffer.setSample(1, position, amplitude);
		return buffer;
	}

	int findPeak(const AudioBuffer<float>& buffer, int channel, int start, int end)
	{
		const float* data = buffer.getReadPointer(channel);
		int peak = start;
		for (int i = start; i < end; i++)
		{
			if (std::abs(data[i]) > std::abs(data[peak]))
				peak = i;
		}
		return peak;
	}

	// Largest magnitude in the buffer, or infinity if any sample is not finite
	float peakLevel(const AudioBuffer<float>& buffer)
	{
		float peak = 0.0f;
		for (int channel = 0; channel < buffer.getNumChannels(); channel++)
		{
			for (int i = 0; i < buffer.getNumSamples(); i++)
			{
				const float sample = buffer.getSample(channel, i);
				if (!std::isfinite(sample))
					return std::numeric_limits<float>::infinity();
				peak = jmax(peak, std::abs(sample));
			}
		}
		return peak;
	}
}

//==============================================================================
class SpacePanTests : public UnitTest
{
public:
	SpacePanTests() : UnitTest("SpacePan") {}

	void runTest() override
	{
		if (canCountAllocations)
		{
			beginTest("Allocation counter detects allocations");
			allocationCount = 0;
			isCountingAllocations = true;
			auto* allocated = new std::vector<int>(100);
			isCountingAllocations = false;
			delete allocated;
			expectGreaterThan(allocationCount.load(), 0);
		}

		beginTest("Random parameters, sample rates and block sizes");
		{
			Random random(1234);
			for (double sampleRate : { 44100.0, 48000.0, 96000.0 })
			{
				for (int blockSize : { 64, 512 })
				{
					TestHost host(sampleRate, blockSize);
					auto& parameters = host.processor.getParameters();
					allocationCount = 0;
					float peak = 0.0f;

					for (int round = 0; round < 50; round++)
					{
						for (auto* parameter : parameters)
							parameter->setValueNotifyingHost(random.nextFloat());

						AudioBuffer<float> buffer(2, 4 * blockSize);
						for (int channel = 0; channel < 2; channel++)
							for (int i = 0; i < buffer.getNumSamples(); i++)
								buffer.setSample(channel, i, random.nextFloat() - 0.5f);

						host.render(buffer, &random);
						const float blockPeak = peakLevel(buffer);
						if (blockPeak > 8.0f && peak <= 8.0f)
						{
							String settings;
							for (auto* parameter : parameters)
								settings << parameter->getName(32) << "=" << parameter->getCurrentValueAsText() << " ";
							logMessage("Loud block with: " + settings);
						}
						peak = jmax(peak, blockPeak);
					}

					const String config = String(sampleRate) + " Hz, block " + String(blockSize);
					expectLessThan(peak, 8.0f, "Peak level at " + config);
					if (canCountAllocations)
						expectEquals(allocationCount.load(), 0, "Allocations on the audio thread at " + config);
				}
			}
			if (!canCountAllocations)
				logMessage("(allocation counting is only available on Linux)");
		}

		beginTest("Delay echoes at the delay time and decays by the feedback");
		{
			// Filters spread an impulse over a few samples, so compare energy around
			// each echo with the energy of the dry impulse through the same filters
			auto renderDelay = [](float mix)
			{
				TestHost host(48000.0, 256);
				host.onlyDelay();
				host.set("delay_tempo_lock", 0.0f);
				host.set("delay_time", 0.25f);
				host.set("delay_feedback", 0.5f);
				host.set("delay_mix", mix);

				auto buffer = makeImpulse(48000);
				host.render(buffer);
				return buffer;
			};
			auto energyAround = [](const AudioBuffer<float>& buffer, int centre)
			{
				float energy = 0.0f;
				for (int i = jmax(0, centre - 50); i < centre + 50; i++)
					energy += buffer.getSample(0, i) * buffer.getSample(0, i);
				return energy;
			};

			const auto dry = renderDelay(0.0f);
			const auto wet = renderDelay(1.0f);

			const int delaySamples = 12000;
			const int firstEcho = findPeak(wet, 0, delaySamples - 100, delaySamples + 100);
			const int secondEcho = findPeak(wet, 0, 2 * delaySamples - 100, 2 * delaySamples + 100);
			const float dryEnergy = energyAround(dry, findPeak(dry, 0, 0, 100));
			const float firstLevel = std::sqrt(energyAround(wet, firstEcho) / dryEnergy);
			const float secondLevel = std::sqrt(energyAround(wet, secondEcho) / dryEnergy);

			expect(std::abs(firstEcho - delaySamples) <= 2, "First echo at " + String(firstEcho));
			expect(std::abs(secondEcho - 2 * delaySamples) <= 4, "Second echo at " + String(secondEcho));
			expectWithinAbsoluteError(firstLevel, 1.0f, 0.1f, "First echo level");
			expectWithinAbsoluteError(secondLevel / firstLevel, 0.5f, 0.05f, "Echo decay ratio");
		}

		beginTest("Tempo-locked delay follows the host tempo");
		{
			for (double bpm : { 120.0, 90.0, 0.0 })
			{
				TestHost host(48000.0, 256);
				FixedTempoPlayHead playHead(bpm);
				host.processor.setPlayHead(&playHead);
				host.onlyDelay();
				host.set("delay_tempo_lock", 1.0f);
				host.set("delay_time_discrete", 3.0f); // 1/4 bar
				host.set("delay_modifier", 2.0f); // whole note values

				auto buffer = makeImpulse(72000);
				host.render(buffer);

				// A tempo of zero is invalid, so the default of 120 bpm is used
				const double effectiveBpm = bpm > 0.0 ? bpm : 120.0;
				const int expected = (int)(0.25 * 4.0 * 60.0 / effectiveBpm * 48000.0);
				const int echo = findPeak(buffer, 0, expected / 2, buffer.getNumSamples());
				expect(std::abs(echo - expected) <= 2, "Echo at " + String(echo) + ", expected " + String(expected) + " at " + String(bpm) + " bpm");
				host.processor.setPlayHead(nullptr);
			}
		}

		beginTest("Panning delays the far ear");
		{
			for (float pan : { 1.0f, -1.0f })
			{
				TestHost host(48000.0, 256);
				host.set("delay_on_off", 0.0f);
				host.set("rev_on_off", 0.0f);
				host.set("room_size", 0.0f);
				host.set("head_width", 0.15f);
				host.set("pan", pan);

				auto buffer = makeImpulse(1024, 100);
				host.render(buffer);

				const int farChannel = pan > 0 ? 0 : 1;
				const int nearChannel = 1 - farChannel;
				const int expectedLag = (int)(0.15f / 343.0f * 48000.0f * 0.8f);
				const int lag = findPeak(buffer, farChannel, 0, 1024) - findPeak(buffer, nearChannel, 0, 1024);
				expect(std::abs(lag - expectedLag) <= 3, "Far ear lag " + String(lag) + ", expected about " + String(expectedLag) + " for pan " + String(pan));
			}
		}

		beginTest("Reverb predelay matches the room size");
		{
			TestHost host(48000.0, 256);
			host.set("delay_on_off", 0.0f);
			host.set("sc_on_off", 0.0f);
			host.set("rev_on_off", 1.0f);
			host.set("rev_mix", 1.0f);
			host.set("rev_lowpass", 2.0e4f);
			host.set("rev_highpass", 100.0f);
			host.set("pan", 0.0f);
			host.set("room_size", 1.0f);

			auto buffer = makeImpulse(24000);
			host.render(buffer);

			int onset = -1;
			for (int i = 0; i < buffer.getNumSamples() && onset < 0; i++)
			{
				if (std::abs(buffer.getSample(0, i)) > 1.0e-4f)
					onset = i;
			}

			// Out to a wall and back, plus the reverb's own shortest comb delay
			const int predelay = (int)(20.0f * 2.0f / 343.0f * 48000.0f);
			expect(onset >= predelay && onset < predelay + 1500, "Reverb onset at " + String(onset) + ", predelay " + String(predelay));
		}

		beginTest("High feedback and saturation stay bounded");
		{
			for (float saturation : { 0.0f, 10.0f })
			{
				// The reverb is off so that the limit applies to the delay loop alone
				TestHost host(48000.0, 512);
				host.set("rev_on_off", 0.0f);
				host.set("delay_on_off", 1.0f);
				host.set("delay_tempo_lock", 0.0f);
				host.set("delay_time", 0.01f);
				host.set("delay_feedback", 1.0f);
				host.set("delay_sat", saturation);
				host.set("delay_sat_char", 0.5f);
				host.set("delay_diffusion", 1.0f);
				host.set("delay_filter_type", 1.0f);
				host.set("delay_lowpass_Q", 5.0f);
				host.set("delay_highpass_Q", 5.0f);

				AudioBuffer<float> buffer(2, 48000 * 20);
				for (int i = 0; i < buffer.getNumSamples(); i++)
				{
					const float sample = std::sin(MathConstants<float>::twoPi * 440.0f * (float)i / 48000.0f);
					buffer.setSample(0, i, sample);
					buffer.setSample(1, i, sample);
				}
				host.render(buffer);
				expectLessThan(peakLevel(buffer), 4.0f, "Peak level with saturation " + String(saturation));
			}
		}

		beginTest("Turning the delay off clears old echoes");
		{
			TestHost host(48000.0, 256);
			host.onlyDelay();
			host.set("delay_tempo_lock", 0.0f);
			host.set("delay_time", 0.5f);
			host.set("delay_feedback", 0.9f);

			auto buffer = makeImpulse(4800);
			host.render(buffer);
			host.set("delay_on_off", 0.0f);
			AudioBuffer<float> silence(2, 4800);
			silence.clear();
			host.render(silence);
			host.set("delay_on_off", 1.0f);

			AudioBuffer<float> after(2, 96000);
			after.clear();
			host.render(after);
			expectLessThan(after.getMagnitude(0, after.getNumSamples()), 1.0e-6f, "Echo after delay was turned off and on");
		}

		beginTest("Mono input is copied to both channels");
		{
			TestHost host(48000.0, 256, 1);
			host.set("delay_on_off", 0.0f);
			host.set("rev_on_off", 0.0f);
			host.set("pan", 0.0f);

			auto buffer = makeImpulse(1024, 100);
			buffer.clear(1, 0, buffer.getNumSamples());
			for (int i = 0; i < buffer.getNumSamples(); i++)
				buffer.setSample(1, i, 9.0f); // garbage on the unused channel
			buffer.setSample(0, 100, 1.0f);
			host.render(buffer);

			float difference = 0.0f;
			for (int i = 0; i < buffer.getNumSamples(); i++)
				difference = jmax(difference, std::abs(buffer.getSample(0, i) - buffer.getSample(1, i)));
			expectLessThan(difference, 1.0e-6f, "Left and right differ");
			expectGreaterThan(buffer.getMagnitude(0, 0, buffer.getNumSamples()), 0.1f, "Output is silent");
		}

		beginTest("State round trip");
		{
			TestHost first(48000.0, 256);
			first.set("pan", -0.3f);
			first.set("delay_time_discrete", 6.0f);
			first.set("rev_on_off", 0.0f);
			first.set("delay_lowpass", 5000.0f);

			MemoryBlock state;
			first.processor.getStateInformation(state);

			TestHost second(48000.0, 256);
			second.processor.setStateInformation(state.getData(), (int)state.getSize());
			for (auto* id : { "pan", "delay_time_discrete", "rev_on_off", "delay_lowpass" })
				expectWithinAbsoluteError(second.get(id), first.get(id), 1.0e-3f, String("Restored ") + id);
		}

		beginTest("Tail length");
		{
			TestHost host(48000.0, 256);
			host.set("delay_on_off", 1.0f);
			host.set("delay_feedback", 0.5f);
			const double tail = host.processor.getTailLengthSeconds();
			expect(tail > 0.0 && std::isfinite(tail), "Tail " + String(tail));
			host.set("delay_feedback", 1.0f);
			expect(std::isinf(host.processor.getTailLengthSeconds()), "Infinite feedback should report an infinite tail");
		}

		beginTest("Editor opens and closes");
		{
			TestHost host(48000.0, 256);
			std::unique_ptr<AudioProcessorEditor> editor(host.processor.createEditorAndMakeActive());
			expect(editor != nullptr);
			expectEquals(editor->getWidth(), 852);
			expectEquals(editor->getHeight(), 600);
			host.processor.editorBeingDeleted(editor.get());
			editor.reset();
		}
	}
};

//==============================================================================
// Fails the run if JUCE reports an assertion
class AssertionLogger : public Logger
{
public:
	void logMessage(const String& message) override
	{
		std::cout << message << std::endl;
		if (message.contains("JUCE Assertion failure"))
			++numAssertions;
	}

	int numAssertions = 0;
};

int main()
{
	ScopedJuceInitialiser_GUI juce;
	AssertionLogger logger;
	Logger::setCurrentLogger(&logger);

	SpacePanTests tests;
	UnitTestRunner runner;
	runner.setAssertOnFailure(false);
	runner.runTests({ &tests });

	int numFailures = 0;
	for (int i = 0; i < runner.getNumResults(); i++)
		numFailures += runner.getResult(i)->failures;

	Logger::setCurrentLogger(nullptr);

	if (logger.numAssertions > 0)
		std::cout << logger.numAssertions << " JUCE assertion(s) were reported" << std::endl;

	return numFailures + logger.numAssertions;
}
