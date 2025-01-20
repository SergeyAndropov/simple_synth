#include "OSCData.h"


void oscData::setWave(const int wave)
{
	switch (wave)
	{
	case 0:
		initialise([](float x) {return std::sin(x); });
		break;
	case 1:
		initialise([](float x) {return x / juce::MathConstants<float>::pi; });
		break;
	case 2:
		initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
		break;
	case 3:
		initialise([](float x) {return asin(std::sin(x)); });
		break;
	case 4:
		initialise([](float x) {return juce::Random::getSystemRandom().nextFloat(); });
		break;
	default:
		jassertfalse;
		break;
	}
}

