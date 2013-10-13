/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_B6965184E577F651__
#define __JUCE_HEADER_B6965184E577F651__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "TeragonGuiComponents.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class HiLoFilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    //==============================================================================
    HiLoFilterAudioProcessorEditor (AudioProcessor *owner, teragon::PluginParameterSet &p, teragon::ResourceCache *r);
    ~HiLoFilterAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();

    // Binary resources:
    static const char* background_png;
    static const int background_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    teragon::PluginParameterSet &parameters;
    teragon::ResourceCache *resources;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<teragon::ImageKnob> filterPositionKnob;
    ScopedPointer<teragon::ImageKnob> hiFilterLimitKnob;
    ScopedPointer<teragon::ImageKnob> loFilterLimitKnob;
    ScopedPointer<teragon::ImageKnob> deadZoneSizeKnob;
    ScopedPointer<teragon::ImageKnob> resonanceKnob;
    Image cachedImage_background_png;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLoFilterAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B6965184E577F651__
