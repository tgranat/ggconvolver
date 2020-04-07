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
    setSize (300, 200);

    //preLevelSlider.setRange(defaults::minSlider, defaults::maxSlider, defaults::sliderStep);
    //pregainSlider.setSliderStyle(Slider::LinearVertical);
    //preLevelSlider.addListener(this);
    //preLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    //preLevelSlider.setTextValueSuffix(" dB");
    //addAndMakeVisible(preLevelSlider);
    //preLevelLabel.setText("Pre Level", dontSendNotification);
    //preLevelLabel.attachToComponent(&preLevelSlider, true);
    //addAndMakeVisible(preLevelLabel);

    //postLevelSlider.setRange(defaults::minSlider, defaults::maxSlider, defaults::sliderStep);
    //postLevelSlider.setSliderStyle(Slider::Rotary);
    postLevelSlider.setRange(0.f, 3.f, 0.01);
    postLevelSlider.setValue(1.0);
    postLevelSlider.setSkewFactorFromMidPoint(1.0);
    postLevelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    postLevelSlider.addListener(this);
    //postLevelSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(postLevelSlider);

    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    postLevelLabel.setText("Level", dontSendNotification);
    postLevelLabel.attachToComponent(&postLevelSlider, false);
    addAndMakeVisible(postLevelLabel);

    //StringArray irNames = StringArray(&BinaryData::namedResourceList[0], BinaryData::namedResourceListSize);

    for (int i = 0; i < BinaryData::namedResourceListSize; i++) {
        irChoice.addItem(BinaryData::namedResourceList[i], i+1);
    }
    
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
    auto sliderLeft = 10;
    //preLevelSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    postLevelSlider.setBounds(sliderLeft, 30, getWidth() - sliderLeft - 10, 20);
    irChoice.setBounds(sliderLeft, 70, getWidth() - sliderLeft - 10, 20);
    //irChoice.setBounds(50, 90, 200, 50);
    //pregainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
void GgconvolverAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Need to convert dB to gain
    if (&postLevelSlider == slider) {
        //processor.mOutLevel = (float)Decibels::decibelsToGain(postLevelSlider.getValue());
        processor.mOutLevel = postLevelSlider.getValue();
    }
}

void GgconvolverAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
    processor.mIrNumber = comboBox->getSelectedId();
    int resourceListIndex = comboBox->getSelectedId() - 1;
    String irName = BinaryData::namedResourceList[resourceListIndex];
    processor.mIrData = BinaryData::getNamedResource(irName.toRawUTF8(), processor.mIrSize);

}

