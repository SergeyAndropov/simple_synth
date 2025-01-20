#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	osc1.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	osc2.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	synthADSR.noteOn();

}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	synthADSR.noteOff();

	if (!allowTailOff || !synthADSR.isActive())
	{
		clearCurrentNote();
	}
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	synthADSR.setSampleRate(sampleRate);

	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc1.prepare(spec);
	osc2.prepare(spec);
	gain.prepare(spec);


	isPrepared = true;
}
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
	synthADSRParams.attack = attack;
	synthADSRParams.decay = decay;
	synthADSRParams.sustain = sustain;
	synthADSRParams.release = release;
	synthADSR.setParameters(synthADSRParams);
}

void SynthVoice::updateGain(const float gainAmount)
{
	gain.setGainLinear(gainAmount);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);

	if (!isVoiceActive())
	{
		return;
	}

	synthBuffer.setSize(outputBuffer.getNumChannels(), outputBuffer.getNumSamples(), false, false, true);
	synthBuffer.clear();

	juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

	osc1.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	osc2.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
	gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

	synthADSR.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());



	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

		if (!synthADSR.isActive())
		{
			clearCurrentNote();
		}
	}
}
