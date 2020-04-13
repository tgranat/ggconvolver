/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EQComponent.h"
#include "IRComponent.h"
#include "LookAndFeelHelp.h"

class GgconvolverAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor&);
    ~GgconvolverAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:

    EQComponent mLevelAndEQ;
    IRComponent mIrChoiceDropdown;

    GgconvolverAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GgconvolverAudioProcessorEditor)
};
