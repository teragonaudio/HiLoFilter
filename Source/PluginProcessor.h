/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_81C4A062__
#define __PLUGINPROCESSOR_H_81C4A062__

#include "../JuceLibraryCode/JuceHeader.h"
#include "TeragonPluginBase.h"
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
static const int kHiLoFilterDeadZoneMax = 11; // Yes, this one goes to 11 \m/

using namespace teragon;

class HiLoFilterAudioProcessor : public TeragonPluginBase, public ParameterObserver {
public:
    HiLoFilterAudioProcessor();
    ~HiLoFilterAudioProcessor() {}

    const String getName() const { return JucePlugin_Name; }
    AudioProcessorEditor *createEditor();

    // Playback
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    // ParameterObserver methods
    void onParameterUpdated(const Parameter* parameter);
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
    // Parameter caches
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
