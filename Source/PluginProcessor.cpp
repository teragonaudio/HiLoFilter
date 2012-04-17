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
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void HiLoFilterAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void HiLoFilterAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  for(int channel = 0; channel < getNumInputChannels(); ++channel) {
    float *channelData = buffer.getSampleData(channel);

    // ..do something to the data...
  }

  // In case we have more outputs than inputs, we'll clear any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }
}

//==============================================================================
bool HiLoFilterAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
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
