#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QTextEdit>
#include <QApplication>

/**
 * An event filter for key presses.
 * Handles spacebar presses, arrowkey presses, and the enter key.
 * The filter does nothing if the user is currently focused on a QTextEdit object.
 * The filter only handles enter key presses if the user is currently focused on a QLineEdit object.
 *
 * @name Michael Timothy and Chandler Eyre
 * @date 04/16/2025
 */
class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit KeyEventFilter(QObject *parent = nullptr);

signals:
    void spacePressed();
    void spaceReleased();
    void leftArrowPressed();
    void leftArrowReleased();
    void rightArrowPressed();
    void rightArrowReleased();
    void enterPressed();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // KEYEVENTFILTER_H
