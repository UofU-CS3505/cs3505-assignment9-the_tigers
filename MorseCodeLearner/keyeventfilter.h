#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include <QApplication>

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit KeyEventFilter(QObject *parent = nullptr);

signals:
    void spacePressed();
    void spaceReleased();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // KEYEVENTFILTER_H
