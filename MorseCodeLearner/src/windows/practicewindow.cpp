#include "practicewindow.h"
#include "ui_practicewindow.h"
#include <ctime>

PracticeWindow::PracticeWindow(QWidget *parent, KeyEventFilter *keyEventFilter, PracticeHandler *practiceHandler)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
    , keyEventFilter(keyEventFilter)
    , practiceHandler(practiceHandler)
    , timer(this)
    , world(b2Vec2(0, -20))
{
    ui->setupUi(this);

    QPixmap soundPlaying(QPixmap::fromImage(QImage(":/icons/playing_audio.png")));
    QPixmap soundNotPlaying(QPixmap::fromImage(QImage(":/icons/not_playing_audio.png")));
    ui->soundDisplayLabel->hide();
    ui->soundDisplayLabel->setPixmap(soundNotPlaying);
    ui->soundDisplayLabel->setScaledContents(true);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::onBackButtonClicked);

    ui->checkInputButton->setIcon(QIcon(":/icons/check.png"));
    ui->checkInputButton->setIconSize(QSize(52, 52));
    ui->checkInputButton->hide();

    ui->skipButton->setIcon(QIcon(":/icons/skip.png"));
    ui->skipButton->setIconSize(QSize(52, 52));
    // add slight left padding
    ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 4px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");

    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Light Indicator
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, practiceHandler, &PracticeHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, practiceHandler, &PracticeHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::enterPressed, practiceHandler, &PracticeHandler::handleEnterPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, practiceHandler, &PracticeHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, practiceHandler, &PracticeHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, practiceHandler, &PracticeHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, practiceHandler, &PracticeHandler::handleRightArrowReleased);

    QObject::connect(practiceHandler, &PracticeHandler::updateInputText, this, &PracticeWindow::updateInputText);
    QObject::connect(practiceHandler, &PracticeHandler::updatePracticeText, this, &PracticeWindow::updatePracticeText);
    QObject::connect(practiceHandler, &PracticeHandler::updateMorsePreviewText, this, [=](QString morse) {ui->morsePreview->setText(morse);});
    QObject::connect(ui->difficultySelectBox, &QComboBox::currentTextChanged, practiceHandler, &PracticeHandler::setDifficulty);
    QObject::connect(ui->difficultySelectBox, &QComboBox::currentTextChanged, this, [=]() {ui->headerLabel->setFocus();});

    QObject::connect(ui->inputText, &QLineEdit::textChanged, practiceHandler, [practiceHandler, this](){practiceHandler->receiveInputText(ui->inputText->text());});

    QObject::connect(ui->modeSelectBox, &QComboBox::currentTextChanged, this, [=](){
        ui->headerLabel->setFocus();
        QString newMode = ui->modeSelectBox->currentText();
        practiceHandler->setMode(ui->modeSelectBox->currentText());
        if (newMode == "Decode Morse" || newMode == "Decode Sound") {
            ui->checkInputButton->show();
            ui->skipButton->setFixedHeight(61);
            // remove left padding
            ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 0px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");
            ui->skipButton->move(1110, 260);
        } else {
            ui->inputText->clearFocus();
            ui->checkInputButton->hide();
            ui->skipButton->setFixedHeight(131);
            // add slight left padding
            ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 4px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");
            ui->skipButton->move(1110, 190);
        }

        if (newMode == "Decode Morse") {
            ui->modeInstructionLabel->setText("Translate the morse code into English. Check your answer with the ENTER key or the button!");
            ui->soundDisplayLabel->hide();
        } else if (newMode == "Decode Sound") {
            ui->modeInstructionLabel->setText("Translate the audible morse code into English. Check your answer with the ENTER key or the button!");
            ui->soundDisplayLabel->show();
        } else {
            ui->modeInstructionLabel->setText("Translate English by inputting morse code with the spacebar or paddles. Your answers are checked automatically!");
            ui->soundDisplayLabel->hide();
        }
    });

    // Iambic paddle illustrations
    QObject::connect(practiceHandler, &PracticeHandler::paddleSelected, this, [this, keyEventFilter](){
        QObject::disconnect(straightPressedConnection);
        QObject::disconnect(straightReleasedConnection);
        QObject::disconnect(rightPressedConnection);
        QObject::disconnect(rightReleasedConnection);
        QObject::disconnect(leftPressedConnection);
        QObject::disconnect(leftReleasedConnection);

        QPixmap paddleRight(QPixmap::fromImage(QImage(":/images/paddle_right.png")));
        QPixmap paddleLeft(QPixmap::fromImage(QImage(":/images/paddle_left.png")));
        QPixmap paddleCenter(QPixmap::fromImage(QImage(":/images/paddle_center.png")));
        QPixmap paddleBoth(QPixmap::fromImage(QImage(":/images/paddle_both.png")));
        ui->illustrationLabel->setPixmap(paddleCenter);
        rightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, [this, paddleRight, paddleBoth](){
            if (paddleState == LEFT) {
                paddleState = BOTH;
                ui->illustrationLabel->setPixmap(paddleBoth);
            } else {
                paddleState = RIGHT;
                ui->illustrationLabel->setPixmap(paddleRight);
            }
        });
        rightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, [this, paddleCenter, paddleLeft](){
            if (paddleState == BOTH) {
                paddleState = LEFT;
                ui->illustrationLabel->setPixmap(paddleLeft);
            } else {
                paddleState = CENTER;
                ui->illustrationLabel->setPixmap(paddleCenter);
            }
        });
        leftPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, [this, paddleLeft, paddleBoth](){
            if (paddleState == RIGHT) {
                paddleState = BOTH;
                ui->illustrationLabel->setPixmap(paddleBoth);
            } else {
                paddleState = LEFT;
                ui->illustrationLabel->setPixmap(paddleLeft);
            }
        });
        leftReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, [this, paddleCenter, paddleRight](){
            if (paddleState == BOTH) {
                paddleState = RIGHT;
                ui->illustrationLabel->setPixmap(paddleRight);
            } else {
                paddleState = CENTER;
                ui->illustrationLabel->setPixmap(paddleCenter);
            }
        });
    });

    // Straight key illustrations
    QObject::connect(practiceHandler, &PracticeHandler::straightKeySelected, this, [this, keyEventFilter](){
        QObject::disconnect(straightPressedConnection);
        QObject::disconnect(straightReleasedConnection);
        QObject::disconnect(rightPressedConnection);
        QObject::disconnect(rightReleasedConnection);
        QObject::disconnect(leftPressedConnection);
        QObject::disconnect(leftReleasedConnection);

        QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
        QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
        ui->illustrationLabel->setPixmap(straightKeyUp);
        straightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
        straightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});
    });

    QObject::connect(practiceHandler, &PracticeHandler::isInputReadOnly, ui->inputText, &QLineEdit::setReadOnly);

    QObject::connect(ui->checkInputButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::checkProblem);

    QObject::connect(practiceHandler, &PracticeHandler::focusInput, this, [this]() {ui->inputText->setFocus();});
    QObject::connect(practiceHandler, &PracticeHandler::updateScore, ui->scoreDisplayLabel, &QLabel::setText);

    QObject::connect(practiceHandler, &PracticeHandler::soundPlaying, this, [this, soundPlaying]() {ui->soundDisplayLabel->setPixmap(soundPlaying);});
    QObject::connect(practiceHandler, &PracticeHandler::soundNotPlaying, this, [this, soundNotPlaying]() {ui->soundDisplayLabel->setPixmap(soundNotPlaying);});

    QObject::connect(ui->skipButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::skipProblem);
    QObject::connect(practiceHandler, &PracticeHandler::updateHighScore, ui->highScoreDisplayLabel, &QLabel::setText);

    QObject::connect(practiceHandler, &PracticeHandler::correctTextJump, this, &PracticeWindow::textJump);
    QObject::connect(practiceHandler, &PracticeHandler::incorrectTextShake, this, &PracticeWindow::textShake);

    setupWorld();
}

