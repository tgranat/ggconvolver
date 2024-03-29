/*
  ==============================================================================

    Simple Speaker Simulator (ggconvolver) processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Analyser.h"

namespace Constant {
    // Compensate for lost level in convolver. Seems to lose 50% in convolver.
    // Increase the gain somewhat more so the perceived loudness sounds good (to me)
    const float compensatingOutGain = 2.f;
    // Hardcoded and default values
    const float lowShelfFilterQ = 0.667f;
    const float lowShelfFrequency = 200.f;
    const float midPeakFilterQ = 1.141f;
    const float midPeakFrequencyLow = 100.f;
    const float midPeakFrequencyHigh = 5000.f;
    const float midPeakFrequency = 1000.f;
    const float highShelfFilterQ = 0.667f;
    const float highShelfFrequency = 4000.f;
    const float oneOctaveQ = 1.141f;
    const float twoOctavesQ = 0.667f;
    const float maxDb = 12.f;                     // Max level dB in graph
    const float maxParameterGain = 12.f;          // Max/min gain in sliders
}
//==============================================================================
/**
*/
class GgconvolverAudioProcessor  : public AudioProcessor,
                                   public ValueTree::Listener,
                                   public ChangeBroadcaster
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

	const std::vector<double>& getMagnitudes();
    
    void createFrequencyPlot(Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);

    void createAnalyserPlot(Path& p, const Rectangle<int> bounds, float minFreq);
    bool checkForNewAnalyserData();

    float getMaxDb() { return Constant::maxDb; }

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
    void updateConvolution(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 totalNumInputChannels);
    
    AudioProcessorValueTreeState mAPVTS;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    void updateParams();
    void processFilters(AudioBuffer<float>* buffer, int totalNumInputChannels);

    Analyser outputAnalyser;

    dsp::Convolution mConvolution;
    dsp::ProcessSpec mSpec;

    IIRFilter mLowShelfFilters[2];
    IIRFilter mMidPeakFilters[2];
    IIRFilter mHighShelfFilters[2];

    dsp::IIR::Coefficients<float>::Ptr mLowCoefficients;
    dsp::IIR::Coefficients<float>::Ptr mMidCoefficients;
    dsp::IIR::Coefficients<float>::Ptr mHighCoefficients;

    std::vector<double> mFrequencies;
    std::vector<double> mMagnitudes;
    std::vector<double> mMidMagnitudes;
    std::vector<double> mLowMagnitudes;
    std::vector<double> mHighMagnitudes;

    int mCurrentIrLoaded;
    float mCurrentLowShelfGain;
    float mCurrentMidPeakGain;
    float mCurrentMidPeakFrequency;
    float mCurrentMidPeakQ;
    float mCurrentHighShelfGain;

    std::atomic<bool> mParamsHaveBeenUpdatedInGUI{ false };
 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GgconvolverAudioProcessor)
 
};
