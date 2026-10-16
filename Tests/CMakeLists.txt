# Test runner. It compiles the plugin sources directly rather than linking the
# plugin target, so that it can drive SpacePanAudioProcessor without a host.
juce_add_console_app(SpacePanTests
    PRODUCT_NAME "SpacePanTests")

juce_generate_juce_header(SpacePanTests)

target_sources(SpacePanTests
    PRIVATE
        SpacePanTests.cpp
        ${SPACEPAN_SOURCES})

target_include_directories(SpacePanTests PRIVATE ${CMAKE_SOURCE_DIR}/Source)

target_compile_definitions(SpacePanTests
    PRIVATE
        ${SPACEPAN_DEFINITIONS}
        JUCE_LOG_ASSERTIONS=1
        JucePlugin_Name="SpacePan"
        JucePlugin_IsSynth=0
        JucePlugin_IsMidiEffect=0
        JucePlugin_WantsMidiInput=0
        JucePlugin_ProducesMidiOutput=0)

target_link_libraries(SpacePanTests
    PRIVATE
        SpacePanAssets
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
