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
#include "LookAndFeelHelp.h"

class GgconvolverAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor&);
    ~GgconvolverAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    void paintFrame(float x, float y, Graphics& g);
    void paintFrameHalf(float x, float y, Graphics& g);

    LookAndFeelHelp basicLookAndFeel;

    ComboBox irChoice;
    Label levelLabel;
    Slider levelSlider;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLevelAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mIrChoiceAttachement;

    EQComponent mEQ;

    GgconvolverAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GgconvolverAudioProcessorEditor)
};
