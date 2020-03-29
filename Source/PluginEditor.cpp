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
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::red);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    preLevelSlider.setRange(-10.0f, 10.0f, 0.5f);
    //pregainSlider.setSliderStyle(Slider::LinearVertical);
    preLevelSlider.addListener(this);
    //preLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    //preLevelSlider.setTextValueSuffix(" Volume");
    addAndMakeVisible(preLevelSlider);
    preLevelLabel.setText("Pre Level", dontSendNotification);
    preLevelLabel.attachToComponent(&preLevelSlider, true);
    addAndMakeVisible(preLevelLabel);

    postLevelSlider.setRange(-10.0f, 10.0f, 0.5f);
    postLevelSlider.addListener(this);
    addAndMakeVisible(postLevelSlider);

    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    postLevelLabel.setText("Post Level", dontSendNotification);
    postLevelLabel.attachToComponent(&postLevelSlider, true);
    addAndMakeVisible(postLevelLabel);

    
    irChoice.addItem("speaker 1", 1);
    irChoice.addItem("speaker 2", 2);
    irChoice.addItem("speaker 3", 3);
    irChoice.addItem("speaker 4", 4);
    irChoice.setSelectedId(1);
    irChoice.addListener(this);
    addAndMakeVisible(irChoice);
    
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
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

 }

void GgconvolverAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    preLevelSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    postLevelSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    irChoice.setBounds(50, 90, 200, 50);
    //pregainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
void GgconvolverAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (&preLevelSlider == slider) {
        processor.preLevel = preLevelSlider.getValue() / 10;
    }
    if (&postLevelSlider == slider) {
        processor.postLevel = postLevelSlider.getValue() / 10;
    }
}

void GgconvolverAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
    processor.menuChoice = comboBox->getSelectedId();
}

