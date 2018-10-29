#include "audiofileplayer.h"

AudioFilePlayer::AudioFilePlayer() :
    selectedFileIndex(-1),
    m_playing(false),
    m_playhead(0)
{
    m_audioFormatManager.registerBasicFormats();

    m_comboBox.addListener(this);
    addAndMakeVisible(m_comboBox);

    m_playButton.addListener(this);
    m_playButton.setButtonText("play");
    addAndMakeVisible(m_playButton);

    m_stopButton.addListener(this);
    m_stopButton.setButtonText("stop");
    addAndMakeVisible(m_stopButton);
}

AudioFilePlayer::~AudioFilePlayer()
{
    
}

void AudioFilePlayer::addFiles(juce::Array<File> files)
{
    for(auto file : files)
    {
        addFile(file);
    }
}

void AudioFilePlayer::addFile(File file)
{
    m_files.add(file);
    juce::AudioFormatReader* audioFormatReader = m_audioFormatManager.createReaderFor(file);
    AudioSampleBuffer audioSampleBuffer ((int) audioFormatReader->numChannels,
                                         (int) audioFormatReader->lengthInSamples);
    audioFormatReader->read(&audioSampleBuffer,
                            0,
                            audioFormatReader->lengthInSamples,
                            0,
                            true,
                            true);
    m_buffers.push_back(audioSampleBuffer);
    m_comboBox.addItem(file.getFileName(), m_comboBox.getNumItems() + 1);
}

void AudioFilePlayer::output(AudioSampleBuffer *audioSampleBuffer)
{
    if(!m_playing || selectedFileIndex < 0)
    {
        return;
    }

    int bufferSize = audioSampleBuffer->getNumSamples();

    AudioSampleBuffer* selectedBuffer = &(m_buffers[selectedFileIndex]);

    if(m_playhead + bufferSize < selectedBuffer->getNumSamples())
    {
        for(int channel = 0; channel < selectedBuffer->getNumChannels(); channel++)
        {
            audioSampleBuffer->addFrom(channel, 0, *selectedBuffer, channel, m_playhead, bufferSize);
        }
        m_playhead += bufferSize;
    }
    else
    {
        for(int sample = 0; sample < bufferSize; sample++)
        {
            for(int channel = 0; channel < selectedBuffer->getNumChannels() && channel < audioSampleBuffer->getNumChannels(); channel++)
            {
                audioSampleBuffer->addSample(channel, sample, selectedBuffer->getSample(channel, m_playhead));
            }
            m_playhead++;
            m_playhead %= selectedBuffer->getNumSamples();
        }
    }
}

void AudioFilePlayer::resized()
{
    auto localBounds = getLocalBounds();
    localBounds.reduce(10, 10);

    int yDivision = localBounds.getHeight() / 3.0f;

    auto comboBoxRect = localBounds.removeFromTop(yDivision * 1);
    m_comboBox.setBounds(comboBoxRect);

    auto playButtonRect = localBounds.removeFromTop(yDivision * 1);
    m_playButton.setBounds(playButtonRect);

    auto stopButtonRect = localBounds.removeFromTop(yDivision * 1);
    m_stopButton.setBounds(stopButtonRect);
}

void AudioFilePlayer::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    m_playing = false;
    selectedFileIndex = comboBoxThatHasChanged->getSelectedItemIndex();
}

void AudioFilePlayer::buttonClicked(Button *button)
{
    if(button == &m_playButton)
    {
        m_playhead = 0;
        m_playing = true;
    }
    else if (button == &m_stopButton)
    {
        m_playing = false;
    }
}
