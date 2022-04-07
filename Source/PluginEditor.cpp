/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "config.h"

//==============================================================================
GgconvolverAudioProcessorEditor::GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor& p)
    : AudioProcessorEditor (&p), mLevelAndEQ(p), mIrChoiceDropdown(p), processor (p)
{
    addAndMakeVisible(mLevelAndEQ);
    addAndMakeVisible(mIrChoiceDropdown);
    setSize (300, 350);
}

GgconvolverAudioProcessorEditor::~GgconvolverAudioProcessorEditor()
{
}


//==============================================================================
void GgconvolverAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour(Colours::white);
    g.setFont(Font("Ariel", 15.0f, Font::bold));
    std::string version = PROJECT_VERSION; // from config.h
    g.drawFittedText("Simple Speaker Simulator " + version, 0, 0, getWidth(), 30, Justification::centred, 1);
 }

void GgconvolverAudioProcessorEditor::resized()
{
    mLevelAndEQ.setBounds(0, 25, 300, 299);
    mIrChoiceDropdown.setBounds(0, 320, 300, 30);
}

