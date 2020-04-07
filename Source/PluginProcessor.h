/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Constant {
    // Compensate for lost level in convolver. Seems to lose 50% in convolver.
    // Increase the gain somewhat more so the perceived loudness sounds good (to me)
    const float compensatingOutGain = 2.5;
}
//==============================================================================
/**
*/
class GgconvolverAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    GgconvolverAudioProcessor();
    ~GgconvolverAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // UI parameters
 
    AudioParameterChoice* mIrChoice;
    AudioParameterFloat* mPostLevel;
 
    //int menuChoice;
    //float preLevel = 0.f;
    //float postLevel = 0.0f;

private:
    void updateConvolution();

    
    dsp::Convolution mConvolution;
    int mCurrentIrLoaded = 0;

 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GgconvolverAudioProcessor)
 
};
