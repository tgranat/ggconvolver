/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GgconvolverAudioProcessorEditor::GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor& p)
    : AudioProcessorEditor (&p), mLevelAndEQ(p), mIrChoiceDropdown(p), processor (p)
{
    addAndMakeVisible(mLevelAndEQ);
    addAndMakeVisible(mIrChoiceDropdown);
    setSize (300, 300);
}

GgconvolverAudioProcessorEditor::~GgconvolverAudioProcessorEditor()
{
}


//==============================================================================
void GgconvolverAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour(Colours::white);
    g.setFont(Font("Ariel", 15.0f, Font::bold));
    g.drawFittedText("Simple Speaker Simulator", 0, 0, getWidth(), 30, Justification::centred, 1);
 }

void GgconvolverAudioProcessorEditor::resized()
{
    mLevelAndEQ.setBounds(0, 25, 300, 249);
    mIrChoiceDropdown.setBounds(0, 270, 300, 30);
}

