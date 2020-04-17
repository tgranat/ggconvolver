/*
  ==============================================================================

    EQComponent.h
    Created: 13 Apr 2020 4:23:51pm
    Author:  tobbe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeelHelp.h"

//==============================================================================
/*
*/
class EQComponent    : public Component
{
public:
    EQComponent(GgconvolverAudioProcessor& p);
    ~EQComponent();

    void paint (Graphics&) override;
 
    void resized() override;

    Path frequencyResponse;

private:
    void paintFrame(float x, float y, Graphics& g);
    void paintFrameWide(float x, float y, Graphics& g);
    float getFrequencyForPosition(float pos);
    void updateFrequencyResponses();

    LookAndFeelHelp basicLookAndFeel;
    LookAndFeelHelp lowSliderLookAndFeel;
    LookAndFeelHelp midLookAndFeel;

    Label levelLabel;
    Slider levelSlider;
    Slider lowSlider;
    Slider highSlider;
    Slider midSlider;
    Slider midFrequencySlider;

    Label lowLabel;
    Label highLabel;
    Label midLabel;
    Label midFrequencyLabel;
    Label midBwLabel;

    TextButton midBw2OctButton{ "2 oct." };
    TextButton midBw1OctButton{ "1 oct." };

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLevelAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidFreqencyAttachement;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mBandwidth1Attachement;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mBandwidth2Attachement;

    Rectangle<int> mIrFrame;
    Rectangle<int> mPlotFrame;

    GgconvolverAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQComponent)
};
