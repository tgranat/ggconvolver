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
    )
#endif
{
    /*
     addParameter(mPostLevel = new AudioParameterFloat("postLevel",
        "Out Level",
        NormalisableRange<float>(0.1f, 2.f, 0.1f),
        1.0f,
        "dB",
        AudioProcessorParameter::genericParameter,
        [](float value, int) { return String(20*log10(value), 1); }));
       // [](float value, int) { return String((float)Decibels::decibelsToGain(value), 2); })); // Didnt work, dont know why

    StringArray irNames = StringArray(&BinaryData::namedResourceList[0], BinaryData::namedResourceListSize);
 
    addParameter(mIrChoice = new AudioParameterChoice(
        "irChoice",
        "Speaker",
        irNames,
        0));
        */
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
    // Initiate filters
    mLowShelfFilters[0].setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, Constant::lowShelfFrequency, Constant::lowShelfFilterQ, 1.0));
    mLowShelfFilters[1].setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, Constant::lowShelfFrequency, Constant::lowShelfFilterQ, 1.0));
    mCurrentLowShelfGain = 1.0;
    mHighShelfFilters[0].setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, Constant::highShelfFrequency, Constant::highShelfFilterQ, 1.0));
    mHighShelfFilters[1].setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, Constant::highShelfFrequency, Constant::highShelfFilterQ, 1.0));   
    mCurrentHighShelfGain = 1.0;
    mMidPeakFilters[0].setCoefficients(IIRCoefficients::makePeakFilter(sampleRate, Constant::midPeakFrequency, Constant::midPeakFilterQ, 1.0));
    mMidPeakFilters[1].setCoefficients(IIRCoefficients::makePeakFilter(sampleRate, Constant::midPeakFrequency, Constant::midPeakFilterQ, 1.0));
    mCurrentMidPeakFrequency = Constant::midPeakFrequency;
    mCurrentMidPeakQ = Constant::midPeakFilterQ;
    mCurrentMidPeakGain = 1.0f;

    // Set IR defaults
    mIrNumber = 1;
    String irName = BinaryData::namedResourceList[0];
    mIrData = BinaryData::getNamedResource(irName.toRawUTF8(), mIrSize);
    // Initialize convolution  
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    mConvolution.prepare(spec);
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

void GgconvolverAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Check if IR to use has been changed in GUI
    if (mIrNumber != mCurrentIrLoaded) {
        updateConvolution();
        mCurrentIrLoaded = mIrNumber;
    }

    //float preRMSLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    mConvolution.process(context);

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        mLowShelfFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        mMidPeakFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
        mHighShelfFilters[channel].processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
    }

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