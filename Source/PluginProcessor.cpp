#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParameters())
#endif

{
	synth.addSound(new SynthSound());
	synth.addVoice(new SynthVoice());
}

SynthAudioProcessor::~SynthAudioProcessor()
{
}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
	return 1;
}

int SynthAudioProcessor::getCurrentProgram()
{
	return 0;
}

void SynthAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SynthAudioProcessor::getProgramName(int index)
{
	return {};
}

void SynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
		}
	}
}

void SynthAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
	}

	for (int i = 0; i < synth.getNumVoices(); ++i)
	{
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			auto& attack = *apvts.getRawParameterValue("ATTACK");
			auto& decay = *apvts.getRawParameterValue("DECAY");
			auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
			auto& release = *apvts.getRawParameterValue("RELEASE");

			auto& osc1Wave = *apvts.getRawParameterValue("OSC1");
			auto& osc2Wave = *apvts.getRawParameterValue("OSC2");

			auto& gainAmount = *apvts.getRawParameterValue("GAIN");

			voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());

			voice->getOscillator1().setWave(osc1Wave);
			voice->getOscillator2().setWave(osc2Wave);

			voice->updateGain(gainAmount.load());
		}
	}



	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
	return true;
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
	return new SynthAudioProcessorEditor(*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = apvts.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void SynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr <juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(apvts.state.getType()))
		{
			apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
		}
	}
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthAudioProcessor::createParameters()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1", "OSC 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle","Noise" }, 0));
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2", "OSC 2 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle","Noise" }, 0));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.01f, 4.0f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.0f, 1.0f}, 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{0.0f, 6.0f}, 0.5f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>{0.01f, 0.4f}, 0.2f));

	return { params.begin(),params.end() };
}
