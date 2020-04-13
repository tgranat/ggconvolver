/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
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
    LookAndFeelHelp lowSliderLookAndFeel;
    LookAndFeelHelp freqSliderLookAndFeel;

    ComboBox irChoice;
    Slider levelSlider;
    Slider lowSlider;
    Slider highSlider;
    Slider midSlider;
    Slider midFrequencySlider;
    Label levelLabel;
    Label lowLabel;
    Label highLabel;
    Label midLabel;
    Label midFrequencyLabel;
    Label midBwLabel;
    TextButton midBw2OctButton { "2 oct." };
    TextButton midBw1OctButton{ "1 oct." };

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLevelAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidFreqencyAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mIrChoiceAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mBandwidth1Attachement;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mBandwidth2Attachement;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GgconvolverAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GgconvolverAudioProcessorEditor)
};
