/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_81C4A062__
#define __PLUGINPROCESSOR_H_81C4A062__

#include "../JuceLibraryCode/JuceHeader.h"

typedef enum {
  kHiLoFilterParamFilterPosition,
  kHiLoFilterParamFilterResonance,
  kHiLoFilterParamHiFilterRange,
  kHiLoFilterParamLoFilterRange,
  kHiLoFilterParamDeadZoneSize,
  kHiLoFilterNumParams
} HiLoFilterParams;

typedef enum {
  kHiLoFilterStatePassthru,
  kHiLoFilterStateLo,
  kHiLoFilterStateHi,
  kHiLoFilterStateInvalid,
  kHiLoFilterStateNumStates,
} HiLoFilterState;

static const float kHiLoFilterPositionMax = 127.0f;
static const float kHiLoFilterResonanceMin = 0.1f;
static const float kHiLoFilterResonanceDefault = 1.0f;
static const float kHiLoFilterResonanceMax = sqrtf(2.0f);
static const float kHiLoFilterRangeMin = 20.0f;
static const float kHiLoFilterRangeMax = 20000.0f;
static const float kHiLoFilterDeadZoneMin = 1.0f;
static const float kHiLoFilterDeadZoneDefault = 1.0f;
static const float kHiLoFilterDeadZoneMax = 11.0f;

#define PARAM_TEXT_NUM_DECIMAL_PLACES 2

//==============================================================================
/**
*/
class HiLoFilterAudioProcessor  : public AudioProcessor {
public:
  //==============================================================================
  HiLoFilterAudioProcessor();

  ~HiLoFilterAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock);

  void releaseResources();

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

  //==============================================================================
  AudioProcessorEditor *createEditor();

  bool hasEditor() const;

  //==============================================================================
  const String getName() const;

  int getNumParameters();

  float getParameter(int index);

  void setParameter(int index, float newValue);

  const String getParameterName(int index);

  const String getParameterText(int index);

  const String getInputChannelName(int channelIndex) const;

  const String getOutputChannelName(int channelIndex) const;

  bool isInputChannelStereoPair(int index) const;

  bool isOutputChannelStereoPair(int index) const;

  bool acceptsMidi() const;

  bool producesMidi() const;

  //==============================================================================
  int getNumPrograms();

  int getCurrentProgram();

  void setCurrentProgram(int index);

  const String getProgramName(int index);

  void changeProgramName(int index, const String& newName);

  //==============================================================================
  void getStateInformation(MemoryBlock& destData);

  void setStateInformation(const void *data, int sizeInBytes);

private:
  float getFilterFrequency();
  void recalculateCoefficients();
  void recalculateHiCoefficients(const double sampleRate, const float frequency, const float resonance);
  void recalculateLoCoefficients(const double sampleRate, const float frequency, const float resonance);

  void processHiFilter(float *channelData, const int channel, const int numSamples);
  void processLoFilter(float *channelData, const int channel, const int numSamples);

  void resetLastIOData();


  float filterPosition;
  float filterResonance;
  float hiFilterRange;
  float loFilterRange;
  float deadZoneSize;

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
