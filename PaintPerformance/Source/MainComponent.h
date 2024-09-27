#pragma once

#include <JuceHeader.h>

static constexpr int cellColour = 0x10000001;

class Stack : public juce::Component
{
public:
	Stack (juce::Component* child_)
	{
		child.reset (child_);
		addAndMakeVisible (*child);
	}

	void resized() override
	{
		child->setBounds (getLocalBounds());
	}

	std::unique_ptr<juce::Component> child;
};

class Draw : public juce::Component
{
public:
	void paint(juce::Graphics& g) override
	{
		g.setColour (findColour(cellColour, true).brighter (adjust));
		//g.setColour (juce::Colours::red.brighter (adjust));

		g.fillAll();
	}

	const float adjust { juce::Random::getSystemRandom().nextFloat () / 10.0f };
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
					   public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
	void paintOverChildren (juce::Graphics&) override;

    void resized() override;
	void timerCallback() override;

private:
    //==============================================================================
	juce::OwnedArray<Stack> stacks;

	int count = 0;
	float fps = 0;
	juce::Time last { juce::Time::getCurrentTime() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
