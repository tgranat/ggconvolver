/*
  ==============================================================================

    EQComponent.h
    Created: 13 Apr 2020 4:23:51pm
    Author:  tobbe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EQComponent    : public Component
{
public:
    EQComponent();
    ~EQComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQComponent)
};
