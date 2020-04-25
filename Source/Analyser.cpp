/*
  ==============================================================================

    Analyser.cpp
    Created: 24 Apr 2020 2:46:29pm

    Based on Analyser class from the Frequaliser by Daniel Walz
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

#include "Analyser.h"

Analyser::Analyser() : Thread("ggconvolver-analyser")
{
    averager.clear();
}

void Analyser::addAudioData(const AudioBuffer<float>& buffer, int startChannel, int numChannels)
{
    if (abstractFifo.getFreeSpace() < buffer.getNumSamples())
        return;

    int start1, block1, start2, block2;
    abstractFifo.prepareToWrite(buffer.getNumSamples(), start1, block1, start2, block2);
    audioFifo.copyFrom(0, start1, buffer.getReadPointer(startChannel), block1);
    if (block2 > 0)
        audioFifo.copyFrom(0, start2, buffer.getReadPointer(startChannel, block1), block2);

    for (int channel = startChannel + 1; channel < startChannel + numChannels; ++channel)
    {
        if (block1 > 0) audioFifo.addFrom(0, start1, buffer.getReadPointer(channel), block1);
        if (block2 > 0) audioFifo.addFrom(0, start2, buffer.getReadPointer(channel, block1), block2);
    }
    abstractFifo.finishedWrite(block1 + block2);
    waitForData.signal();
}

void Analyser::setupAnalyser(int audioFifoSize, float sampleRateToUse)
{
    sampleRate = sampleRateToUse;
    audioFifo.setSize(1, audioFifoSize);
    abstractFifo.setTotalSize(audioFifoSize);

    startThread(5);
}

void Analyser::run() 
{
    while (!threadShouldExit())
    {
        if (abstractFifo.getNumReady() >= fft.getSize())
        {
            fftBuffer.clear();

            int start1, block1, start2, block2;
            abstractFifo.prepareToRead(fft.getSize(), start1, block1, start2, block2);
            if (block1 > 0) fftBuffer.copyFrom(0, 0, audioFifo.getReadPointer(0, start1), block1);
            if (block2 > 0) fftBuffer.copyFrom(0, block1, audioFifo.getReadPointer(0, start2), block2);
            abstractFifo.finishedRead((block1 + block2) / 2);

            windowing.multiplyWithWindowingTable(fftBuffer.getWritePointer(0), size_t(fft.getSize()));
            fft.performFrequencyOnlyForwardTransform(fftBuffer.getWritePointer(0));

            ScopedLock lockedForWriting(pathCreationLock);
            averager.addFrom(0, 0, averager.getReadPointer(averagerPtr), averager.getNumSamples(), -1.0f);
            averager.copyFrom(averagerPtr, 0, fftBuffer.getReadPointer(0), averager.getNumSamples(), 1.0f / (averager.getNumSamples() * (averager.getNumChannels() - 1)));
            averager.addFrom(0, 0, averager.getReadPointer(averagerPtr), averager.getNumSamples());
            if (++averagerPtr == averager.getNumChannels()) averagerPtr = 1;

            newDataAvailable = true;
        }

        if (abstractFifo.getNumReady() < fft.getSize())
            waitForData.wait(100);
    }
}

void Analyser::createPath(Path& p, const Rectangle<float> bounds, float minFreq)
{
    p.clear();
    p.preallocateSpace(8 + averager.getNumSamples() * 3);

    ScopedLock lockedForReading(pathCreationLock);
    const auto* fftData = averager.getReadPointer(0);
    const auto  factor = bounds.getWidth() / 10.0f;

    p.startNewSubPath(bounds.getX() + factor * indexToX(0, minFreq), binToY(fftData[0], bounds));
    for (int i = 0; i < averager.getNumSamples(); ++i) {
        p.lineTo(bounds.getX() + factor * indexToX(i, minFreq),  binToY(fftData[i], bounds));
        //++i; // fix to only plot every other value
    }

}

bool Analyser::checkForNewData()
{
    auto available = newDataAvailable.load();
    newDataAvailable.store(false);
    return available;
}
