/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace defaults
{
    const float minSlider = -18.f;
    const float maxSlider = 18.f;
    const float sliderStep = 1.f;
}

class GgconvolverAudioProcessorEditor  : public AudioProcessorEditor,
    private Slider::Listener, ComboBox::Listener
{
public:
    GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor&);
    ~GgconvolverAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBox) override;

    ComboBox irChoice;
    Slider preLevelSlider;
    Label preLevelLabel;
    Slider postLevelSlider;
    Label postLevelLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GgconvolverAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GgconvolverAudioProcessorEditor)
};
