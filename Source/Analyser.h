/*
  ==============================================================================

    Analyser.h
    Created: 24 Apr 2020 2:46:29pm
    
    Based on Analyser class from Frequaliser by Daniel Walz
    https://github.com/ffAudio/Frequalizer

    Copyright (c) 2018, Daniel Walz - Foleys Finest Audio UG All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors
    may be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
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

