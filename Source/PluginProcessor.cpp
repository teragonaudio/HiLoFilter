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

HiLoFilterAudioProcessor::HiLoFilterAudioProcessor() : ParameterObserver() {
    filterPosition = new IntegerParameter("Filter Position", 0, 100, 50);
    parameters.add(filterPosition);
    resonance = new FloatParameter("Resonance", 0.1, sqrt(2.0), 1.0);
    parameters.add(resonance);

    hiFilterLimit = new FrequencyParameter("Hi Filter Limit", 20.0, kMaxFilterFrequency, kMaxFilterFrequency);
    parameters.add(hiFilterLimit);

    loFilterLimit = new FrequencyParameter("Lo Filter Limit", 20.0, kMaxFilterFrequency, 20.0);
    parameters.add(loFilterLimit);

    deadZoneSize = new IntegerParameter("Dead Zone Size", 1, 11, 1); // This one goes to 11 \m/
    parameters.add(deadZoneSize);

    // Changing any of the above parameters will require recalculating coefficients, so observe
    // all parameters in the map (thus far).
    for(int i = 0; i < parameters.size(); i++) {
        parameters.get(i)->addObserver(this);
    }

    // Other parameters
    ParameterString version = ProjectInfo::projectName;
    version.append(" version ").append(ProjectInfo::versionString);
    parameters.add(new StringParameter("Version", version));

    recalculateCoefficients();
}

double HiLoFilterAudioProcessor::getHiFilterCutoffPosition() {
    return (filterPosition->getMaxValue() + deadZoneSize->getValue()) / 2.0;
}

double HiLoFilterAudioProcessor::getLoFilterCutoffPosition() {
    return (filterPosition->getMaxValue() - deadZoneSize->getValue()) / 2.0;
}

void HiLoFilterAudioProcessor::setFilterState(int currentFilterPosition) {
/*
    FilterState newFilterState = kHiLoFilterStatePassthru;
    if(currentFilterPosition > getHiFilterCutoffPosition()) {
        newFilterState = kHiLoFilterStateHi;
    }
    else if(currentFilterPosition < getLoFilterCutoffPosition()) {
        newFilterState = kHiLoFilterStateLo;
    }

    if(newFilterState != filterState) {
        filterState = newFilterState;
        resetLastIOData();
        recalculateCoefficients();
    }
*/
}

void HiLoFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    TeragonPluginBase::prepareToPlay(sampleRate, samplesPerBlock);
    resetLastIOData();
    recalculateCoefficients();
}

void HiLoFilterAudioProcessor::resetLastIOData() {
    for(int i = 0; i < 2; i++) {
        lastHiInput1[i] = 0.0f;
        lastHiInput2[i] = 0.0f;
        lastHiInput3[i] = 0.0f;
        lastHiOutput1[i] = 0.0f;
        lastHiOutput2[i] = 0.0f;
        lastLoInput1[i] = 0.0f;
        lastLoInput2[i] = 0.0f;
        lastLoInput3[i] = 0.0f;
        lastLoOutput1[i] = 0.0f;
        lastLoOutput2[i] = 0.0f;
    }
}

void HiLoFilterAudioProcessor::recalculateCoefficients() {
    recalculateHiCoefficients(getSampleRate(), getHiFilterFrequency(), resonance->getValue());
    recalculateLoCoefficients(getSampleRate(), getLoFilterFrequency(), resonance->getValue());
/*
    setFilterState((int)filterPosition->getValue());
    switch(filterState) {
        case kHiLoFilterStateHi:
            recalculateHiCoefficients(getSampleRate(), getFilterFrequency(), resonance->getValue());
            break;
        case kHiLoFilterStateLo:
            recalculateLoCoefficients(getSampleRate(), getFilterFrequency(), resonance->getValue());
            break;
        case kHiLoFilterStatePassthru:
        default:
            break;
    }
*/
}

double HiLoFilterAudioProcessor::getFilterFrequency() {
    double newFrequency = 0.0;
    const double position = filterPosition->getValue();
    const double loCutoff = getLoFilterCutoffPosition();
    // Will represent how much of the filter should be applied, after considering which half
    // of the filter we are in and subtracting the dead zone size. The result will be a value
    // between 0.0-1.0 which would correspond to a separate knob for the given filter.
    double relativePosition;

/*
    switch(filterState) {
        case kHiLoFilterStateHi: {
            relativePosition = (position - loCutoff - deadZoneSize->getValue()) / loCutoff;
            newFrequency = getHiFrequencyFromPosition(relativePosition);
            break;
        }
        case kHiLoFilterStateLo: {
            relativePosition = position / loCutoff;
            newFrequency = getLoFrequencyFromPosition(relativePosition);
            break;
        }
        case kHiLoFilterStatePassthru:
        default:
            break;
    }
*/

    printf("Filter position: %g, frequency: %g\n", position, newFrequency);
    if(newFrequency < 0.0) {
        newFrequency = 0.0;
    }
    else if(newFrequency > kMaxFilterFrequency) {
        newFrequency = kMaxFilterFrequency;
    }
    return newFrequency;
}

double HiLoFilterAudioProcessor::getHiFilterFrequency() {
    double newFrequency = 0.0;
    const double position = filterPosition->getValue();
    const double loCutoff = getLoFilterCutoffPosition();
    // Will represent how much of the filter should be applied, after considering which half
    // of the filter we are in and subtracting the dead zone size. The result will be a value
    // between 0.0-1.0 which would correspond to a separate knob for the given filter.
    double relativePosition;

            relativePosition = (position - loCutoff - deadZoneSize->getValue()) / loCutoff;
            newFrequency = getHiFrequencyFromPosition(relativePosition);

    if(newFrequency < 0.0) {
        newFrequency = 0.0;
    }
    else if(newFrequency > kMaxFilterFrequency) {
        newFrequency = kMaxFilterFrequency;
    }
    printf("Hi Filter position: %g, frequency: %g\n", position, newFrequency);
    return newFrequency;
}

