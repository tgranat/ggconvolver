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
    const float compensatingOutGain = 1.5;

    const float lowShelfFilterQ = 1.41; // 1 octave = 1.41   2 octave = 0.667
    const float lowShelfFrequency = 200.f;
    const float midPeakFilterQ = 1.41;
    const float midPeakFrequency = 800.f;
    const float highShelfFilterQ = 1.41;
    const float highShelfFrequency = 4000.f;
    const float oneOctaveQ = 1.141f;
    const float twoOctavesQ = 0.667f;
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
 
    // Level gain
    float mOutLevel;
    // Low shelving filter gain
    float mLowShelfGain;
    // Mid peak gain
    float mMidPeakGain;
    // Mid peak frequency
    float mMidPeakFrequency;
    // Mid peak q
    float mMidPeakQ;
    // New:
    bool mBandwidth1;
    bool mBandwidth2;
    // High shelving filter gain
    float mHighShelfGain;
    // IR data id
    int mIrNumber;
    // Size of IR
    int mIrSize;
    // IR data
    const char* mIrData;

    AudioProcessorValueTreeState& getAPVTS() { return mAPVTS; }

private:
    void updateConvolution();
    
    AudioProcessorValueTreeState mAPVTS;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    void updateParams();
    dsp::Convolution mConvolution;

    IIRFilter mLowShelfFilters[2];
    IIRFilter mMidPeakFilters[2];
    IIRFilter mHighShelfFilters[2];
    //IIRFilter mLowShelfFilter1;
    //IIRFilter mLowShelfFilter2;

    int mCurrentIrLoaded = 1;
    float mCurrentLowShelfGain = 1.f;
    float mCurrentMidPeakGain = 1.f;
    float mCurrentMidPeakFrequency = 1000.f;
    float mCurrentMidPeakQ = Constant::midPeakFilterQ;
    float mCurrentHighShelfGain = 1.f;
 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GgconvolverAudioProcessor)
 
};
