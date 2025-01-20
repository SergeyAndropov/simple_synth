#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "OSCData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void updateADSR(const float attack, const float decay, const float sustain, const float release);
	void updateGain(const float gainAmount);
	void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
	
	oscData& getOscillator1() { return osc1; }
	oscData& getOscillator2() { return osc2; }

private:
	juce::ADSR synthADSR;
	juce::ADSR::Parameters synthADSRParams;
	juce::AudioBuffer<float> synthBuffer;

	oscData osc1;
	oscData osc2;	
	
	juce::dsp::Gain<float> gain;

	bool isPrepared { false };
};
