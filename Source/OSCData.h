#pragma once

#include <JuceHeader.h>
class oscData : public juce::dsp::Oscillator<float>
{
public:
	void setWave(const int wave);

private:

};
