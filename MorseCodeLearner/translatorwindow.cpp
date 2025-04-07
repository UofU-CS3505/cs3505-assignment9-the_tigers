#include "translatorwindow.h"
#include "ui_translatorwindow.h"
#include <QDebug>

translatorwindow::translatorwindow(QWidget *parent, MorseHandler *morseHandler)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
    , morseHandler(morseHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);

    ui->swapButton->setIcon(QIcon(":/icons/swap.svg"));
    ui->swapButton->setIconSize(QSize(52, 104));

    mode = MORSE_TO_TEXT;

    qApp->installEventFilter(this);

    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    format.setChannelConfig(QAudioFormat::ChannelConfigMono);

    audio = new QAudioSink(format, this);
    sineGenerator = new SineWaveGenerator(this);

    QObject::connect(audio, &QAudioSink::stateChanged, this, &translatorwindow::onAudioStateChanged);
}

translatorwindow::~translatorwindow()
{
    delete ui;
    delete audio;
    delete sineGenerator;
}

void translatorwindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool translatorwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void translatorwindow::setupMorse(MorseHandler *handler) {
    morseHandler = handler;
    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &translatorwindow::onMorseReceived);
}

void translatorwindow::onBackButtonClicked() {
    morseAudioOutputBuffer = "";
    morseAudioPlayback = false;
    audio->suspend();
    ui->inputText->clear();
    ui->outputText->clear();
    emit goHome();
    userOnThisPage = false;
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
    if (userOnThisPage == false || morseAudioPlayback || mode == TEXT_TO_MORSE)
        return false;

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        QWidget *focusWidget = QApplication::focusWidget();
        if (qobject_cast<QTextEdit *>(focusWidget)) {
            return false;
        }

        if (keyEvent->key() == Qt::Key_Space && !keyEvent->isAutoRepeat()) {
            if (event->type() == QEvent::KeyPress) {
                // check if straight key is the current selected device
                if (sineGenerator->bytesAvailable() == 0) {
                    sineGenerator->start(440, 20000);
                    audio->start(sineGenerator);
                } else {
                    audio->resume();
                }
                morseHandler->straightKeyDown();
            } else {
                morseHandler->straightKeyUp();
                audio->suspend();
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

void translatorwindow::onAudioStateChanged() {
    if (audio->state() == QAudio::IdleState) {
        sineGenerator->start(440, 20000);
        audio->start(sineGenerator);
    }
}

void translatorwindow::playMorseAudio() {
    if (morseAudioOutputBuffer.empty()) {
        morseAudioPlayback = false;
        ui->audioPlayButton->setText("Play");
        return;
    }

    if (morseAudioPlayback == false)
        return;

    float unit = morseHandler->getUnitTime();

    if (morseAudioOutputBuffer.at(0) == '-') {
        if (sineGenerator->bytesAvailable() == 0) {
            sineGenerator->start(440, 20000);
            audio->start(sineGenerator);
        } else {
            audio->resume();
        }
        stopTimer.singleShot(unit * 3, [this]() {audio->suspend();});
        gapTimer.singleShot(unit * 4, this, &translatorwindow::playMorseAudio);
    } else if (morseAudioOutputBuffer.at(0) == '.') {
        if (sineGenerator->bytesAvailable() == 0) {
            sineGenerator->start(440, 20000);
            audio->start(sineGenerator);
        } else {
            audio->resume();
        }
        stopTimer.singleShot(unit, [this]() {audio->suspend();});
        gapTimer.singleShot(unit * 2, this, &translatorwindow::playMorseAudio);
    } else if (morseAudioOutputBuffer.at(0) == ' ' || morseAudioOutputBuffer.at(0) == '/') {
        gapTimer.singleShot(unit * 2, this, &translatorwindow::playMorseAudio); // A slash is between 2 spaces so 1(char gap) + 2(space gap) + 2(slash gap) + 2(space gap) = 7 units
    } else {
        morseAudioOutputBuffer = morseAudioOutputBuffer.substr(1);
        playMorseAudio();
        return;
    }
    morseAudioOutputBuffer = morseAudioOutputBuffer.substr(1);
}

void translatorwindow::on_audioPlayButton_clicked()
{
    if (morseAudioPlayback == false) {
        audio->suspend();
        ui->audioPlayButton->setText("Pause");
        morseAudioPlayback = true;
        if (mode == TEXT_TO_MORSE) {
            morseAudioOutputBuffer = ui->outputText->toPlainText().toStdString();
        } else {
            morseAudioOutputBuffer = ui->inputText->toPlainText().toStdString();
        }
        playMorseAudio();
    } else {
        ui->audioPlayButton->setText("Play");
        morseAudioPlayback = false;
        morseAudioOutputBuffer = "";
        audio->suspend();
    }
}

