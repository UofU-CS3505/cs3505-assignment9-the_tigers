#include "keyeventfilter.h"

KeyEventFilter::KeyEventFilter(QObject *parent) : QObject(parent) {}

bool KeyEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QWidget *focusWidget = QApplication::focusWidget();
        QTextEdit *textEdit = qobject_cast<QTextEdit *>(focusWidget);
        if (textEdit) {
            return false;
        }

        if (keyEvent->key() == Qt::Key_Space && !keyEvent->isAutoRepeat()) {
            if (event->type() == QEvent::KeyPress) {
                emit spacePressed();
            } else {
                emit spaceReleased();
            }
            return true;
        }

        if (keyEvent->key() == Qt::Key_Left && !keyEvent->isAutoRepeat()) {
            if (event->type() == QEvent::KeyPress) {
                emit leftArrowPressed();
            } else {
                emit leftArrowReleased();
            }
            return true;
        }

        if (keyEvent->key() == Qt::Key_Right && !keyEvent->isAutoRepeat()) {
            if (event->type() == QEvent::KeyPress) {
                emit rightArrowPressed();
            } else {
                emit rightArrowReleased();
            }
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}
