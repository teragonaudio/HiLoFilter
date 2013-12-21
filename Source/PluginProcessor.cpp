/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Resources.h"

#if WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif


//==============================================================================
HiLoFilterAudioProcessor::HiLoFilterAudioProcessor() : PluginParameterObserver() {
  parameters.add(new FloatParameter("Filter Position", 0, kHiLoFilterPositionMax, kHiLoFilterPositionMax / 2.0f));
  parameters.add(new FloatParameter("Resonance", kHiLoFilterResonanceMin, kHiLoFilterResonanceMax, kHiLoFilterResonanceDefault));
  parameters.add(new FrequencyParameter("Hi Filter Limit", kHiLoFilterRangeMin, kHiLoFilterRangeMax, kHiLoFilterRangeMax));
  parameters.add(new FrequencyParameter("Lo Filter Limit", kHiLoFilterRangeMin, kHiLoFilterRangeMax, kHiLoFilterRangeMin));
  parameters.add(new FloatParameter("Dead Zone Size", kHiLoFilterDeadZoneMin, kHiLoFilterDeadZoneMax, kHiLoFilterDeadZoneDefault));
  for(int i = 0; i < parameters.size(); i++) {
    parameters.get(i)->addObserver(this);
  }
  filterState = kHiLoFilterStatePassthru;
}

HiLoFilterAudioProcessor::~HiLoFilterAudioProcessor() {
}

//==============================================================================
const String HiLoFilterAudioProcessor::getName() const {
  return JucePlugin_Name;
}

void HiLoFilterAudioProcessor::resetLastIOData() {
  for(int i = 0; i < 2; i++) {
    lastInput1[i] = 0.0f;
    lastInput2[i] = 0.0f;
    lastInput3[i] = 0.0f;
    lastOutput1[i] = 0.0f;
    lastOutput2[i] = 0.0f;
  }
}

int HiLoFilterAudioProcessor::getNumParameters() {
  return kHiLoFilterNumParams;
}

float HiLoFilterAudioProcessor::getParameter(int index) {
  return parameters.get(index)->getScaledValue();
}

float HiLoFilterAudioProcessor::getHiFilterCutoffPosition() {
  return (kHiLoFilterPositionMax + parameters["Dead Zone Size"]->getValue()) / 2.0f;
}

float HiLoFilterAudioProcessor::getLoFilterCutoffPosition() {
  return (kHiLoFilterPositionMax - parameters["Dead Zone Size"]->getValue()) / 2.0f;
}

void HiLoFilterAudioProcessor::setFilterState(float currentFilterPosition) {
  float loCutoff = (kHiLoFilterPositionMax - parameters["Dead Zone Size"]->getValue()) / 2.0f;
  float hiCutoff = (kHiLoFilterPositionMax + parameters["Dead Zone Size"]->getValue()) / 2.0f;
  HiLoFilterState newFilterState;
  if(currentFilterPosition > hiCutoff) {
    newFilterState = kHiLoFilterStateHi;
  }
  else if(currentFilterPosition < loCutoff) {
    newFilterState = kHiLoFilterStateLo;
  }
  else {
    newFilterState = kHiLoFilterStatePassthru;
  }

  if(newFilterState != filterState) {
    filterState = newFilterState;
    resetLastIOData();
    recalculateCoefficients();
  }
}

// TODO: This can be eliminated by making parameters for the relativeFilterPosition in getFilterFrequency()
static float scaleParameterRangeToFrequency(float value, float max, float min) {
  float frequency = expf(value * (logf(max) - logf(min)) + logf(min));
  if(frequency > max) {
    return max;
  }
  else if(frequency < min) {
    return min;
  }
  else {
    return frequency;
  }
}

