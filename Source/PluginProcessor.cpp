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
    FilterState newFilterState = kHiLoFilterStatePassthru;
    if(currentFilterPosition > getHiFilterCutoffPosition()) {
        newFilterState = kHiLoFilterStateHi;
    }
    else if(currentFilterPosition < getLoFilterCutoffPosition()) {
        newFilterState = kHiLoFilterStateLo;
    }

    if(newFilterState != filterState) {
        filterState = newFilterState;
        // resetLastIOData();
        recalculateCoefficients();
    }
}

void HiLoFilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    TeragonPluginBase::prepareToPlay(sampleRate, samplesPerBlock);
    resetLastIOData();
    recalculateCoefficients();
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

void HiLoFilterAudioProcessor::recalculateCoefficients() {
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
}

double HiLoFilterAudioProcessor::getFilterFrequency() {
    double newFrequency = 0.0;
    const double position = filterPosition->getValue();
    const double loCutoff = getLoFilterCutoffPosition();
    // Will represent how much of the filter should be applied, after considering which half
    // of the filter we are in and subtracting the dead zone size. The result will be a value
    // between 0.0-1.0 which would correspond to a separate knob for the given filter.
    double relativePosition;

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

    printf("Filter position: %g, frequency: %g\n", position, newFrequency);
    if(newFrequency < 0.0) {
        newFrequency = 0.0;
    }
    else if(newFrequency > kMaxFilterFrequency) {
        newFrequency = kMaxFilterFrequency;
    }
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
        const int numFrames = buffer.getNumSamples();
        for(int i = 0; i < numFrames; ++i) {
            lastInput3[channel] = lastInput2[channel];
            lastInput2[channel] = lastInput1[channel];
            lastInput1[channel] = channelData[i];

            switch(filterState) {
                case kHiLoFilterStateHi:
                    channelData[i] = (float)((hiCoeffA1 * lastInput1[channel]) +
                        (hiCoeffA2 * lastInput2[channel]) +
                        (hiCoeffA1 * lastInput3[channel]) -
                        (hiCoeffB1 * lastOutput1[channel]) -
                        (hiCoeffB2 * lastOutput2[channel]));
                    break;
                case kHiLoFilterStateLo:
                    channelData[i] = (float)((loCoeffA1 * lastInput1[channel]) +
                        (loCoeffA2 * lastInput2[channel]) +
                        (loCoeffA1 * lastInput3[channel]) -
                        (loCoeffB1 * lastOutput1[channel]) -
                        (loCoeffB2 * lastOutput2[channel]));
                    break;
                default:
                    break;
            }

            lastOutput2[channel] = lastOutput1[channel];
            lastOutput1[channel] = channelData[i];
        }
    }
}

void HiLoFilterAudioProcessor::processHiFilter(float *channelData, const int channel, const int numFrames) {
    for(int i = 0; i < numFrames; ++i) {
        lastInput3[channel] = lastInput2[channel];
        lastInput2[channel] = lastInput1[channel];
        lastInput1[channel] = channelData[i];

        channelData[i] = (float)((hiCoeffA1 * lastInput1[channel]) +
                                 (hiCoeffA2 * lastInput2[channel]) +
                                 (hiCoeffA1 * lastInput3[channel]) -
                                 (hiCoeffB1 * lastOutput1[channel]) -
                                 (hiCoeffB2 * lastOutput2[channel]));

        lastOutput2[channel] = lastOutput1[channel];
        lastOutput1[channel] = channelData[i];
    }
}

void HiLoFilterAudioProcessor::processLoFilter(float *channelData, const int channel, const int numFrames) {
    for(int i = 0; i < numFrames; ++i) {
        lastInput3[channel] = lastInput2[channel];
        lastInput2[channel] = lastInput1[channel];
        lastInput1[channel] = channelData[i];

        channelData[i] = (float)((loCoeffA1 * lastInput1[channel]) +
                                 (loCoeffA2 * lastInput2[channel]) +
                                 (loCoeffA1 * lastInput3[channel]) -
                                 (loCoeffB1 * lastOutput1[channel]) -
                                 (loCoeffB2 * lastOutput2[channel]));

        lastOutput2[channel] = lastOutput1[channel];
        lastOutput1[channel] = channelData[i];
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
