#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{

	setSize(600, 400);

	using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
	using comboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

	juce::StringArray waveChoices = { "Sine", "Saw", "Square", "Triangle", "Noise" };

	attackAttachment = std::make_unique<sliderAttachment>(audioProcessor.apvts, "ATTACK", attack);
	decayAttachment = std::make_unique<sliderAttachment>(audioProcessor.apvts, "DECAY", decay);
	sustainAttachment = std::make_unique<sliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustain);
	releaseAttachment = std::make_unique<sliderAttachment>(audioProcessor.apvts, "RELEASE", release);

	gainAttachment = std::make_unique<sliderAttachment>(audioProcessor.apvts, "GAIN", gain);

	osc1ComboBox = std::make_unique<comboBoxAttachment>(audioProcessor.apvts, "OSC1", osc1WaveSelector);
	osc2ComboBox = std::make_unique<comboBoxAttachment>(audioProcessor.apvts, "OSC2", osc2WaveSelector);

	osc1WaveSelector.addItemList(waveChoices, 1);
	osc2WaveSelector.addItemList(waveChoices, 1);

	attack.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	decay.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	sustain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
	release.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);

	attack.setSliderStyle(juce::Slider::LinearVertical);
	decay.setSliderStyle(juce::Slider::LinearVertical);
	sustain.setSliderStyle(juce::Slider::LinearVertical);
	release.setSliderStyle(juce::Slider::LinearVertical);

	gain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
	gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

	attackLabel.setText("Attack", juce::dontSendNotification);
	decayLabel.setText("Decay", juce::dontSendNotification);
	sustainLabel.setText("Sustain", juce::dontSendNotification);
	releaseLabel.setText("Release", juce::dontSendNotification);

	gainLabel.setText("Gain Level", juce::dontSendNotification);

	attackLabel.attachToComponent(&attack, false);
	decayLabel.attachToComponent(&decay, false);
	sustainLabel.attachToComponent(&sustain, false);
	releaseLabel.attachToComponent(&release, false);

	gainLabel.attachToComponent(&gain, false);

	addAndMakeVisible(osc1WaveSelector);
	addAndMakeVisible(osc2WaveSelector);

	addAndMakeVisible(attack);
	addAndMakeVisible(decay);
	addAndMakeVisible(sustain);
	addAndMakeVisible(release);

	addAndMakeVisible(gain);

	addAndMakeVisible(attackLabel);
	addAndMakeVisible(decayLabel);
	addAndMakeVisible(sustainLabel);
	addAndMakeVisible(releaseLabel);

	addAndMakeVisible(gainLabel);

	osc1WaveSelector.addListener(this);
	osc2WaveSelector.addListener(this);

	attack.addListener(this);
	decay.addListener(this);
	sustain.addListener(this);
	release.addListener(this);

	gain.addListener(this);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
}
void SynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
	audioProcessor.noteAttack = attack.getValue();
}
void SynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{

}

void SynthAudioProcessorEditor::resized()
{

	attack.setBounds(getWidth() / 30, getHeight() / 20, 50, getHeight() / 1.5f);
	decay.setBounds(getWidth() / 10, getHeight() / 20, 50, getHeight() / 1.5f);
	sustain.setBounds(getWidth() / 6, getHeight() / 20, 50, getHeight() / 1.5f);
	release.setBounds(getWidth() / 4.29f, getHeight() / 20, 50, getHeight() / 1.5f);

	osc1WaveSelector.setBounds(getWidth() / 2.4f, getHeight() / 30, getWidth() / 2, getHeight() / 10);
	osc2WaveSelector.setBounds(getWidth() / 2.4f, getHeight() / 2.72f, getWidth() / 2, getHeight() / 10);

	gain.setBounds(getWidth() / 8 - 70, getHeight() / 1.25f, 200, getHeight() / 5);
}
