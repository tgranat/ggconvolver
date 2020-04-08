/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// Customize slider look and feel
class SliderLookAndFeel : public LookAndFeel_V4 {
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin(width / 2, height / 2) - 4.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        // fill
        g.setColour(findColour(ColourTarget::top));
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(Colours::darkgrey);
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        Path p;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 4.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        // pointer
        g.setColour(findColour(ColourTarget::tip));
        g.fillPath(p);

    }
    enum ColourTarget {tip, top};
 };

class GgconvolverAudioProcessorEditor  : public AudioProcessorEditor,
    private Slider::Listener, ComboBox::Listener
{
public:
    GgconvolverAudioProcessorEditor (GgconvolverAudioProcessor&);
    ~GgconvolverAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBox) override;

    SliderLookAndFeel levelSliderLookAndFeel;
    SliderLookAndFeel lowSliderLookAndFeel;


    ComboBox irChoice;
    Slider levelSlider;
    Slider lowSlider;
    Slider highSlider;
    Label levelLabel;
    Label lowLabel;
    Label highLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GgconvolverAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GgconvolverAudioProcessorEditor)
};
