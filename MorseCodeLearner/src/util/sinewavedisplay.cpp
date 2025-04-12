    // SineWaveDisplay.cpp
#include "SineWaveDisplay.h"
#include <QPainter>

SineWaveDisplay::SineWaveDisplay(QWidget* parent)
    : QWidget(parent) {}

void SineWaveDisplay::updateWaveform(const QVector<qint16>& samples) {
    m_samples = samples;
    update(); // triggers paintEvent
}

void SineWaveDisplay::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::green);

    int w = width();
    int h = height();
    int centerY = h / 2;

    if (m_samples.isEmpty())
        return;

    int step = qMax(1, m_samples.size() / w);

    for (int x = 0; x < w - 1; ++x) {
        int idx1 = x * step;
        int idx2 = (x + 1) * step;
        if (idx2 >= m_samples.size()) break;

        int y1 = centerY - (m_samples[idx1] * centerY) / 32768;
        int y2 = centerY - (m_samples[idx2] * centerY) / 32768;
        painter.drawLine(x, y1, x + 1, y2);
    }
}
