#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
class SynthAudioProcessorEditor : public juce::AudioProcessorEditor,
	private juce::Slider::Listener,
	private juce::ComboBox::Listener
{
public:
	SynthAudioProcessorEditor(SynthAudioProcessor&);
	~SynthAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void sliderValueChanged(juce::Slider* slider) override;
	void comboBoxChanged(juce::ComboBox* comboBox) override;


private:

	SynthAudioProcessor& audioProcessor;

	juce::Slider attack;
	juce::Slider decay;
	juce::Slider sustain;
	juce::Slider release;

	juce::Slider gain;

	juce::ComboBox osc1WaveSelector;
	juce::ComboBox osc2WaveSelector;


	using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	std::unique_ptr<sliderAttachment> attackAttachment;
	std::unique_ptr<sliderAttachment> decayAttachment;
	std::unique_ptr<sliderAttachment> sustainAttachment;
	std::unique_ptr<sliderAttachment> releaseAttachment;

	std::unique_ptr<sliderAttachment> gainAttachment;

	using comboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

	std::unique_ptr<comboBoxAttachment> osc1ComboBox;
	std::unique_ptr<comboBoxAttachment> osc2ComboBox;

	juce::Label attackLabel;
	juce::Label decayLabel;
	juce::Label sustainLabel;
	juce::Label releaseLabel;

	juce::Label gainLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor);
};