double HiLoFilterAudioProcessor::getLoFilterFrequency() {
    double newFrequency = 0.0;
    const double position = filterPosition->getValue();
    const double loCutoff = getLoFilterCutoffPosition();
    // Will represent how much of the filter should be applied, after considering which half
    // of the filter we are in and subtracting the dead zone size. The result will be a value
    // between 0.0-1.0 which would correspond to a separate knob for the given filter.
    double relativePosition;

            relativePosition = position / loCutoff;
            newFrequency = getLoFrequencyFromPosition(relativePosition);

    if(newFrequency < 0.0) {
        newFrequency = 0.0;
    }
    else if(newFrequency > kMaxFilterFrequency) {
        newFrequency = kMaxFilterFrequency;
    }
    printf("Lo Filter position: %g, frequency: %g\n", position, newFrequency);
    return newFrequency;
}

double HiLoFilterAudioProcessor::getHiFrequencyFromPosition(const double relativePosition) {
    return kHiK0 +
           (relativePosition * kHiK1) +
           (pow(relativePosition, 2) * kHiK2) +
           (pow(relativePosition, 3) * kHiK3) +
           (pow(relativePosition, 4) * kHiK4) +
           (pow(relativePosition, 5) * kHiK5);
}

double HiLoFilterAudioProcessor::getLoFrequencyFromPosition(const double relativePosition) {
    return kLoK0 +
        (relativePosition * kLoK1) +
        (pow(relativePosition, 2) * kLoK2) +
        (pow(relativePosition, 3) * kLoK3) +
        (pow(relativePosition, 4) * kLoK4) +
        (pow(relativePosition, 5) * kLoK5);
}

void HiLoFilterAudioProcessor::recalculateHiCoefficients(const double sampleRate,
                                                         const double frequency,
                                                         const double resonance) {
    const double coeffConstant = tan(M_PI * frequency / sampleRate);
    hiCoeffA1 = 1.0 / ((1.0 + resonance * coeffConstant) + (coeffConstant * coeffConstant));
    hiCoeffA2 = -2.0 * hiCoeffA1;
    hiCoeffB1 = 2.0 * hiCoeffA1 * ((coeffConstant * coeffConstant) - 1.0);
    hiCoeffB2 = hiCoeffA1 * (1.0 - (resonance * coeffConstant) + (coeffConstant * coeffConstant));
}

void HiLoFilterAudioProcessor::recalculateLoCoefficients(const double sampleRate,
                                                         const double frequency,
                                                         const double resonance) {
    const double coeffConstant = 1.0 / tan(frequency / sampleRate);
    loCoeffA1 = 1.0 / (1.0 + (resonance * coeffConstant) + (coeffConstant * coeffConstant));
    loCoeffA2 = 2.0 * loCoeffA1;
    loCoeffB1 = 2.0 * loCoeffA1 * (1.0 - (coeffConstant * coeffConstant));
    loCoeffB2 = loCoeffA1 * (1.0 - (resonance * coeffConstant) + (coeffConstant * coeffConstant));
}

void HiLoFilterAudioProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    TeragonPluginBase::processBlock(buffer, midiMessages);

    for(int channel = 0; channel < getNumInputChannels(); ++channel) {
        float *channelData = buffer.getSampleData(channel);
        processHiFilter(channelData, channel, buffer.getNumSamples());
        processLoFilter(channelData, channel, buffer.getNumSamples());
/*
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
*/
    }
}

void HiLoFilterAudioProcessor::processHiFilter(float *channelData, const int channel, const int numFrames) {
    for(int i = 0; i < numFrames; ++i) {
        lastHiInput3[channel] = lastHiInput2[channel];
        lastHiInput2[channel] = lastHiInput1[channel];
        lastHiInput1[channel] = channelData[i];

        channelData[i] = (float)((hiCoeffA1 * lastHiInput1[channel]) +
                                 (hiCoeffA2 * lastHiInput2[channel]) +
                                 (hiCoeffA1 * lastHiInput3[channel]) -
                                 (hiCoeffB1 * lastHiOutput1[channel]) -
                                 (hiCoeffB2 * lastHiOutput2[channel]));

        lastHiOutput2[channel] = lastHiOutput1[channel];
        lastHiOutput1[channel] = channelData[i];
    }
}

void HiLoFilterAudioProcessor::processLoFilter(float *channelData, const int channel, const int numFrames) {
    for(int i = 0; i < numFrames; ++i) {
        lastLoInput3[channel] = lastLoInput2[channel];
        lastLoInput2[channel] = lastLoInput1[channel];
        lastLoInput1[channel] = channelData[i];

        channelData[i] = (float)((loCoeffA1 * lastLoInput1[channel]) +
                                 (loCoeffA2 * lastLoInput2[channel]) +
                                 (loCoeffA1 * lastLoInput3[channel]) -
                                 (loCoeffB1 * lastLoOutput1[channel]) -
                                 (loCoeffB2 * lastLoOutput2[channel]));

        lastLoOutput2[channel] = lastLoOutput1[channel];
        lastLoOutput1[channel] = channelData[i];
    }
}

void HiLoFilterAudioProcessor::releaseResources() {
    TeragonPluginBase::releaseResources();
}

void HiLoFilterAudioProcessor::onParameterUpdated(const Parameter *) {
    recalculateCoefficients();
}

AudioProcessorEditor *HiLoFilterAudioProcessor::createEditor() {
    return new HiLoFilterAudioProcessorEditor(this, parameters, Resources::getCache());
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new HiLoFilterAudioProcessor();
}
