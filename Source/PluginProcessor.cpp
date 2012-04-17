/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HiLoFilterAudioProcessor::HiLoFilterAudioProcessor() {
  filterState = kHiLoFilterStateInvalid;
}

HiLoFilterAudioProcessor::~HiLoFilterAudioProcessor() {
}

//==============================================================================
const String HiLoFilterAudioProcessor::getName() const {
  return JucePlugin_Name;
}

int HiLoFilterAudioProcessor::getNumParameters() {
  return kHiLoFilterNumParams;
}

float HiLoFilterAudioProcessor::getParameter(int index) {
  switch(index) {
    case kHiLoFilterParamFilterFrequency: return filterFrequency;
    case kHiLoFilterParamFilterResonance: return filterResonance;
    case kHiLoFilterParamHiFilterRange: return hiFilterRange;
    case kHiLoFilterParamLoFilterRange: return loFilterRange;
    case kHiLoFilterParamDeadZoneSize: return deadZoneSize;
    default: return 0.0f;
  }
}

void HiLoFilterAudioProcessor::setParameter(int index, float newValue) {
  switch(index) {
    case kHiLoFilterParamFilterFrequency:
      break;
    case kHiLoFilterParamFilterResonance:
      break;
    case kHiLoFilterParamHiFilterRange:
      break;
    case kHiLoFilterParamLoFilterRange:
      break;
    case kHiLoFilterParamDeadZoneSize:
      break;
    default: break;
  }
}

const String HiLoFilterAudioProcessor::getParameterName(int index) {
  switch(index) {
    case kHiLoFilterParamFilterFrequency: return String("Frequency");
    case kHiLoFilterParamFilterResonance: return String("Resonance");
    case kHiLoFilterParamHiFilterRange: return String("Hi Filter Range");
    case kHiLoFilterParamLoFilterRange: return String("Lo Filter Range");
    case kHiLoFilterParamDeadZoneSize: return String("Dead Zone Size");
    default: return String::empty;
  }
}

const String HiLoFilterAudioProcessor::getParameterText(int index) {
  switch(index) {
    case kHiLoFilterParamFilterFrequency: return String("Frequency");
    case kHiLoFilterParamFilterResonance: return String("Resonance");
    case kHiLoFilterParamHiFilterRange: return String("Hi Filter Range");
    case kHiLoFilterParamLoFilterRange: return String("Lo Filter Range");
    case kHiLoFilterParamDeadZoneSize: return String(deadZoneSize);
    default: return String::empty;
  }
}

const String HiLoFilterAudioProcessor::getInputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
}

const String HiLoFilterAudioProcessor::getOutputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
}

bool HiLoFilterAudioProcessor::isInputChannelStereoPair(int index) const {
  return true;
}

bool HiLoFilterAudioProcessor::isOutputChannelStereoPair(int index) const {
  return true;
}

bool HiLoFilterAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
  return false;
#endif
}

bool HiLoFilterAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
  return false;
#endif
}

int HiLoFilterAudioProcessor::getNumPrograms() {
  return 0;
}

int HiLoFilterAudioProcessor::getCurrentProgram() {
  return 0;
}

void HiLoFilterAudioProcessor::setCurrentProgram(int index) {
}

const String HiLoFilterAudioProcessor::getProgramName(int index) {
  return String::empty;
}

void HiLoFilterAudioProcessor::changeProgramName(int index, const String& newName) {
}

//==============================================================================
void HiLoFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  for(int i = 0; i < 2; i++) {
    lastInput1[i] = 0.0f;
    lastInput2[i] = 0.0f;
    lastInput3[i] = 0.0f;
    lastOutput1[i] = 0.0f;
    lastOutput2[i] = 0.0f;
  }

  recalculateHiCoefficients(sampleRate, filterFrequency, filterResonance);
  recalculateLoCoefficients(sampleRate, filterFrequency, filterResonance);
}

void HiLoFilterAudioProcessor::recalculateHiCoefficients(const double sampleRate, const float frequency, const float resonance) {
  const float coeffConstant = (float)tan(M_PI * frequency / sampleRate);
  hiCoeffA1 = 1.0f / ((1.0f + resonance * coeffConstant) + (coeffConstant * coeffConstant));
  hiCoeffA2 = -2.0f * hiCoeffA1;
  hiCoeffB1 = 2.0f * hiCoeffA1 * ((coeffConstant * coeffConstant) - 1.0f);
  hiCoeffB2 = hiCoeffA1 * (1.0f - (resonance * coeffConstant) + (coeffConstant * coeffConstant));
}

void HiLoFilterAudioProcessor::recalculateLoCoefficients(const double sampleRate, const float frequency, const float resonance) {
  const float coeffConstant = (float)(1.0f / tan(frequency / sampleRate));
  loCoeffA1 = 1.0f / (1.0f + (resonance * coeffConstant) + (coeffConstant * coeffConstant));
  loCoeffA2 = 2.0f * loCoeffA1;
  loCoeffB1 = 2.0f * loCoeffA1 * (1.0f - (coeffConstant * coeffConstant));
  loCoeffB2 = loCoeffA1 * (1.0f - (resonance * coeffConstant) + (coeffConstant * coeffConstant));
}

void HiLoFilterAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HiLoFilterAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  for(int channel = 0; channel < getNumInputChannels(); ++channel) {
    switch(filterState) {
      case kHiLoFilterStateHi:
      case kHiLoFilterStateLo:
        processFilter(buffer.getSampleData(channel), channel, buffer.getNumSamples());
        break;
      case kHiLoFilterStatePassthru:
      case kHiLoFilterStateInvalid:
      default:
        break;
    }
  }

  // In case we have more outputs than inputs, we'll clear any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }
}

void HiLoFilterAudioProcessor::processFilter(float *channelData, const int channel, const int numFrames) {
  for(int i = 0; i < numFrames; ++i) {
    lastInput3[channel] = lastInput2[channel];
    lastInput2[channel] = lastInput1[channel];
    lastInput1[channel] = channelData[i];

    channelData[i] = (hiCoeffA1 * lastInput1[channel]) +
      (hiCoeffA2 * lastInput2[channel]) +
      (hiCoeffA1 * lastInput3[channel]) -
      (hiCoeffB1 * lastOutput1[channel]) -
      (hiCoeffB2 * lastOutput2[channel]);

    lastOutput2[channel] = lastOutput1[channel];
    lastOutput1[channel] = channelData[i];
  }
}

//==============================================================================
bool HiLoFilterAudioProcessor::hasEditor() const {
  return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *HiLoFilterAudioProcessor::createEditor() {
  return new HiLoFilterAudioProcessorEditor (this);
}

//==============================================================================
void HiLoFilterAudioProcessor::getStateInformation(MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void HiLoFilterAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new HiLoFilterAudioProcessor();
}