float HiLoFilterAudioProcessor::getFilterFrequency() {
  switch(filterState) {
    case kHiLoFilterStateHi: {
      float relativeFilterPosition = (parameters["Filter Position"]->getValue() - (kHiLoFilterPositionMax / 2.0f)) / getLoFilterCutoffPosition();
      float newFrequency = scaleParameterRangeToFrequency(relativeFilterPosition, parameters["Hi Filter Limit"]->getValue(), kHiLoFilterRangeMin);
      return newFrequency;
    }
    case kHiLoFilterStateLo: {
      float relativeFilterPosition = parameters["Filter Position"]->getValue() / getLoFilterCutoffPosition();
      float newFrequency = scaleParameterRangeToFrequency(relativeFilterPosition, kHiLoFilterRangeMax, parameters["Lo Filter Limit"]->getValue());
      return newFrequency;
    }
    default:
      return 0.0f;
  }
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

void HiLoFilterAudioProcessor::recalculateCoefficients() {
  switch(filterState) {
    case kHiLoFilterStateHi:
      recalculateHiCoefficients(getSampleRate(), getFilterFrequency(), parameters["Resonance"]->getValue());
    case kHiLoFilterStateLo:
      recalculateLoCoefficients(getSampleRate(), getFilterFrequency(), parameters["Resonance"]->getValue());
    default:
      break;
  }
}

void HiLoFilterAudioProcessor::onParameterUpdated(const PluginParameter* parameter) {
  recalculateCoefficients();
  setFilterState(parameters["Filter Position"]->getValue());
  // TODO: There is probably a better way to check to see if this must be called
  if(parameters["Filter Position"]->getValue() != kHiLoFilterStatePassthru) {
    recalculateCoefficients();
  }
}

void HiLoFilterAudioProcessor::setParameter(int index, float newValue) {
  parameters.setScaled(index, newValue);
}

const String HiLoFilterAudioProcessor::getParameterName(int index) {
  return parameters[index]->getName().c_str();
}

const String HiLoFilterAudioProcessor::getParameterText(int index) {
  return parameters[index]->getDisplayText().c_str();
}

const String HiLoFilterAudioProcessor::getInputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
}

const String HiLoFilterAudioProcessor::getOutputChannelName(int channelIndex) const {
  return String(channelIndex + 1);
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

  resetLastIOData();
  recalculateCoefficients();
}

void HiLoFilterAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HiLoFilterAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  parameters.processRealtimeEvents();
  for(int channel = 0; channel < getNumInputChannels(); ++channel) {
    switch(filterState) {
      case kHiLoFilterStateHi:
        processHiFilter(buffer.getSampleData(channel), channel, buffer.getNumSamples());
        break;
      case kHiLoFilterStateLo:
        processLoFilter(buffer.getSampleData(channel), channel, buffer.getNumSamples());
        break;
      case kHiLoFilterStatePassthru:
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

void HiLoFilterAudioProcessor::processHiFilter(float *channelData, const int channel, const int numFrames) {
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

void HiLoFilterAudioProcessor::processLoFilter(float *channelData, const int channel, const int numFrames) {
  for(int i = 0; i < numFrames; ++i) {
    lastInput3[channel] = lastInput2[channel];
    lastInput2[channel] = lastInput1[channel];
    lastInput1[channel] = channelData[i];

    channelData[i] = (loCoeffA1 * lastInput1[channel]) +
      (loCoeffA2 * lastInput2[channel]) +
      (loCoeffA1 * lastInput3[channel]) -
      (loCoeffB1 * lastOutput1[channel]) -
      (loCoeffB2 * lastOutput2[channel]);

    lastOutput2[channel] = lastOutput1[channel];
    lastOutput1[channel] = channelData[i];
  }
}

//==============================================================================
AudioProcessorEditor *HiLoFilterAudioProcessor::createEditor() {
  return new HiLoFilterAudioProcessorEditor (this, parameters, Resources::getCache());
}

//==============================================================================
void HiLoFilterAudioProcessor::getStateInformation(MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  XmlElement xml("HiLoFilterStorage");
  for(int i = 0; i < parameters.size(); i++) {
    PluginParameter* parameter = parameters[i];
    xml.setAttribute(parameter->getSafeName().c_str(), parameter->getValue());
  }
  copyXmlToBinary(xml, destData);
}

void HiLoFilterAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
  ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
  if(xmlState != 0 && xmlState->hasTagName("HiLoFilterStorage")) {
    for(int i = 0; i < parameters.size(); i++) {
      PluginParameter* parameter = parameters[i];
      parameters.set(parameter, xmlState->getDoubleAttribute(parameter->getSafeName().c_str()));
    }
  }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new HiLoFilterAudioProcessor();
}
