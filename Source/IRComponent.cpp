/*
  ==============================================================================

    IRComponent.cpp
    Created: 13 Apr 2020 7:48:47pm
    Author:  tobbe

  ==============================================================================
*/

#include <JuceHeader.h>
#include "IRComponent.h"

//==============================================================================
IRComponent::IRComponent(GgconvolverAudioProcessor& p) : processor(p)
{
    // SPEAKER CHOICE DROPDOWN
    // Populate ComboBox from resources in BinaryData
    // In the GUI, the strings from the ComboBox are displayed.
    // If no GUI is used, the strings from the AudioParameterChoice are used.
    // The AudioParameterChoice in the processor and the ComboBox here has to correspond.
    for (int i = 0; i < BinaryData::namedResourceListSize; i++) {
        irChoice.addItem(BinaryData::namedResourceList[i], i + 1);
    }

    addAndMakeVisible(irChoice);
    mIrChoiceAttachement = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.getAPVTS(), "IRCHOICE", irChoice);
}

IRComponent::~IRComponent()
{
}

void IRComponent::paint (Graphics& g)
{
}

void IRComponent::resized()
{
    int sliderLeft = 10;
    int sliderRow = 50;

    irChoice.setBounds(sliderLeft, sliderRow + 120, getWidth() - sliderLeft - 10, 20);
}
