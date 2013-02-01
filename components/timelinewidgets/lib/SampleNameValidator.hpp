#ifndef SAMPLENAMEVALIDATOR_HPP
#define SAMPLENAMEVALIDATOR_HPP

#include "stable.h"

namespace WTS {

class Project;
class WaveformWidget;

class SampleNameValidator : public QValidator
{
    Q_OBJECT
public:
    explicit SampleNameValidator(WaveformWidget * waveform, Project * project);

protected:
    State validate (QString & input, int &) const;

    Project * m_project;
    WaveformWidget * m_waveform;
};

}

#endif // SAMPLENAMEVALIDATOR_HPP
