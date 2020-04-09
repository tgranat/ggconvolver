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

    Font controlFont("Ariel", 13.0f, Font::plain);

    // Init look and feel
    basicLookAndFeel.setColour(MyLookAndFeel::ColourTarget::tip, Colours::blue);
    basicLookAndFeel.setColour(MyLookAndFeel::ColourTarget::top, Colours::grey);

    lowSliderLookAndFeel.setColour(MyLookAndFeel::ColourTarget::tip, Colours::yellow);
    lowSliderLookAndFeel.setColour(MyLookAndFeel::ColourTarget::top, Colours::grey);

    freqSliderLookAndFeel.setColour(MyLookAndFeel::ColourTarget::tip, Colours::green);
    freqSliderLookAndFeel.setColour(MyLookAndFeel::ColourTarget::top, Colours::grey);

    
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
 
    levelSlider.setLookAndFeel(&basicLookAndFeel);
    addAndMakeVisible(levelSlider);

    // LEVEL LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    levelLabel.setFont(controlFont);
    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setJustificationType(Justification::centredBottom);
    levelLabel.attachToComponent(&levelSlider, false);
    addAndMakeVisible(levelLabel);

    // LOW SHELF SLIDER
    lowSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    // Filter doesn't like gain = 0.0
    lowSlider.setRange(0.05f, 1.95f, 0.01);
    lowSlider.setValue(1.f);
    // Don't show text box with values
    lowSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    lowSlider.setPopupDisplayEnabled(true, false, this);
    lowSlider.addListener(this);
    // Set customized look and feel

    lowSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(lowSlider);

    // LOW SHELF LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    lowLabel.setFont(controlFont);
    lowLabel.setText("Low", dontSendNotification);
    lowLabel.setJustificationType(Justification::centredBottom);
    lowLabel.attachToComponent(&lowSlider, false);
    addAndMakeVisible(lowLabel);

    // MID PEAK SLIDER
    midSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    midSlider.setRange(0.05f, 1.95f, 0.01);
    midSlider.setValue(1.f);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    //levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    midSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    midSlider.addListener(this);
    midSlider.setPopupDisplayEnabled(true, false, this);
    midSlider.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midSlider);

    // MID PEAK LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    midLabel.setFont(controlFont);
    midLabel.setText("Mid", dontSendNotification);
    midLabel.setJustificationType(Justification::centredBottom);
    midLabel.attachToComponent(&midSlider, false);
    addAndMakeVisible(midLabel);

    // MID FREQUENCY SLIDER
    midFrequency.setSliderStyle(Slider::RotaryVerticalDrag);
    midFrequency.setRange(200.f, 4000.f, 1.f);
    midFrequency.setValue(2100.f);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    //levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    midFrequency.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    midFrequency.addListener(this);
    // Set customized look and feel
    midFrequency.setPopupDisplayEnabled(true, false, this);
    midFrequency.setTextValueSuffix(" Hz");
    midFrequency.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midFrequency);

    // MID FREQUENCY LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    midFrequencyLabel.setFont(controlFont);
    midFrequencyLabel.setText("Freq", dontSendNotification);
    midFrequencyLabel.setJustificationType(Justification::centredBottom);
    midFrequencyLabel.attachToComponent(&midFrequency, false);
    addAndMakeVisible(midFrequencyLabel);

    // MID BANDWIDTH BUTTONS
    midBw2OctButton.setClickingTogglesState(true);
    midBw2OctButton.setRadioGroupId(101);
    midBw2OctButton.onClick = [this] { GgconvolverAudioProcessorEditor::updateToggleState(&midBw2OctButton, 0.667f); };
    // Set this button as default on at startup
    midBw2OctButton.setToggleState(true, false);
    midBw2OctButton.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midBw2OctButton);

    midBw1OctButton.setClickingTogglesState(true);
    midBw1OctButton.setRadioGroupId(101);
    midBw1OctButton.onClick = [this] { GgconvolverAudioProcessorEditor::updateToggleState(&midBw1OctButton, 1.141f); };
    addAndMakeVisible(midBw1OctButton);
    midBwLabel.setFont(controlFont);
    midBwLabel.setText("BW", dontSendNotification);
    midBwLabel.setJustificationType(Justification::centredBottom);
    midBwLabel.attachToComponent(&midBw1OctButton, false);
    midBw1OctButton.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midBwLabel);


    // HIGH SHELF SLIDER
    highSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    highSlider.setRange(0.05f, 1.95f, 0.01);
    highSlider.setValue(1.f);
    // Sets mid point to middle of slider even though 1.0 not is in the middle of the range
    //levelSlider.setSkewFactorFromMidPoint(1.0);
    // Don't show text box with values
    highSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    highSlider.addListener(this);
    highSlider.setPopupDisplayEnabled(true, false, this);
    highSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(highSlider);

    // HIGH SHELF LABEL
    //postLevelLabel.setFont(Font(15.0f, Font::bold));
    highLabel.setFont(controlFont);
    highLabel.setText("High", dontSendNotification);
    highLabel.setJustificationType(Justification::centredBottom);
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
    int sliderRow = 50;
    int w = 70;
    int h = 50;
    levelSlider.setBounds(sliderLeft, sliderRow, w, h);
    lowSlider.setBounds(sliderLeft + w, sliderRow, w, h);
    midSlider.setBounds(sliderLeft + 2*w, sliderRow, w, h);
    highSlider.setBounds(sliderLeft + 3*w, sliderRow, w, h);

    midFrequency.setBounds(sliderLeft + 2 * w - 30, sliderRow + 60, w, h);

    midBw1OctButton.setBounds(sliderLeft + 2 * w + 30, sliderRow + 60, 50, 18);
    midBw2OctButton.setBounds(sliderLeft + 2 * w + 30, sliderRow + 85, 50, 18);

    irChoice.setBounds(sliderLeft, sliderRow + 120, getWidth() - sliderLeft - 10, 20);
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
void GgconvolverAudioProcessorEditor::updateToggleState(Button* button, float midPeakQ) {
    processor.mMidPeakQ = midPeakQ;
}

