/*
    Andes - synthesiser plugin based on Perlin noise
    Copyright (C) 2017  Artem Popov <art@artfwo.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Noise.h"

class DumbLFO
{
public:
    DumbLFO() : phase(0.0f), phaseDelta(0.0f) {}

    void setFreqHz(double freqHz, double sampleRateHz) { phaseDelta = freqHz / sampleRateHz; }

    float getSineSample()
    {
        phase += phaseDelta;
        while (phase > 1.0f) phase -= 1.0f;
        return 0.5f + 0.5f * sinf(phase);
    }

    float getSawSample()
    {
        phase += phaseDelta;
        while (phase > 1.0f) phase -= 1.0f;
        return phase;
    }

    float getSeeSawSample()
    {
        phase += phaseDelta;
        while (phase > 1.0f) phase -= 1.0f;
        if (phase < 0.5f)
            return 2.0f * phase;
        else
            return 2.0f * (1.0f - phase);
    }

protected:
    float phase, phaseDelta;
};

//==============================================================================
/**
*/
class AndesAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AndesAudioProcessor();
    ~AndesAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    Noise noise;

    AudioProcessorValueTreeState parameters;
    ValueTree internalParameters;

private:
    //==============================================================================
    Synthesiser synth;
    DumbLFO lfo1, lfo2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AndesAudioProcessor)
};
