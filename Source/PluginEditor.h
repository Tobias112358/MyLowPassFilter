/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyLowPassFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MyLowPassFilterAudioProcessorEditor (MyLowPassFilterAudioProcessor&);
    ~MyLowPassFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider filterCutoffDial;
    juce::Slider filterResDial;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> filterResValue;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyLowPassFilterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyLowPassFilterAudioProcessorEditor)
};
