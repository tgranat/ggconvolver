/*
  ==============================================================================

   Implementation of Simple Speaker Simulator (ggconvolver)

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

    //
    mFrequencies.resize(300);
    for (size_t i = 0; i < mFrequencies.size(); ++i) {
        mFrequencies[i] = 20.0 * std::pow(2.0, i / 30.0);
    }
    mMagnitudes.resize(mFrequencies.size());
    mMidMagnitudes.resize(mFrequencies.size());
    mLowMagnitudes.resize(mFrequencies.size());
    mHighMagnitudes.resize(mFrequencies.size());
}

GgconvolverAudioProcessor::~GgconvolverAudioProcessor()
{
    outputAnalyser.stopThread(1000);
}

//==============================================================================
const String GgconvolverAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GgconvolverAudioProcessor::acceptsMidi() const
{
    return false;
}

bool GgconvolverAudioProcessor::producesMidi() const
{
    return false;
}

bool GgconvolverAudioProcessor::isMidiEffect() const
{
    return false;
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
    outputAnalyser.setupAnalyser(int(sampleRate), float(sampleRate));
}

// Called when plugin removed (not when only disabled)
// Also called during startup of plugin (at least on Reaper: first activate, then deactivate, then activate again) 
// Same as VST AudioEffect::setActive(0)
void GgconvolverAudioProcessor::releaseResources()
{
    outputAnalyser.stopThread(1000);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GgconvolverAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}
#endif

// If signal is muted, processBlock isn't called (Reaper)
void GgconvolverAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    ignoreUnused(midiMessages);

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear output channels if there are more than input channels 
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (mParamsHaveBeenUpdatedInGUI) {
        updateParams();
        mParamsHaveBeenUpdatedInGUI = false;
    }

    // Update IR if it has been changed in GUI
    if (mIrNumber != mCurrentIrLoaded) {
        updateConvolution();
        mCurrentIrLoaded = mIrNumber;
    }

    // Process impulse response
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    mConvolution.process(context);

    // Apply filters
    processFilters(&buffer, totalNumInputChannels);

    // Apply output level + compensating gain to achieve that the perceived loudness sounds good (to me)
    buffer.applyGain(mOutLevel * Constant::compensatingOutGain);
    //float postRMSLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

    if (getActiveEditor() != nullptr) {
        outputAnalyser.addAudioData(buffer, 0, getTotalNumOutputChannels());
    }
}

//==============================================================================
bool GgconvolverAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GgconvolverAudioProcessor::createEditor()
{
    return new GgconvolverAudioProcessorEditor (*this);
}

//==============================================================================
void GgconvolverAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Run updateParams() to make sure latest GUI params have been fecthed (in case of "Reset to factory")
    updateParams();
    auto state = getAPVTS().copyState(); 
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GgconvolverAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(getAPVTS().state.getType()))
            getAPVTS().replaceState(ValueTree::fromXml(*xmlState));
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
    // 0 means no limit for original impulse response size
    mConvolution.loadImpulseResponse(mIrData, mIrSize, false, false, 0, true);
}

AudioProcessorValueTreeState::ParameterLayout GgconvolverAudioProcessor::createParameters() {
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

    // How to get gain parameters handle dB nicely is inspired by https://github.com/ffAudio/Frequalizer

    const float maxGain = Decibels::decibelsToGain(Constant::maxParameterGain);
    parameters.push_back(std::make_unique<AudioParameterFloat>("LEVEL", "Level",
        NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
        std::log(0.5f) / std::log((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
        1.f,
        String(),
        AudioProcessorParameter::genericParameter,
        [](float value, int) {return String(Decibels::gainToDecibels(value), 1) + " dB"; },
        [](String text) {return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }));

    parameters.push_back(std::make_unique<AudioParameterFloat>("LOW", "Low",
        NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
        std::log(0.5f) / std::log((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
        1.f,
        String(),
        AudioProcessorParameter::genericParameter,
        [](float value, int) {return String(Decibels::gainToDecibels(value), 1) + " dB"; },
        [](String text) {return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }));

    parameters.push_back(std::make_unique<AudioParameterFloat>("MID", "Mid",
        NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
        std::log(0.5f) / std::log((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
        1.f,
        String(),
        AudioProcessorParameter::genericParameter,
        [](float value, int) {return String(Decibels::gainToDecibels(value), 1) + " dB"; },
        [](String text) {return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }));

    parameters.push_back(std::make_unique<AudioParameterFloat>("HIGH", "High",
        NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
        std::log(0.5f) / std::log((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
        1.f,
        String(),
        AudioProcessorParameter::genericParameter,
        [](float value, int) {return String(Decibels::gainToDecibels(value), 1) + " dB"; },
        [](String text) {return Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue()); }));

    parameters.push_back(std::make_unique<AudioParameterFloat>("MID FREQ", "Mid Freq",
        NormalisableRange<float> {Constant::midPeakFrequencyLow, 
                                  Constant::midPeakFrequencyHigh,
                                  1.0f, std::log(0.5f) / std::log((Constant::midPeakFrequency-Constant::midPeakFrequencyLow) / (Constant::midPeakFrequencyHigh- Constant::midPeakFrequencyLow))},
        Constant::midPeakFrequency,
        String(),
        AudioProcessorParameter::genericParameter,
        [](float value, int) { return (value < 1000) ? String(value, 0) + " Hz" : String(value / 1000.0, 2) + " kHz"; },
        [](String text) { return text.endsWith(" kHz") ? text.dropLastCharacters(4).getFloatValue() * 1000.0 : text.dropLastCharacters(3).getFloatValue(); }));

    // TODO: Make this an AudioParameterChoice instead

    parameters.push_back(std::make_unique<AudioParameterBool>("BANDWIDTH1", "BW 1 oct", false));
    parameters.push_back(std::make_unique<AudioParameterBool>("BANDWIDTH2", "BW 2 oct", true));

    // We populate the AudioParameterChoice here BUT we must also populate the ComboBox in the editor.
    // The number of choices must correspond. In the GUI, the strings from the ComboBox are displayed.
    // If no GUI is used, the strings from the AudioParameterChoice are used.
    StringArray dummyChoices;
    for (int i = 0; i < BinaryData::namedResourceListSize; i++) {
        dummyChoices.add(BinaryData::namedResourceList[i]);
    }
    parameters.push_back(std::make_unique<AudioParameterChoice>("IRCHOICE", "IR Choice", dummyChoices, 0));

    return { parameters.begin(), parameters.end() };
}

void GgconvolverAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) {
    mParamsHaveBeenUpdatedInGUI = true;
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
    
    if (mBandwidth1) {
        mMidPeakQ = Constant::oneOctaveQ;
    }
    if (mBandwidth2) {
        mMidPeakQ = Constant::twoOctavesQ;
    }
    // High shelving filter gain
    mHighShelfGain = mAPVTS.getRawParameterValue("HIGH")->load();
    // IR data id
    mIrNumber = mAPVTS.getRawParameterValue("IRCHOICE")->load();
    // Size of IR
    String irName = BinaryData::namedResourceList[mIrNumber];
    mIrData = BinaryData::getNamedResource(irName.toRawUTF8(), mIrSize);

    // Inform editor that parameters have been changed, in case the editor wants to do something
    sendChangeMessage();
}

void GgconvolverAudioProcessor::processFilters(AudioBuffer<float>* buffer, int totalNumInputChannels) {
    // Process filters
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        mLowShelfFilters[channel].processSamples(buffer->getWritePointer(channel), buffer->getNumSamples());
        mMidPeakFilters[channel].processSamples(buffer->getWritePointer(channel), buffer->getNumSamples());
        mHighShelfFilters[channel].processSamples(buffer->getWritePointer(channel), buffer->getNumSamples());
    }

    // Update filters that have been updated in gui
    // Low shelf
    if (mLowShelfGain != mCurrentLowShelfGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mLowShelfFilters[channel].setCoefficients(IIRCoefficients::makeLowShelf(getSampleRate(), Constant::lowShelfFrequency, Constant::lowShelfFilterQ, mLowShelfGain));
        }
        mCurrentLowShelfGain = mLowShelfGain;
    }
    // High shelf
    if (mHighShelfGain != mCurrentHighShelfGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mHighShelfFilters[channel].setCoefficients(IIRCoefficients::makeHighShelf(getSampleRate(), Constant::highShelfFrequency, Constant::highShelfFilterQ, mHighShelfGain));
        }
        mCurrentHighShelfGain = mHighShelfGain;
    }
    // Mid
    if (mMidPeakFrequency != mCurrentMidPeakFrequency || mMidPeakQ != mCurrentMidPeakQ || mMidPeakGain != mCurrentMidPeakGain) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            mMidPeakFilters[channel].setCoefficients(IIRCoefficients::makePeakFilter(getSampleRate(), mMidPeakFrequency, mMidPeakQ, mMidPeakGain));

        }
        mCurrentMidPeakFrequency = mMidPeakFrequency;
        mCurrentMidPeakQ = mMidPeakQ;
        mCurrentMidPeakGain = mMidPeakGain;
    }

}
const std::vector<double>& GgconvolverAudioProcessor::getMagnitudes()
{
    return mMagnitudes;
}

// Called by the editor component to update Path
void GgconvolverAudioProcessor::createFrequencyPlot(Path & p, const std::vector<double> & mags, const Rectangle<int> bounds, float pixelsPerDouble)
{
    mLowCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), Constant::lowShelfFrequency, Constant::lowShelfFilterQ, mLowShelfGain);
    mHighCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), Constant::highShelfFrequency, Constant::highShelfFilterQ, mHighShelfGain);
    mMidCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), mMidPeakFrequency, mMidPeakQ, mMidPeakGain);
    mLowCoefficients->getMagnitudeForFrequencyArray(mFrequencies.data(), mLowMagnitudes.data(), mFrequencies.size(), getSampleRate());
    mMidCoefficients->getMagnitudeForFrequencyArray(mFrequencies.data(), mMidMagnitudes.data(), mFrequencies.size(), getSampleRate());
    mHighCoefficients->getMagnitudeForFrequencyArray(mFrequencies.data(), mHighMagnitudes.data(), mFrequencies.size(), getSampleRate());
    // Set outlevel as entry magnitude
    std::fill(mMagnitudes.begin(), mMagnitudes.end(), mOutLevel);
    // Multiply the magnitudes to get a magnitude for all three filter
    FloatVectorOperations::multiply(mMagnitudes.data(), mLowMagnitudes.data(), static_cast<int> (mMidMagnitudes.size()));
    FloatVectorOperations::multiply(mMagnitudes.data(), mMidMagnitudes.data(), static_cast<int> (mMidMagnitudes.size()));
    FloatVectorOperations::multiply(mMagnitudes.data(), mHighMagnitudes.data(), static_cast<int> (mMidMagnitudes.size()));

    float yPos = float(mags[0] > 0 ? bounds.getCentreY() - pixelsPerDouble * std::log(mags[0]) / std::log(2) : bounds.getBottom());
    if (yPos < bounds.getY()) yPos = bounds.getY();
    if (yPos > bounds.getBottom()) yPos = bounds.getBottom();
    p.startNewSubPath(bounds.getX(), yPos);
    const double xFactor = static_cast<double> (bounds.getWidth()) / mFrequencies.size();

    for (size_t i = 1; i < mFrequencies.size(); ++i)
    {
        yPos = float(mags[i] > 0 ? bounds.getCentreY() - pixelsPerDouble * std::log(mags[i]) / std::log(2) : bounds.getBottom());
        if (yPos < bounds.getY()) yPos = bounds.getY();
        if (yPos > bounds.getBottom()) yPos = bounds.getBottom();
        p.lineTo(float(bounds.getX() + i * xFactor), yPos);
    }
}
void GgconvolverAudioProcessor::createAnalyserPlot(Path& p, const Rectangle<int> bounds, float minFreq)
{
    outputAnalyser.createPath(p, bounds.toFloat(), minFreq);
}

bool GgconvolverAudioProcessor::checkForNewAnalyserData()
{
    return outputAnalyser.checkForNewData();
}

