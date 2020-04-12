/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
GgconvolverAudioProcessor::GgconvolverAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
         #if ! JucePlugin_IsMidiEffect
             #if ! JucePlugin_IsSynth
              .withInput("Input", AudioChannelSet::stereo(), true)
             #endif
              .withOutput("Output", AudioChannelSet::stereo(), true)
         #endif
              ), mAPVTS(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    mAPVTS.state.addListener(this);
}

GgconvolverAudioProcessor::~GgconvolverAudioProcessor()
{
}

//==============================================================================
const String GgconvolverAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GgconvolverAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GgconvolverAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GgconvolverAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GgconvolverAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GgconvolverAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GgconvolverAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GgconvolverAudioProcessor::setCurrentProgram (int index)
{
}

const String GgconvolverAudioProcessor::getProgramName (int index)
{
    return {};
}

void GgconvolverAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
// Called when starting a plugin (not when enabled after been disabled)
// Same as VST AudioEffect::setActive(1)
void GgconvolverAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    updateParams();

    // Initiate filters
    mLowShelfFilters[0].setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, Constant::lowShelfFrequency, Constant::lowShelfFilterQ, mLowShelfGain));
    mLowShelfFilters[1].setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, Constant::lowShelfFrequency, Constant::lowShelfFilterQ, mLowShelfGain));
    mCurrentLowShelfGain = mLowShelfGain;
    mHighShelfFilters[0].setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, Constant::highShelfFrequency, Constant::highShelfFilterQ, mHighShelfGain));
    mHighShelfFilters[1].setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, Constant::highShelfFrequency, Constant::highShelfFilterQ, mHighShelfGain));
    mCurrentHighShelfGain = mHighShelfGain;
    mMidPeakFilters[0].setCoefficients(IIRCoefficients::makePeakFilter(sampleRate, mMidPeakFrequency, mMidPeakQ, mMidPeakGain));
    mMidPeakFilters[1].setCoefficients(IIRCoefficients::makePeakFilter(sampleRate, mMidPeakFrequency, mMidPeakQ, mMidPeakGain));
    mCurrentMidPeakFrequency = mMidPeakFrequency;
    mCurrentMidPeakQ = mMidPeakQ;
    mCurrentMidPeakGain = mMidPeakGain;

    // Initialize convolution  
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    mConvolution.prepare(spec);
    // Set first impulse response
    updateConvolution();
    mCurrentIrLoaded = mIrNumber;
 }

// Called when plugin removed (not when only disabled)
// Also called during startup of plugin (at least on Reaper: first activate, then deactivate, then activate again) 
// Same as VST AudioEffect::setActive(0)
void GgconvolverAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GgconvolverAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GgconvolverAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (mParamsHaveBeenUpdated) {
       updateParams();
       mParamsHaveBeenUpdated = false;
    }

    // Update IR if it has been changed in GUI
    if (mIrNumber != mCurrentIrLoaded) {
        updateConvolution();
        mCurrentIrLoaded = mIrNumber;
    }

    //float preRMSLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    // Process impulse response
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    mConvolution.process(context);

    // Process filters
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        mLowShelfFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        mMidPeakFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        mHighShelfFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
    }

    // Update filters that have been updated in gui
     if (mLowShelfGain != mCurrentLowShelfGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mLowShelfFilters[channel].setCoefficients(IIRCoefficients::makeLowShelf(getSampleRate(), Constant::lowShelfFrequency, Constant::lowShelfFilterQ, mLowShelfGain));
        }
        mCurrentLowShelfGain = mLowShelfGain;
    }
    if (mHighShelfGain != mCurrentHighShelfGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mHighShelfFilters[channel].setCoefficients(IIRCoefficients::makeHighShelf(getSampleRate(), Constant::highShelfFrequency, Constant::highShelfFilterQ, mHighShelfGain));
        }
        mCurrentHighShelfGain = mHighShelfGain;
    }

    if (mMidPeakFrequency != mCurrentMidPeakFrequency || mMidPeakQ != mCurrentMidPeakQ || mMidPeakGain != mCurrentMidPeakGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mMidPeakFilters[channel].setCoefficients(IIRCoefficients::makePeakFilter(getSampleRate(), mMidPeakFrequency, mMidPeakQ, mMidPeakGain));
        }
        mCurrentMidPeakFrequency = mMidPeakFrequency;
        mCurrentMidPeakQ = mMidPeakQ;
        mCurrentMidPeakGain = mMidPeakGain;
    }

    // Apply output level + compensating gain to achieve that the perceived loudness sounds good (to me)
    buffer.applyGain(mOutLevel * Constant::compensatingOutGain);
}

