#include "lessonwindow.h"
#include "ui_lessonwindow.h"

lessonwindow::lessonwindow(LessonHandler *lessonHandler, MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
    , lessonHandler(lessonHandler)
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
    , timer(this)
    , world(b2Vec2(0, -20))
{
    ui->setupUi(this);

    qDebug() << "lesson page constructed";


    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));

    // Light indicator
    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Buttons
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonwindow::onBackButtonClicked);
    QObject::connect(this, &lessonwindow::backButtonClicked, lessonHandler, &LessonHandler::onBackButtonClicked);

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, lessonHandler, &LessonHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, lessonHandler, &LessonHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, lessonHandler, &LessonHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, lessonHandler, &LessonHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, lessonHandler, &LessonHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, lessonHandler, &LessonHandler::handleRightArrowReleased);

    // Morse Handler
    QObject::connect(morseHandler, &MorseHandler::decodedInput, lessonHandler, &LessonHandler::onMorseReceived);

    // Lesson Handler
    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});
    QObject::connect(lessonHandler, &LessonHandler::guessCorrect, this, &lessonwindow::guessCorrect);
    QObject::connect(lessonHandler, &LessonHandler::guessIncorrect, this, &lessonwindow::guessIncorrect);
    QObject::connect(lessonHandler, &LessonHandler::displayTextToUI, this, &lessonwindow::displayTextQuestion);
    QObject::connect(lessonHandler, &LessonHandler::updateInputText, this, &lessonwindow::updateInputText);
    QObject::connect(lessonHandler, &LessonHandler::completedLesson, this, &lessonwindow::onBackButtonClicked);
    QObject::connect(lessonHandler, &LessonHandler::updateLessonTitle, this, &lessonwindow::updateLessonTitle);

    // Iambic paddle illustrations
    QObject::connect(lessonHandler, &LessonHandler::paddleSelected, this, [this, keyEventFilter](){
        QObject::disconnect(straightPressedConnection);
        QObject::disconnect(straightReleasedConnection);

        QPixmap paddleRight(QPixmap::fromImage(QImage(":/images/paddle_right.png")));
        QPixmap paddleLeft(QPixmap::fromImage(QImage(":/images/paddle_left.png")));
        QPixmap paddleCenter(QPixmap::fromImage(QImage(":/images/paddle_center.png")));
        ui->illustrationLabel->setPixmap(paddleCenter);
        rightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, [this, paddleRight](){ui->illustrationLabel->setPixmap(paddleRight);});
        rightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, [this, paddleCenter](){ui->illustrationLabel->setPixmap(paddleCenter);});
        leftPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, [this, paddleLeft](){ui->illustrationLabel->setPixmap(paddleLeft);});
        leftReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, [this, paddleCenter](){ui->illustrationLabel->setPixmap(paddleCenter);});
    });

    // Straight key illustrations
    QObject::connect(lessonHandler, &LessonHandler::straightKeySelected, this, [this, keyEventFilter](){
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

    setupWorld();
}

lessonwindow::~lessonwindow()
{
    delete ui;
}

void lessonwindow::onBackButtonClicked() {
    emit backButtonClicked();
    emit goToLessonSelect();
}

void lessonwindow::guessCorrect() {
    ui->problemText->setText("Correct!");

    textJumpBody->SetLinearVelocity(b2Vec2(0, 15));
    textJumpBody->SetAwake(true);
}

void lessonwindow::guessIncorrect() {
    ui->problemText->setText("Incorrect!");

    shakeFrameCount = 0;
    currentlyShaking = true;
}

void lessonwindow::displayTextQuestion(QString text) {
    ui->inputText->setText("");
    ui->problemText->setText(text);
}

void lessonwindow::updateInputText(QString inputText) {
    ui->inputText->setText(inputText);
}

void lessonwindow::updateLessonTitle(int lessonNumber) {
    ui->headerLabel->setText("Lesson " + QString::fromStdString(std::to_string(lessonNumber)));
}

void lessonwindow::setupWorld() {
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

    timer.singleShot(10, this, &lessonwindow::updateWorld);
}

void lessonwindow::updateWorld() {
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

    timer.singleShot(10, this, &lessonwindow::updateWorld);
}
