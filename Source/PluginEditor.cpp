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
    setSize (300, 200);

    // LEVEL SLIDER
    levelSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    levelSlider.setRange(0.f, 3.f, 0.01);
    levelSlider.setValue(1.0);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    levelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    levelSlider.addListener(this);
    // Set customized look and feel
    levelSliderLookAndFeel.setColour(SliderLookAndFeel::ColourTarget::tip, Colours::blue);
    levelSliderLookAndFeel.setColour(SliderLookAndFeel::ColourTarget::top, Colours::grey);

    levelSlider.setLookAndFeel(&levelSliderLookAndFeel);
    addAndMakeVisible(levelSlider);

    // LEVEL LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    levelLabel.setFont(Font("Ariel", 13.0f, Font::plain));
    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setJustificationType(Justification::centredTop);
    levelLabel.attachToComponent(&levelSlider, false);
    addAndMakeVisible(levelLabel);

    // LOW SHELF SLIDER
    lowSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    // Filter doesn't like gain = 0.0
    lowSlider.setRange(0.05f, 2.f, 0.01);
    lowSlider.setValue(1.0);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    lowSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //lowSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 40, 40);
    lowSlider.addListener(this);
    // Set customized look and feel
    lowSliderLookAndFeel.setColour(SliderLookAndFeel::ColourTarget::tip, Colours::yellow);
    lowSliderLookAndFeel.setColour(SliderLookAndFeel::ColourTarget::top, Colours::grey);

    lowSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(lowSlider);

    // LOW SHELF LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    lowLabel.setFont(Font("Ariel", 13.0f, Font::plain));
    lowLabel.setText("Low", dontSendNotification);
    lowLabel.setJustificationType(Justification::centredTop);
    lowLabel.attachToComponent(&lowSlider, false);
    addAndMakeVisible(lowLabel);

    // HIGH SHELF SLIDER
    highSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    highSlider.setRange(0.05f, 2.f, 0.01);
    highSlider.setValue(1.0);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    highSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    highSlider.addListener(this);
    // Set customized look and feel
    highSlider.setColour(SliderLookAndFeel::ColourTarget::tip, Colours::yellow);
    highSlider.setColour(SliderLookAndFeel::ColourTarget::top, Colours::grey);

    highSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(highSlider);

    // HIGH SHELF LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    highLabel.setFont(Font("Ariel", 13.0f, Font::plain));
    highLabel.setText("High", dontSendNotification);
    highLabel.setJustificationType(Justification::centredTop);
    highLabel.attachToComponent(&highSlider, false);
    addAndMakeVisible(highLabel);

    // SPEAKER CHOICE DROPDOWN
    // Populate from resources in BinaryData
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
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(Font("Ariel", 15.0f, Font::bold));
    g.drawFittedText("Simple Speaker Simulator", 0, 0, getWidth(), 30, Justification::centred, 1);

 }

void GgconvolverAudioProcessorEditor::resized()
{
    int sliderLeft = 10;
    int sliderRow = 60;
    int w = 50;
    int h = 50;
    levelSlider.setBounds(sliderLeft, sliderRow, w, h);
    lowSlider.setBounds(sliderLeft + w, sliderRow, w, h);
    highSlider.setBounds(sliderLeft + 2*w, sliderRow, w, h);
    irChoice.setBounds(sliderLeft, sliderRow + 70, getWidth() - sliderLeft - 10, 20);
    //irChoice.setBounds(50, 90, 200, 50);
   
}
void GgconvolverAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (&levelSlider == slider) {
        //processor.mOutLevel = (float)Decibels::decibelsToGain(postLevelSlider.getValue());
        processor.mOutLevel = levelSlider.getValue();
    }
    if (&lowSlider == slider) {
        processor.mLowShelfGain = lowSlider.getValue();
    }
    if (&highSlider == slider) {
        processor.mHighShelfGain = highSlider.getValue();
    }

}

void GgconvolverAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
    processor.mIrNumber = comboBox->getSelectedId();
    int resourceListIndex = comboBox->getSelectedId() - 1;
    String irName = BinaryData::namedResourceList[resourceListIndex];
    processor.mIrData = BinaryData::getNamedResource(irName.toRawUTF8(), processor.mIrSize);

}

