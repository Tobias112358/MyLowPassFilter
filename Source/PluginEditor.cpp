/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyLowPassFilterAudioProcessorEditor::MyLowPassFilterAudioProcessorEditor (MyLowPassFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(200, 200);

    filterCutoffDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);

    filterResDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterResDial.setRange(0.1f, 1.0f);
    filterResDial.setValue(2.0f);
    filterResDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterResDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResDial);

    //filterCutoffValue = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "cutoff", filterCutoffDial);
    //filterResValue = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "resonance", filterResDial);
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f);
}

MyLowPassFilterAudioProcessorEditor::~MyLowPassFilterAudioProcessorEditor()
{
}

//==============================================================================
void MyLowPassFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    //background ui stuff
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Filter", titleArea, juce::Justification::centredTop);
    g.drawText("Cutoff", 46, 70, 50, 25, juce::Justification::centredLeft);
    g.drawText("Resonance", 107, 70, 70, 25, juce::Justification::centredLeft);

    juce::Rectangle <float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void MyLowPassFilterAudioProcessorEditor::resized()
{
    //need to come back and dynamically set these...ok for now
    juce::Rectangle<int> area = getLocalBounds().reduced(40);

    filterCutoffDial.setBounds(30, 90, 70, 70);
    filterResDial.setBounds(100, 90, 70, 70);
}
