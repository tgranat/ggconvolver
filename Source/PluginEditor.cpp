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
    levelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //levelSlider.setPopupDisplayEnabled(true, false, this);
    levelSlider.setLookAndFeel(&basicLookAndFeel);
    addAndMakeVisible(levelSlider);
    mLevelAttachement = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "LEVEL", levelSlider);


    // LEVEL LABEL
    levelLabel.setFont(controlFont);
    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setJustificationType(Justification::centredBottom);
    levelLabel.attachToComponent(&levelSlider, false);
    addAndMakeVisible(levelLabel);

    // LOW SHELF SLIDER
    lowSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    // Don't show text box with values
    lowSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //lowSlider.setPopupDisplayEnabled(true, false, this);
    lowSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(lowSlider);
    mLowAttachement = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "LOW", lowSlider);

    // LOW SHELF LABEL
    lowLabel.setFont(controlFont);
    lowLabel.setText("Low", dontSendNotification);
    lowLabel.setJustificationType(Justification::centredBottom);
    lowLabel.attachToComponent(&lowSlider, false);
    addAndMakeVisible(lowLabel);

    // MID PEAK SLIDER
    midSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    // Don't show text box with values
    midSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //midSlider.setPopupDisplayEnabled(true, false, this);
    midSlider.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midSlider);
    mMidAttachement = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "MID", midSlider);

    // MID PEAK LABEL
    midLabel.setFont(controlFont);
    midLabel.setText("Mid", dontSendNotification);
    midLabel.setJustificationType(Justification::centredBottom);
    midLabel.attachToComponent(&midSlider, false);
    addAndMakeVisible(midLabel);

    // MID FREQUENCY SLIDER
    midFrequencySlider.setSliderStyle(Slider::RotaryVerticalDrag);
    // Don't show text box with values
    midFrequencySlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //midFrequency.addListener(this);
    midFrequencySlider.setPopupDisplayEnabled(true, false, this);
    midFrequencySlider.setTextValueSuffix(" Hz");
    midFrequencySlider.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midFrequencySlider);
    mMidFreqencyAttachement = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "MID FREQ", midFrequencySlider);

    // MID FREQUENCY LABEL
    midFrequencyLabel.setFont(controlFont);
    midFrequencyLabel.setText("Freq", dontSendNotification);
    midFrequencyLabel.setJustificationType(Justification::centredBottom);
    midFrequencyLabel.attachToComponent(&midFrequencySlider, false);
    addAndMakeVisible(midFrequencyLabel);

    // MID BANDWIDTH BUTTONS
    midBw2OctButton.setClickingTogglesState(true);
    midBw2OctButton.setRadioGroupId(101);
    midBw2OctButton.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midBw2OctButton);
    mBandwidth2Attachement = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.getAPVTS(), "BANDWIDTH2", midBw2OctButton);

    midBw1OctButton.setClickingTogglesState(true);
    midBw1OctButton.setRadioGroupId(101);
    midBw1OctButton.setLookAndFeel(&freqSliderLookAndFeel);
    addAndMakeVisible(midBw1OctButton);
    mBandwidth1Attachement = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.getAPVTS(), "BANDWIDTH1", midBw1OctButton);


    midBwLabel.setFont(controlFont);
    midBwLabel.setText("BW", dontSendNotification);
    midBwLabel.setJustificationType(Justification::centredBottom);
    midBwLabel.attachToComponent(&midBw1OctButton, false);
    addAndMakeVisible(midBwLabel);


    // HIGH SHELF SLIDER
    highSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    highSlider.setRange(0.05f, 1.95f, 0.01);
    highSlider.setValue(1.f);
    // Don't show text box with values
    highSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    //highSlider.setPopupDisplayEnabled(true, false, this);
    highSlider.setLookAndFeel(&lowSliderLookAndFeel);
    addAndMakeVisible(highSlider);
    mHighAttachement = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "HIGH", highSlider);


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
    
    addAndMakeVisible(irChoice);   
    mIrChoiceAttachement = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.getAPVTS(), "IRCHOICE", irChoice);

}

GgconvolverAudioProcessorEditor::~GgconvolverAudioProcessorEditor()
{
}

void GgconvolverAudioProcessorEditor::paintFrame(float sx, float sy, Graphics& g) {
    Path path;
    path.startNewSubPath(sx + 5, sy - 10);
    path.lineTo(sx, sy - 10);
    path.lineTo(sx, sy + 55);
    path.startNewSubPath(sx + 50, sy + 55);
    path.lineTo(sx + 50, sy - 10);
    path.lineTo(sx + 45, sy - 10);
    g.strokePath(path, PathStrokeType(2.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::square));
}

void GgconvolverAudioProcessorEditor::paintFrameHalf(float sx, float sy, Graphics& g) {
    Path path;
    path.startNewSubPath(sx + 5, sy - 10);
    path.lineTo(sx - 20, sy - 10);
    path.lineTo(sx - 20, sy + 55);
    path.startNewSubPath(sx + 45, sy - 10);
    path.lineTo(sx + 70, sy - 10);
    path.lineTo(sx + 70, sy + 55);
    g.strokePath(path, PathStrokeType(2.0f, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::square));
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

    paintFrame(10, 50, g);
    paintFrame(10 + 60, 50, g);
    paintFrame(10 + 220, 50, g);
    paintFrameHalf(10 + 140, 50, g);
 }

void GgconvolverAudioProcessorEditor::resized()
{
    int sliderLeft = 10;
    int sliderRow = 50;
    int w = 50;
    int h = 50;
    levelSlider.setBounds(sliderLeft, sliderRow, w, h);
    lowSlider.setBounds(sliderLeft + 60, sliderRow, w, h);
    midSlider.setBounds(sliderLeft + 140, sliderRow, w, h);
    highSlider.setBounds(sliderLeft + 220, sliderRow, w, h);

    midFrequencySlider.setBounds(sliderLeft + 117, sliderRow + 60, w, h);

    midBw1OctButton.setBounds(sliderLeft + 165, sliderRow + 60, 40, 18);
    midBw2OctButton.setBounds(sliderLeft + 165, sliderRow + 85, 40, 18);

    irChoice.setBounds(sliderLeft, sliderRow + 120, getWidth() - sliderLeft - 10, 20);
    //irChoice.setBounds(50, 90, 200, 50);
   
}

