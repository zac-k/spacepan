cmake_minimum_required(VERSION 3.22)

project(SpacePan VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE is fetched at a pinned release. To build against a local checkout instead,
# configure with -DFETCHCONTENT_SOURCE_DIR_JUCE=/path/to/JUCE
include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.15
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(JUCE)

# Formats that can be built on the current platform
set(SPACEPAN_FORMATS VST3 Standalone)
if(APPLE)
    list(APPEND SPACEPAN_FORMATS AU)
endif()

juce_add_plugin(SpacePan
    COMPANY_NAME "ZacK"
    COMPANY_WEBSITE "https://zac-k.github.io"
    BUNDLE_ID com.zack.spacepan
    PLUGIN_MANUFACTURER_CODE ZacK
    PLUGIN_CODE Spcp
    FORMATS ${SPACEPAN_FORMATS}
    PRODUCT_NAME "SpacePan"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES Fx Spatial Delay Reverb
    AU_MAIN_TYPE kAudioUnitType_Effect
    COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(SpacePan)

set(SPACEPAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/CustomLookAndFeel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Envelope.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginEditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PluginProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/StandardButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/StandardRotary.cpp)

set(SPACEPAN_DEFINITIONS
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_sources(SpacePan PRIVATE ${SPACEPAN_SOURCES})
target_compile_definitions(SpacePan PUBLIC ${SPACEPAN_DEFINITIONS})

juce_add_binary_data(SpacePanAssets
    SOURCES
        Resource/BPdots.otf
        Resource/all_sprite_0.png
        Resource/all_sprite_63.png
        Resource/animation_knob.png
        Resource/animation_knob2.png
        Resource/cro_glass.png
        Resource/delayOnButtonOff.png
        Resource/delayOnButtonOn.png
        Resource/delayTempoLockOnOff.png
        Resource/sig.png
        Resource/sigDown.png
        Resource/subknob.png
        Resource/three_pos_switch.png)

target_link_libraries(SpacePan
    PRIVATE
        SpacePanAssets
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
