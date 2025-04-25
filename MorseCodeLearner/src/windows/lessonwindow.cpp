#include "lessonwindow.h"
#include "ui_lessonwindow.h"

LessonWindow::LessonWindow(LessonHandler *lessonHandler, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
    , lessonHandler(lessonHandler)
    , timer(this)
    , world(b2Vec2(0, -20))
{
    ui->setupUi(this);

    QPixmap soundNotPlaying(QPixmap::fromImage(QImage(":/icons/not_playing_audio.png")));
    ui->soundDisplayLabel->hide();
    ui->soundDisplayLabel->setPixmap(soundNotPlaying);
    ui->soundDisplayLabel->setScaledContents(true);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));

    ui->previousSlideButton->setIcon(QIcon(":/icons/back.png"));
    ui->previousSlideButton->setIconSize(QSize(52, 52));
    ui->previousSlideButton->setVisible(false);

    ui->nextSlideButton->setIcon(QIcon(QPixmap(":/icons/back.png").transformed(QTransform().scale(-1, 1)))); // Reverses the arrow icon
    ui->nextSlideButton->setIconSize(QSize(52, 52));

    ui->progressDisplayLabel->setText("0%");

    // Light indicator
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Buttons
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &LessonWindow::onBackButtonClicked);
    QObject::connect(ui->nextSlideButton, &QPushButton::clicked, this, &LessonWindow::onNextSlideClicked);
    QObject::connect(ui->previousSlideButton, &QPushButton::clicked, this, &LessonWindow::onPreviousSlideClicked);
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &LessonWindow::onStackedWidgetIndexChange);

    // Lesson Handler
    QObject::connect(lessonHandler, &LessonHandler::setLightIndicator, this, &LessonWindow::setLightIndicator);
    QObject::connect(lessonHandler, &LessonHandler::guessCorrect, this, &LessonWindow::guessCorrect);
    QObject::connect(lessonHandler, &LessonHandler::guessIncorrect, this, &LessonWindow::guessIncorrect);
    QObject::connect(lessonHandler, &LessonHandler::displayTextToUI, this, &LessonWindow::displayTextQuestion);
    QObject::connect(lessonHandler, &LessonHandler::updateInputText, this, &LessonWindow::updateInputText);
    QObject::connect(lessonHandler, &LessonHandler::completedLesson, this, &LessonWindow::onBackButtonClicked);
    QObject::connect(lessonHandler, &LessonHandler::updateLessonTitle, this, &LessonWindow::updateLessonTitle);
    QObject::connect(lessonHandler, &LessonHandler::updateLessonProgressBar, this, &LessonWindow::updateLessonProgressBar);
    QObject::connect(lessonHandler, &LessonHandler::displayCorrectAnswer, this, &LessonWindow::displayCorrectAnswer);
    QObject::connect(lessonHandler, &LessonHandler::setReferenceText, this, &LessonWindow::setReferenceText);
    QObject::connect(lessonHandler, &LessonHandler::isInputReadOnly, this, &LessonWindow::setInputReadOnly);
    QObject::connect(lessonHandler, &LessonHandler::soundPlaying, this, &LessonWindow::setSoundPlaying);
    QObject::connect(lessonHandler, &LessonHandler::soundNotPlaying, this, &LessonWindow::setSoundNotPlaying);
    QObject::connect(lessonHandler, &LessonHandler::isAudioDecodeMode, this, &LessonWindow::setAudioDecodeMode);
    QObject::connect(lessonHandler, &LessonHandler::focusInput, this, &LessonWindow::setFocusInput);
    QObject::connect(lessonHandler, &LessonHandler::getInputTextSignal, this, &LessonWindow::sendInputText);

    // Lesson Window
    QObject::connect(this, &LessonWindow::setCurrentIndex, lessonHandler, &LessonHandler::setCurrentIndex);
    QObject::connect(this, &LessonWindow::backButtonClicked, lessonHandler, &LessonHandler::onBackButtonClicked);
    QObject::connect(this, &LessonWindow::sendInputTextSignal, lessonHandler, &LessonHandler::getInputText);

    // Iambic paddle illustrations
    QObject::connect(lessonHandler, &LessonHandler::paddleSelected, this, &LessonWindow::paddleSelected);

    // Straight Key Illustrations
    QObject::connect(lessonHandler, &LessonHandler::straightKeySelected, this, &LessonWindow::straightKeySelected);

    setupWorld();
    currentIndex = 0;
}

LessonWindow::~LessonWindow()
{
    delete ui;
}

void LessonWindow::straightKeySelected() {
    QObject::disconnect(straightPressedConnection);
    QObject::disconnect(straightReleasedConnection);
    QObject::disconnect(rightPressedConnection);
    QObject::disconnect(rightReleasedConnection);
    QObject::disconnect(leftPressedConnection);
    QObject::disconnect(leftReleasedConnection);

    QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
    QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
    ui->illustrationLabel->setPixmap(straightKeyUp);
    straightPressedConnection = QObject::connect(lessonHandler, &LessonHandler::straightKeyPressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
    straightReleasedConnection = QObject::connect(lessonHandler, &LessonHandler::straightKeyReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});
}

