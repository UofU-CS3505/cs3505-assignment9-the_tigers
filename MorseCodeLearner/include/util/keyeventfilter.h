#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include <QApplication>

/**
 * An event filter for key presses. Currently handles spacebar presses
 * but more can be added. The filter does nothing if the user is currently focused
 * on a QTextEdit object.
 *
 * @name Michael Timothy
 * @date 04/07/2025
 */
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
