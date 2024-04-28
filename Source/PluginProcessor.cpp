/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyLowPassFilterAudioProcessor::MyLowPassFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr), lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1))
#endif
{
    juce::NormalisableRange<float> cutoffRange(20.0f, 20000.0f);
    juce::NormalisableRange<float> resRange(0.1f, 1.0f);

    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 100.0f, nullptr, nullptr);
    tree.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 0.1f, nullptr, nullptr);
}

MyLowPassFilterAudioProcessor::~MyLowPassFilterAudioProcessor()
{
}

//==============================================================================
const juce::String MyLowPassFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyLowPassFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyLowPassFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyLowPassFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyLowPassFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyLowPassFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyLowPassFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyLowPassFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyLowPassFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyLowPassFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyLowPassFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..


    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();


    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
}

void MyLowPassFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyLowPassFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void MyLowPassFilterAudioProcessor::updateFilter()
{
    float freq = *tree.getRawParameterValue("cutoff");
    float res = *tree.getRawParameterValue("resonance");

    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq, res);
}

void MyLowPassFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock <float> block(buffer);
    updateFilter();
    lowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool MyLowPassFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyLowPassFilterAudioProcessor::createEditor()
{
    return new MyLowPassFilterAudioProcessorEditor (*this);
}

//==============================================================================
void MyLowPassFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyLowPassFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyLowPassFilterAudioProcessor();
}
