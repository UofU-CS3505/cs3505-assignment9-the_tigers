// SineWaveDisplay.h
#pragma once

#include <QWidget>
#include <QVector>

class SineWaveDisplay : public QWidget {
    Q_OBJECT

public:
    explicit SineWaveDisplay(QWidget* parent = nullptr);
    void updateWaveform(const QVector<qint16>& samples);  // for 16-bit audio

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<qint16> m_samples;
};
