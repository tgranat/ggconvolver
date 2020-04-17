/*
  ==============================================================================

    IRComponent.h
    Created: 13 Apr 2020 7:48:47pm
    Author:  tobbe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class IRComponent    : public Component
{
public:
    IRComponent(GgconvolverAudioProcessor& p);
    ~IRComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    ComboBox irChoice;

    Rectangle<int> mIrFrame;

    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mIrChoiceAttachement;

    GgconvolverAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRComponent)
};
