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
    }

    return QObject::eventFilter(obj, event);
}
