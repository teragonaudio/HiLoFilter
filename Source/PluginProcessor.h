/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_81C4A062__
#define __PLUGINPROCESSOR_H_81C4A062__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginParameters.h"

typedef enum {
    kHiLoFilterStatePassthru,
    kHiLoFilterStateLo,
    kHiLoFilterStateHi,
} HiLoFilterState;

static const int kHiLoFilterPositionMin = 0;
static const int kHiLoFilterPositionMax = 127;
static const int kHiLoFilterPositionDefault = kHiLoFilterPositionMax / 2;
static const float kHiLoFilterResonanceMin = 0.1f;
static const float kHiLoFilterResonanceDefault = 1.0f;
static const float kHiLoFilterResonanceMax = sqrtf(2.0f);
static const float kHiLoFilterRangeMin = 20.0f;
static const float kHiLoFilterRangeMax = 20000.0f;
static const int kHiLoFilterDeadZoneMin = 1;
static const int kHiLoFilterDeadZoneDefault = 1;
static const int kHiLoFilterDeadZoneMax = 11; // Yes, this one goes to 11...

using namespace teragon;

class HiLoFilterAudioProcessor  : public AudioProcessor, public PluginParameterObserver {
public:
    HiLoFilterAudioProcessor();
    ~HiLoFilterAudioProcessor() {}

    // Playback
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources() {}
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    // Editor
    AudioProcessorEditor *createEditor();
    bool hasEditor() const { return true; }

    // Parameter handling
    int getNumParameters() { return parameters.size(); }
    float getParameter(int index);
    void setParameter(int index, float newValue);
    const String getParameterName(int index);
    const String getParameterText(int index);

    // Plugin configuration and basic properties
    const String getName() const { return JucePlugin_Name; }
    const String getInputChannelName(int channelIndex) const;
    const String getOutputChannelName(int channelIndex) const;
    bool isInputChannelStereoPair(int index) const { return true; }
    bool isOutputChannelStereoPair(int index) const { return true; }
    bool acceptsMidi() const { return false; }
    bool producesMidi() const { return false; }
    double getTailLengthSeconds() const { return 0.0; }
    bool silenceInProducesSilenceOut() const { return true; }

    // Program support (not needed by this plugin)
    int getNumPrograms() { return 0; }
    int getCurrentProgram() { return 0; }
    void setCurrentProgram(int index) {}
    const String getProgramName(int index) { return String::empty; }
    void changeProgramName(int index, const String& newName) {}

    // State restore
    void getStateInformation(MemoryBlock& destData);
    void setStateInformation(const void *data, int sizeInBytes);

    // PluginParameterObserver methods
    void onParameterUpdated(const PluginParameter* parameter);
    bool isRealtimePriority() const { return true; }

private:
    float getFilterFrequency();
    void recalculateCoefficients();
    void recalculateHiCoefficients(const double sampleRate,
                                   const float frequency,
                                   const float resonance);
    void recalculateLoCoefficients(const double sampleRate,
                                   const float frequency,
                                   const float resonance);

    void processHiFilter(float *channelData, const int channel, const int numSamples);
    void processLoFilter(float *channelData, const int channel, const int numSamples);

    void resetLastIOData();
    float getHiFilterCutoffPosition();
    float getLoFilterCutoffPosition();
    void setFilterState(int currentFilterPosition);

private:
    // Parameter storage and caches
    ThreadsafePluginParameterSet parameters;
    IntegerParameter *filterPosition;
    FloatParameter *resonance;
    FrequencyParameter *hiFilterLimit;
    FrequencyParameter *loFilterLimit;
    IntegerParameter *deadZoneSize;

    float lastInput1[2], lastInput2[2], lastInput3[2];
    float lastOutput1[2], lastOutput2[2];

    float loCoeffA1, loCoeffA2;
    float loCoeffB1, loCoeffB2;
    float hiCoeffA1, hiCoeffA2;
    float hiCoeffB1, hiCoeffB2;

    HiLoFilterState filterState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLoFilterAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_81C4A062__
