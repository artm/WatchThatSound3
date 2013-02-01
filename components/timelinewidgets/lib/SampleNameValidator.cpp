#include "SampleNameValidator.hpp"

#include "Project.hpp"
#include "WaveformWidget.hpp"
#include "SoundBuffer.hpp"

using namespace WTS;

SampleNameValidator::SampleNameValidator(WaveformWidget * waveform, Project * project) :
    QValidator(waveform),
    m_project(project),
    m_waveform(waveform)
{
}

QValidator::State SampleNameValidator::validate(QString &input, int &) const
{
    if (!m_project || !m_waveform)
        return QValidator::Invalid;

    SoundBuffer * editedBuffer = m_waveform->soundBuffer();
    QString fname = SoundBuffer::makeFileName( input );

    if (!editedBuffer) {
        // if no buffer is being edited, keep it grey no matter what
        return QValidator::Acceptable;
    }

    if (fname == ".wav")
        return QValidator::Intermediate;

    foreach( WtsAudio::BufferAt * bat, m_project->getSequence() ) {
        if (bat->buffer() != editedBuffer) {
            QString otherFname = SoundBuffer::makeFileName( bat->buffer()->name() );
            if (fname == otherFname)
                return QValidator::Intermediate;
        }
    }
    return QValidator::Acceptable;

}