//==============================================================================
bool GgconvolverAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GgconvolverAudioProcessor::createEditor()
{
    return new GgconvolverAudioProcessorEditor (*this);
    //return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void GgconvolverAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GgconvolverAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // Note! This method may be called one or several times at startup of a plugin. This means
    // the stored data will be overwritten with default/start values unless you
    // have some logic here
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GgconvolverAudioProcessor();
}

// Initiate/update convolution engine with parameters set by GUI
void GgconvolverAudioProcessor::updateConvolution() {
    mConvolution.reset();
    mConvolution.loadImpulseResponse(mIrData, mIrSize, true, false, 0, true);
}

AudioProcessorValueTreeState::ParameterLayout GgconvolverAudioProcessor::createParameters() {
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<AudioParameterFloat>("LEVEL", "Level", 0.0, 3.0, 1.5));
    parameters.push_back(std::make_unique<AudioParameterFloat>("LOW", "Low", 0.05f, 1.95f, 1.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("MID", "Mid", 0.05f, 1.95f, 1.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("HIGH", "High", 0.05f, 1.95f, 1.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("MID FREQ", "Mid Freq", 200.f, 4000.f, 2100.f));
    parameters.push_back(std::make_unique<AudioParameterBool>("BANDWIDTH1", "BW 1 oct", false));
    parameters.push_back(std::make_unique<AudioParameterBool>("BANDWIDTH2", "BW 2 oct", true));

    // ComboBox is populated in editor.
    // Workaround: We need to do a "dummy" initiation of choices in the AudioParameterChoice that
    // corresponds to the number of choices in the ComboBox.
    StringArray dummyChoices;
    for (int i = 0; i < BinaryData::namedResourceListSize; i++) {
        dummyChoices.add("dummy data");
    }
    parameters.push_back(std::make_unique<AudioParameterChoice>("IRCHOICE", "IR Choice", dummyChoices, 1));

    return { parameters.begin(), parameters.end() };
}

void GgconvolverAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) {
    mParamsHaveBeenUpdated = true;
}

// update params from gui. 
void GgconvolverAudioProcessor::updateParams() {
    // Level gain
    mOutLevel = mAPVTS.getRawParameterValue("LEVEL")->load();
    // Low shelving filter gain
    mLowShelfGain = mAPVTS.getRawParameterValue("LOW")->load();
    // Mid peak gain
    mMidPeakGain = mAPVTS.getRawParameterValue("MID")->load();
    // Mid peak frequency
    mMidPeakFrequency = mAPVTS.getRawParameterValue("MID FREQ")->load();
    // Mid peak q
    mBandwidth1 = mAPVTS.getRawParameterValue("BANDWIDTH1")->load();
    mBandwidth2 = mAPVTS.getRawParameterValue("BANDWIDTH2")->load();
    if (mBandwidth1) mMidPeakQ = Constant::oneOctaveQ;
    if (mBandwidth2) mMidPeakQ = Constant::twoOctavesQ;
    // High shelving filter gain
    mHighShelfGain = mAPVTS.getRawParameterValue("HIGH")->load();
    // IR data id
    mIrNumber = mAPVTS.getRawParameterValue("IRCHOICE")->load();
    // Size of IR
    String irName = BinaryData::namedResourceList[mIrNumber];
    mIrData = BinaryData::getNamedResource(irName.toRawUTF8(), mIrSize);

}