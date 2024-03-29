/*
  ==============================================================================

    LookAndFeelHelp.h
    Created: 13 Apr 2020 4:48:22pm
    Author:  tobbe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// Helper class for customizing look and feel. 
// Code copied from LookAndFeel_V2 and adapted

class LookAndFeelHelp : public LookAndFeel_V4 {
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        (void) slider; // unused
        auto radius = jmin(width / 2, height / 2) - 8.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        // fill
        //g.setColour(findColour(ColourTarget::top));
        //g.fillEllipse(rx, ry, rw, rw);
        drawGlassSphere(g, rx, ry, rw, findColour(ColourTarget::top), 1.f);

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
    // From LookAndFeel_V2
    void drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        (void)isMouseOverButton; // unused
        auto font = getTextButtonFont(button, button.getHeight());
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
            : TextButton::textColourOffId)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        auto yIndent = jmin(4, button.proportionOfHeight(0.3f));
        auto cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

        auto fontHeight = roundToInt(font.getHeight() * 0.6f);
        auto leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        auto rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;

        auto edge = 4;
        auto offset = isButtonDown ? edge / 2 : 0;

        if (textWidth > 0)
            g.drawFittedText(button.getButtonText(),
                leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                Justification::centred, 2);
    }


    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        const int width = button.getWidth();
        const int height = button.getHeight();

        const float outlineThickness = button.isEnabled() ? ((isButtonDown || isMouseOverButton) ? 1.2f : 0.7f) : 0.4f;
        const float halfThickness = outlineThickness * 0.5f;

        const float indentL = button.isConnectedOnLeft() ? 0.1f : halfThickness;
        const float indentR = button.isConnectedOnRight() ? 0.1f : halfThickness;
        const float indentT = button.isConnectedOnTop() ? 0.1f : halfThickness;
        const float indentB = button.isConnectedOnBottom() ? 0.1f : halfThickness;


        const Colour baseColour(createBaseColour(button.getToggleState() ? findColour(ColourTarget::tip) : backgroundColour,
            button.hasKeyboardFocus(true),
            isMouseOverButton,
            isButtonDown)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));


        LookAndFeel_V2::drawGlassLozenge(g,
            indentL,
            indentT,
            width - indentL - indentR,
            height - indentT - indentB,
            baseColour,
            outlineThickness, -1.0f,
            button.isConnectedOnLeft(),
            button.isConnectedOnRight(),
            button.isConnectedOnTop(),
            button.isConnectedOnBottom());
    }
    enum ColourTarget { tip, top };

private:
    static Colour createBaseColour(Colour buttonColour,
        bool hasKeyboardFocus,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour(buttonColour.withMultipliedSaturation(sat));

        if (shouldDrawButtonAsDown)        return baseColour.contrasting(0.2f);
        if (shouldDrawButtonAsHighlighted) return baseColour.contrasting(0.1f);

        return baseColour;
    }
};
