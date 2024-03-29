/*
  ==============================================================================

    IRComponent.cpp
    Created: 13 Apr 2020 7:48:47pm
    Author:  tobbe

  ==============================================================================
*/

#include "IRComponent.h"
#include <BinaryData.h>

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
    g.setColour(Colours::white);
    mIrFrame = getLocalBounds().reduced(2, 2);
    g.drawRect(mIrFrame);
}

void IRComponent::resized()
{
    irChoice.setBounds(5, 5, 290, 20);
}