void LessonWindow::paddleSelected() {
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
    rightPressedConnection = QObject::connect(lessonHandler, &LessonHandler::paddleRightPressed, this, [this, paddleRight, paddleBoth](){
        if (paddleState == LEFT) {
            paddleState = BOTH;
            ui->illustrationLabel->setPixmap(paddleBoth);
        } else {
            paddleState = RIGHT;
            ui->illustrationLabel->setPixmap(paddleRight);
        }
    });
    rightReleasedConnection = QObject::connect(lessonHandler, &LessonHandler::paddleRightReleased, this, [this, paddleCenter, paddleLeft](){
        if (paddleState == BOTH) {
            paddleState = LEFT;
            ui->illustrationLabel->setPixmap(paddleLeft);
        } else {
            paddleState = CENTER;
            ui->illustrationLabel->setPixmap(paddleCenter);
        }
    });
    leftPressedConnection = QObject::connect(lessonHandler, &LessonHandler::paddleLeftPressed, this, [this, paddleLeft, paddleBoth](){
        if (paddleState == RIGHT) {
            paddleState = BOTH;
            ui->illustrationLabel->setPixmap(paddleBoth);
        } else {
            paddleState = LEFT;
            ui->illustrationLabel->setPixmap(paddleLeft);
        }
    });
    leftReleasedConnection = QObject::connect(lessonHandler, &LessonHandler::paddleLeftReleased, this, [this, paddleCenter, paddleRight](){
        if (paddleState == BOTH) {
            paddleState = RIGHT;
            ui->illustrationLabel->setPixmap(paddleRight);
        } else {
            paddleState = CENTER;
            ui->illustrationLabel->setPixmap(paddleCenter);
        }
    });
}

void LessonWindow::onBackButtonClicked() {
    emit backButtonClicked();
    emit goToLessonSelect();
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    emit setCurrentIndex(currentIndex);
}

void LessonWindow::guessCorrect() {
    ui->soundDisplayLabel->hide();
    ui->problemText->setText("Correct!");

    textJumpBody->SetLinearVelocity(b2Vec2(0, 15));
    textJumpBody->SetAwake(true);
}

void LessonWindow::guessIncorrect() {
    ui->soundDisplayLabel->hide();
    ui->problemText->setText("Incorrect!");

    shakeFrameCount = 0;
    currentlyShaking = true;
}

void LessonWindow::displayTextQuestion(QString text) {
    ui->inputText->setText("");
    ui->problemText->setText(text);
}

void LessonWindow::updateInputText(QString inputText) {
    ui->inputText->setText(inputText);
}

void LessonWindow::updateLessonTitle(int lessonNumber) {
    ui->headerLabel->setText("Lesson " + QString::fromStdString(std::to_string(lessonNumber)));
}

void LessonWindow::updateLessonProgressBar(float progress) {
    ui->progressBar->setValue((int)progress);
    QString progressString = QString::number((int)progress) + "%";
    ui->progressDisplayLabel->setText(progressString);
}

void LessonWindow::displayCorrectAnswer(QString correctAnswer) {
    ui->problemText->setText("The correct answer was: " + correctAnswer);
}

void LessonWindow::setReferenceText(QString referenceText) {
    ui->referenceLabel->setText(referenceText);
}

void LessonWindow::onNextSlideClicked() {
    if (currentIndex != 1) {
        currentIndex++;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void LessonWindow::onPreviousSlideClicked() {
    if (currentIndex != 0) {
        currentIndex--;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void LessonWindow::onStackedWidgetIndexChange(int index) {
    emit setCurrentIndex(index);

    if (index == 0) {
        ui->previousSlideButton->setVisible(false);
        ui->nextSlideButton->setVisible(true);
    } else if (index == 1) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(false);
    }
}

void LessonWindow::setupWorld() {
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

    timer.singleShot(10, this, &LessonWindow::updateWorld);
}

void LessonWindow::updateWorld() {
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

    timer.singleShot(10, this, &LessonWindow::updateWorld);
}

void LessonWindow::setAudioDecodeMode(bool isAudio) {
    if (isAudio) {
        ui->soundDisplayLabel->show();
    } else {
        ui->soundDisplayLabel->hide();
    }
}

void LessonWindow::startLesson(int lessonNumber) {
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void LessonWindow::setLightIndicator(bool lightIndicator) {
    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));

    if (lightIndicator) {
        ui->flashIndicator->setPixmap(lightOn);
    } else {
        ui->flashIndicator->setPixmap(lightOff);
    }
}

void LessonWindow::setInputReadOnly(bool readOnly) {
    ui->inputText->setReadOnly(readOnly);
    ui->inputText->clearFocus();
}

void LessonWindow::setSoundPlaying() {
    QPixmap soundPlaying(QPixmap::fromImage(QImage(":/icons/playing_audio.png")));
    ui->soundDisplayLabel->setPixmap(soundPlaying);
}

void LessonWindow::setSoundNotPlaying() {
    QPixmap soundNotPlaying(QPixmap::fromImage(QImage(":/icons/not_playing_audio.png")));
    ui->soundDisplayLabel->setPixmap(soundNotPlaying);
}

void LessonWindow::setFocusInput() {
    ui->inputText->setFocus();
}

void LessonWindow::sendInputText() {
    emit sendInputTextSignal(ui->inputText->text());
}
