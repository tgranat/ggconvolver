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
    : AudioProcessorEditor (&p), mEQ(p), processor (p)
{
    addAndMakeVisible(mEQ);

    setSize (300, 200);

    Font controlFont("Ariel", 13.0f, Font::plain);

    // Init look and feel
    basicLookAndFeel.setColour(LookAndFeelHelp::ColourTarget::tip, Colours::blue);
    basicLookAndFeel.setColour(LookAndFeelHelp::ColourTarget::top, Colours::grey);


    
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

    mEQ.setBounds(0, 0, 300, 200);

    irChoice.setBounds(sliderLeft, sliderRow + 120, getWidth() - sliderLeft - 10, 20);

   
}