PracticeWindow::~PracticeWindow() {
    delete ui;
}

void PracticeWindow::setUserOnThisPage(bool userOnThisPage) {
    practiceHandler->setUserOnThisPage(userOnThisPage);
}

bool PracticeWindow::getUserOnThisPage() {
    return practiceHandler->getUserOnThisPage();
}

void PracticeWindow::updateInputText(QString text) {
    ui->inputText->setText(text);
}

void PracticeWindow::updatePracticeText(QString text) {
    ui->problemText->setText(text);
}

void PracticeWindow::setupWorld(){
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2BodyDef textShakeBodyDefinition;
    textShakeBodyDefinition.type = b2_dynamicBody;
    textShakeBodyDefinition.position.Set(0.0f, 2.0f);
    textShakeBody = world.CreateBody(&textShakeBodyDefinition);

    b2BodyDef textJumpBodyDefinition;
    textJumpBodyDefinition.type = b2_dynamicBody;
    textJumpBodyDefinition.position.Set(5.0f, 2.0f);
    textJumpBody = world.CreateBody(&textJumpBodyDefinition);

    b2BodyDef shakeAnchorDefinition;
    shakeAnchorDefinition.type = b2_staticBody;
    shakeAnchorDefinition.position.Set(0.0f, 10.0f);
    shakeAnchor = world.CreateBody(&shakeAnchorDefinition);

    b2RopeJointDef ropeJointDefinition;
    ropeJointDefinition.bodyA = shakeAnchor;
    ropeJointDefinition.bodyB = textShakeBody;
    ropeJointDefinition.collideConnected = false;
    ropeJoint = (b2RopeJoint*) world.CreateJoint(&ropeJointDefinition);

    b2FixtureDef textShakeFixtureDefinition;
    textShakeFixtureDefinition.shape = &dynamicBox;
    textShakeFixtureDefinition.density = 1.0f;
    textShakeFixtureDefinition.friction = 0.3f;

    b2FixtureDef textJumpFixtureDefinition;
    textJumpFixtureDefinition.shape = &dynamicBox;
    textJumpFixtureDefinition.density = 5.0f;
    textJumpFixtureDefinition.friction = 0.3f;

    textShakeBody->CreateFixture(&textShakeFixtureDefinition);
    textJumpBody->CreateFixture(&textJumpFixtureDefinition);

    problemTextX = ui->problemText->x();
    problemTextY = ui->problemText->y();

    timer.singleShot(10, this, &PracticeWindow::updateWorld);
}

void PracticeWindow::updateWorld(){
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    ui->problemText->move(problemTextX - textShakeBody->GetPosition().x, problemTextY - textShakeBody->GetPosition().y - textJumpBody->GetPosition().y + 4);

    if(currentlyShaking){
        if(shakeFrameCount >= 30){
            currentlyShaking = false;
            textShakeBody->SetLinearVelocity(b2Vec2(0,0));
        }

        else if (shakeFrameCount % 2 == 0){
            textShakeBody->SetLinearVelocity(b2Vec2(5000,0));
        }

        else {
            textShakeBody->SetLinearVelocity(b2Vec2(-5000,0));
        }

        shakeFrameCount++;
    }

    timer.singleShot(10, this, &PracticeWindow::updateWorld);
}

void PracticeWindow::textJump(){
    textJumpBody->SetLinearVelocity(b2Vec2(0, 15));
    textJumpBody->SetAwake(true);
}

void PracticeWindow::textShake(){
    shakeFrameCount = 0;
    currentlyShaking = true;
}

