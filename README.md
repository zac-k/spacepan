# SpacePan

SpacePan is an audio effect plugin that places a sound in a space. It combines:

- **Pan** with an interaural time delay and high-frequency head shadow, so the far ear hears the sound later and duller.
- **Reverb** with a predelay set by the distance to the walls, which differs between the ears as the sound is panned.
- **Delay**, free or locked to the host tempo (note, dotted or triplet values), with swing-pong and widen stereo modes, pre, post or in-loop filtering, saturation, and diffusion.
- **Sidechain ducking** of the reverb and delay, with a shaped ADSR envelope.

It builds as VST3 and Standalone on Windows, macOS and Linux, and as AU on macOS.

## Building

You need CMake 3.22 or later and a C++17 compiler. JUCE 8 is downloaded during configuration.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The plugins are written to `build/SpacePan_artefacts/Release/`. On Linux, first install JUCE's dependencies:

```sh
sudo apt-get install libasound2-dev libfreetype-dev libfontconfig1-dev libx11-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libxext-dev libxcomposite-dev libgl1-mesa-dev
```

To build against a JUCE checkout you already have, add `-DFETCHCONTENT_SOURCE_DIR_JUCE=/path/to/JUCE` when configuring.

## Testing

The build also produces a test runner, which drives the processor directly. It checks echo timing and decay, tempo sync, the interaural delay, reverb predelay, stability at maximum feedback, state restoration, and that random parameter changes at several sample rates and block sizes never produce non-finite output. On Linux it also checks that the audio thread never allocates memory.

```sh
build/Tests/SpacePanTests_artefacts/Release/SpacePanTests
```

On a machine without a display, run it with `xvfb-run -a`. The exit code is the number of failures.

Every push is built and tested on Windows, macOS and Linux by GitHub Actions, which also validates the VST3 with [pluginval](https://github.com/Tracktion/pluginval) at strictness level 10. The built plugins can be downloaded from each run's artifacts.
