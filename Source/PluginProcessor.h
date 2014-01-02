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
    double getFilterFrequency();
    double getHiFrequencyFromPosition(const double relativePosition);
    double getLoFrequencyFromPosition(const double relativePosition);
    void recalculateCoefficients();
    void recalculateHiCoefficients(const double sampleRate,
                                   const double frequency,
                                   const double resonance);
    void recalculateLoCoefficients(const double sampleRate,
                                   const double frequency,
                                   const double resonance);

    void processHiFilter(float *channelData, const int channel, const int numSamples);
    void processLoFilter(float *channelData, const int channel, const int numSamples);

    void resetLastIOData();
    double getHiFilterCutoffPosition();
    double getLoFilterCutoffPosition();
    void setFilterState(int currentFilterPosition);

private:
    const double kMaxFilterFrequency = 22000.0;

    const double kHiK0 = 7.0953e-4;
    const double kHiK1 = -317.5514;
    const double kHiK2 = 9257.8428;
    const double kHiK3 = -2.1476e4;
    const double kHiK4 = 3585.1226;
    const double kHiK5 = 2.895e4;

    const double kLoK0 = 20.0;
    const double kLoK1 = 1.1316e4;
    const double kLoK2 = 6880.9585;
    const double kLoK3 = 3554.5339;
    const double kLoK4 = 1059.7163;
    const double kLoK5 = -811.3955;

private:
    // Parameter caches
    IntegerParameter *filterPosition;
    FloatParameter *resonance;
    FrequencyParameter *hiFilterLimit;
    FrequencyParameter *loFilterLimit;
    IntegerParameter *deadZoneSize;

    float lastInput1[2], lastInput2[2], lastInput3[2];
    float lastOutput1[2], lastOutput2[2];

    double loCoeffA1, loCoeffA2;
    double loCoeffB1, loCoeffB2;
    double hiCoeffA1, hiCoeffA2;
    double hiCoeffB1, hiCoeffB2;

    typedef enum {
        kHiLoFilterStatePassthru,
        kHiLoFilterStateLo,
        kHiLoFilterStateHi,
    } FilterState;
    FilterState filterState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLoFilterAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_81C4A062__
