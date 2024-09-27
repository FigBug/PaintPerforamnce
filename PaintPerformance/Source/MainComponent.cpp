#include "MainComponent.h"

constexpr int gridSize = 40;

double usedTime = 0.0;

//==============================================================================
MainComponent::MainComponent()
{
	for (int i = 0; i < gridSize * gridSize; i++)
	{
		auto draw = new Draw();
		juce::Component* last = draw;

		for (int i = 0; i < 20; i++)
			last = new Stack (last);

		stacks.add (dynamic_cast<Stack*>(last));
		addAndMakeVisible (last);
	}
	startTimer (1);

	timerCallback();
	setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::paintOverChildren (juce::Graphics& g)
{
	auto now = juce::Time::getCurrentTime();
	auto diff = now - last;

	if (diff >= juce::RelativeTime::seconds (1.0))
	{
		fps = count / diff.inSeconds();
		last = now;
		count = 0;

		used = usedTime / diff.inSeconds();
		usedTime = 0;
	}

	g.setColour (juce::Colours::black);
	g.setFont (juce::Font (juce::FontOptions (25.0f)));
	g.drawText (juce::String::formatted ("%0.1f fps %0.1f%%", fps, used * 100), getLocalBounds(), juce::Justification::centred);

	count++;
}

void MainComponent::resized()
{
	float w = getWidth() / float (gridSize);
	float h = getHeight() / float (gridSize);

	for (int i = 0; i < gridSize * gridSize; i++)
	{
		auto x1 = int ((i % gridSize) * w);
		auto x2 = int (((i % gridSize) + 1) * w);
		auto y1 = int ((i / gridSize) * h);
		auto y2 = int (((i + 1 / gridSize) + 1) * h);

		stacks[i]->setBounds (x1, y1, x2 - x1, y2 - y1);
	}
}

juce::Colour makeRandomColor()
{
	auto& random = juce::Random::getSystemRandom();

	auto red   = (uint8_t) (random.nextInt (10) + 200);
	auto green = (uint8_t) (random.nextInt (10) + 20);
	auto blue  = (uint8_t) (random.nextInt (10) + 40);
	auto alpha = (uint8_t) 255;
	return juce::Colour { red, green, blue, alpha };
}

void MainComponent::timerCallback()
{
	setColour (cellColour, makeRandomColor());

	repaint();
}
