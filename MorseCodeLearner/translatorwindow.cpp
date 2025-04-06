#include "translatorwindow.h"
#include "ui_translatorwindow.h"
#include <QDebug>

translatorwindow::translatorwindow(QWidget *parent, MorseHandler *morseHandler)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
    , morseHandler(morseHandler)
{
    ui->setupUi(this);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);

    mode = MORSE_TO_TEXT;

    qApp->installEventFilter(this);
}

translatorwindow::~translatorwindow()
{
    delete ui;
}

void translatorwindow::setupMorse(MorseHandler *handler) {
    morseHandler = handler;
    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &translatorwindow::onMorseReceived);
}

void translatorwindow::onBackButtonClicked() {
    emit goHome();
}

void translatorwindow::on_swapButton_clicked()
{
    morseHandler->stopTimers();

    if (mode == MORSE_TO_TEXT) {
        mode = TEXT_TO_MORSE;
        ui->inputLabel->setText("Input: Text");
        ui->outputLabel->setText("Output: Morse");

    } else {
        mode = MORSE_TO_TEXT;
        ui->inputLabel->setText("Input: Morse");
        ui->outputLabel->setText("Output: Text");
    }

    ui->inputText->setText(ui->outputText->toPlainText());
}


void translatorwindow::on_inputText_textChanged()
{
    std::string text = ui->inputText->toPlainText().toStdString();
    if (mode == TEXT_TO_MORSE) {
        text = morseHandler->encodeText(text);
    } else {
        text = morseHandler->decodeMorse(text);
    }
    ui->outputText->setText(QString::fromStdString(text));
}

bool translatorwindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QWidget *focusWidget = QApplication::focusWidget();
        if (qobject_cast<QTextEdit *>(focusWidget)) {
            return false;
        }

        if (keyEvent->key() == Qt::Key_Space && !keyEvent->isAutoRepeat()) {
            if (event->type() == QEvent::KeyPress) {
                morseHandler->straightKeyDown();
            } else {
                morseHandler->straightKeyUp();
            }
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void translatorwindow::onMorseReceived(const string morse) {
    QString qmorse = QString::fromStdString(morse);
    ui->inputText->blockSignals(true);
    ui->inputText->setText(ui->inputText->toPlainText() + qmorse);
    ui->inputText->blockSignals(false);

    if (morse == " ") {
        on_inputText_textChanged();
    }
}

