/*
  ==============================================================================

    Analyser.h
    Created: 24 Apr 2020 2:46:29pm
    
    Based on Analyser class from Frequaliser by Daniel Walz
    https://github.com/ffAudio/Frequalizer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Analyser : public Thread
{
public:
    Analyser();
    virtual ~Analyser() = default;

    void addAudioData(const AudioBuffer<float>& buffer, int startChannel, int numChannels);
    void setupAnalyser(int audioFifoSize, float sampleRateToUse);
    void run() override;
    void createPath(Path& p, const Rectangle<float> bounds, float minFreq);
    bool checkForNewData();

private:
    inline float indexToX(float index, float minFreq) const
    {
        const auto freq = (sampleRate * index) / fft.getSize();
        return (freq > 0.01f) ? std::log(freq / minFreq) / std::log(2.0f) : 0.0f;
    }

    inline float binToY(float bin, const Rectangle<float> bounds) const
    {
        const float infinity = -80.0f;
        return jmap(Decibels::gainToDecibels(bin, infinity),
            infinity, 0.0f, bounds.getBottom(), bounds.getY());
    }

    WaitableEvent waitForData;
    CriticalSection pathCreationLock;

    float sampleRate{};

    dsp::FFT fft{ 12 };
    dsp::WindowingFunction<float> windowing{ size_t(fft.getSize()), dsp::WindowingFunction<float>::hann, true };
    AudioBuffer<float> fftBuffer{ 1, fft.getSize() * 2 };

    AudioBuffer<float> averager{ 5, fft.getSize() / 2 };
    int averagerPtr = 1;

    AbstractFifo abstractFifo{ 48000 };
    AudioBuffer<float> audioFifo;

    std::atomic<bool> newDataAvailable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Analyser)
};

