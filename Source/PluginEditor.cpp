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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    pregainSlider.setRange(-18.0, 12.0, 1.0);
    //pregainSlider.setSliderStyle(Slider::LinearVertical);
    pregainSlider.setTextValueSuffix(" dB");
    pregainSlider.addListener(this);
    addAndMakeVisible(pregainSlider);

    pregainLabel.setText("Pre gain", dontSendNotification);
    pregainLabel.attachToComponent(&pregainSlider, true);

    levelSlider.setRange(-18.0, 12.0, 1.0);
    levelSlider.setTextValueSuffix(" dB");
    levelSlider.addListener(this);
    addAndMakeVisible(levelSlider);

    levelLabel.setText("Level Out", dontSendNotification);
    levelLabel.attachToComponent(&levelSlider, true);

}

GgconvolverAudioProcessorEditor::~GgconvolverAudioProcessorEditor()
{
}

//==============================================================================
void GgconvolverAudioProcessorEditor::paint (Graphics& g)
{
    /*
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    */
        
}

void GgconvolverAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    pregainSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    levelSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    //pregainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
void GgconvolverAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
   
}
