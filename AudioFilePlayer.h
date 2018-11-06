#ifndef AUDIOFILEPLAYER_H
#define AUDIOFILEPLAYER_H

#include "JuceHeader.h"

class AudioFilePlayer : public Component,
        public ComboBox::Listener,
        public Button::Listener
{

public:
    AudioFilePlayer();
    
    ~AudioFilePlayer();

    void addFiles(juce::Array<juce::File> files);

    void addFile(juce::File file);

    void output(AudioSampleBuffer* audioSampleBuffer);

    void resized() override;

    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;

    void buttonClicked(Button *button) override;
    
    const std::vector<AudioSampleBuffer>& getBuffers() const;

private:
    AudioFormatManager m_audioFormatManager;
    juce::Array<File> m_files;
    std::vector<AudioSampleBuffer> m_buffers;
    ComboBox m_comboBox;

    TextButton m_playButton;
    TextButton m_stopButton;

    int selectedFileIndex;
    bool m_playing;
    int m_playhead;
};

#endif // AUDIOFILEPLAYER_H
