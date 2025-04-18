#include "lessonselectwindow.h"
#include "ui_lessonselectwindow.h"

lessonselectwindow::lessonselectwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonselectwindow)
    , world(b2Vec2(0, -20))
    , timer(this)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonselectwindow::onBackButtonClicked);

    // Connections for lesson buttons
    // TODO: emit to a slot in mainwindow, which then emits a signal to lessonhandler and lessonwindow
    QObject::connect(ui->lessonButton_1, &QPushButton::clicked, this, [this](){emit selectLesson(1);});
    QObject::connect(ui->lessonButton_2, &QPushButton::clicked, this, [this](){emit selectLesson(2);});
    QObject::connect(ui->lessonButton_3, &QPushButton::clicked, this, [this](){emit selectLesson(3);});
    QObject::connect(ui->lessonButton_4, &QPushButton::clicked, this, [this](){emit selectLesson(4);});
    QObject::connect(ui->lessonButton_5, &QPushButton::clicked, this, [this](){emit selectLesson(5);});
    QObject::connect(ui->lessonButton_6, &QPushButton::clicked, this, [this](){emit selectLesson(6);});
    QObject::connect(ui->lessonButton_7, &QPushButton::clicked, this, [this](){emit selectLesson(7);});
    QObject::connect(ui->lessonButton_8, &QPushButton::clicked, this, [this](){emit selectLesson(8);});
    QObject::connect(ui->lessonButton_9, &QPushButton::clicked, this, [this](){emit selectLesson(9);});
    QObject::connect(ui->lessonButton_10, &QPushButton::clicked, this, [this](){emit selectLesson(10);});

    // Event Filters
    ui->lessonButton_1->installEventFilter(this);
    ui->lessonButton_2->installEventFilter(this);
    ui->lessonButton_3->installEventFilter(this);
    ui->lessonButton_4->installEventFilter(this);
    ui->lessonButton_5->installEventFilter(this);
    ui->lessonButton_6->installEventFilter(this);
    ui->lessonButton_7->installEventFilter(this);
    ui->lessonButton_8->installEventFilter(this);
    ui->lessonButton_9->installEventFilter(this);
    ui->lessonButton_10->installEventFilter(this);

    setupWorld();
}

lessonselectwindow::~lessonselectwindow()
{
    delete ui;
}

void lessonselectwindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool lessonselectwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void lessonselectwindow::onBackButtonClicked()
{
    emit goHome();
    userOnThisPage = false;
}

void lessonselectwindow::setupWorld() {
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -50.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(100.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2BodyDef lessonOneBodyDefinition;
    lessonOneBodyDefinition.type = b2_dynamicBody;
    lessonOneBodyDefinition.position.Set(8.0f, -48.0f);
    lessonOneBody = world.CreateBody(&lessonOneBodyDefinition);

    b2BodyDef lessonTwoBodyDefinition;
    lessonTwoBodyDefinition.type = b2_dynamicBody;
    lessonTwoBodyDefinition.position.Set(16.0f, -48.0f);
    lessonTwoBody = world.CreateBody(&lessonTwoBodyDefinition);

    b2BodyDef lessonThreeBodyDefinition;
    lessonThreeBodyDefinition.type = b2_dynamicBody;
    lessonThreeBodyDefinition.position.Set(24.0f, -48.0f);
    lessonThreeBody = world.CreateBody(&lessonThreeBodyDefinition);

    b2BodyDef lessonFourBodyDefinition;
    lessonFourBodyDefinition.type = b2_dynamicBody;
    lessonFourBodyDefinition.position.Set(32.0f, -48.0f);
    lessonFourBody = world.CreateBody(&lessonFourBodyDefinition);

    b2BodyDef lessonFiveBodyDefinition;
    lessonFiveBodyDefinition.type = b2_dynamicBody;
    lessonFiveBodyDefinition.position.Set(40.0f, -48.0f);
    lessonFiveBody = world.CreateBody(&lessonFiveBodyDefinition);

    b2BodyDef lessonSixBodyDefinition;
    lessonSixBodyDefinition.type = b2_dynamicBody;
    lessonSixBodyDefinition.position.Set(48.0f, -48.0f);
    lessonSixBody = world.CreateBody(&lessonSixBodyDefinition);

    b2BodyDef lessonSevenBodyDefinition;
    lessonSevenBodyDefinition.type = b2_dynamicBody;
    lessonSevenBodyDefinition.position.Set(56.0f, -48.0f);
    lessonSevenBody = world.CreateBody(&lessonSevenBodyDefinition);

    b2BodyDef lessonEightBodyDefinition;
    lessonEightBodyDefinition.type = b2_dynamicBody;
    lessonEightBodyDefinition.position.Set(64.0f, -48.0f);
    lessonEightBody = world.CreateBody(&lessonEightBodyDefinition);

    b2BodyDef lessonNineBodyDefinition;
    lessonNineBodyDefinition.type = b2_dynamicBody;
    lessonNineBodyDefinition.position.Set(72.0f, -48.0f);
    lessonNineBody = world.CreateBody(&lessonNineBodyDefinition);

    b2BodyDef lessonTenBodyDefinition;
    lessonTenBodyDefinition.type = b2_dynamicBody;
    lessonTenBodyDefinition.position.Set(80.0f, -48.0f);
    lessonTenBody = world.CreateBody(&lessonTenBodyDefinition);

    b2FixtureDef bodyFixtureDefinition;
    bodyFixtureDefinition.shape = &dynamicBox;
    bodyFixtureDefinition.density = 10.0f;
    bodyFixtureDefinition.friction = 0.3f;

    lessonOneBody->CreateFixture(&bodyFixtureDefinition);
    lessonTwoBody->CreateFixture(&bodyFixtureDefinition);
    lessonThreeBody->CreateFixture(&bodyFixtureDefinition);
    lessonFourBody->CreateFixture(&bodyFixtureDefinition);
    lessonFiveBody->CreateFixture(&bodyFixtureDefinition);
    lessonSixBody->CreateFixture(&bodyFixtureDefinition);
    lessonSevenBody->CreateFixture(&bodyFixtureDefinition);
    lessonEightBody->CreateFixture(&bodyFixtureDefinition);
    lessonNineBody->CreateFixture(&bodyFixtureDefinition);
    lessonTenBody->CreateFixture(&bodyFixtureDefinition);

    lessonOneY = ui->lessonButton_1->y();
    lessonTwoY = ui->lessonButton_2->y();
    lessonThreeY = ui->lessonButton_3->y();
    lessonFourY = ui->lessonButton_4->y();
    lessonFiveY = ui->lessonButton_5->y();
    lessonSixY = ui->lessonButton_6->y();
    lessonSevenY = ui->lessonButton_7->y();
    lessonEightY = ui->lessonButton_8->y();
    lessonNineY = ui->lessonButton_9->y();
    lessonTenY = ui->lessonButton_10->y();

    timer.singleShot(10, this, &lessonselectwindow::updateWorld);
}

void lessonselectwindow::updateWorld() {
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    int rowHeightOffset = 230;

    ui->lessonButton_1->move(ui->lessonButton_1->x(), lessonOneY - lessonOneBody->GetPosition().y);
    ui->lessonButton_2->move(ui->lessonButton_2->x(), lessonTwoY - lessonTwoBody->GetPosition().y);
    ui->lessonButton_3->move(ui->lessonButton_3->x(), lessonThreeY - lessonThreeBody->GetPosition().y);
    ui->lessonButton_4->move(ui->lessonButton_4->x(), lessonFourY - lessonFourBody->GetPosition().y);
    ui->lessonButton_5->move(ui->lessonButton_5->x(), lessonFiveY - lessonFiveBody->GetPosition().y);

    ui->lessonButton_6->move(ui->lessonButton_6->x(), lessonSixY - lessonSixBody->GetPosition().y + rowHeightOffset);
    ui->lessonButton_7->move(ui->lessonButton_7->x(), lessonSevenY - lessonSevenBody->GetPosition().y + rowHeightOffset);
    ui->lessonButton_8->move(ui->lessonButton_8->x(), lessonEightY - lessonEightBody->GetPosition().y + rowHeightOffset);
    ui->lessonButton_9->move(ui->lessonButton_9->x(), lessonNineY - lessonNineBody->GetPosition().y + rowHeightOffset);
    ui->lessonButton_10->move(ui->lessonButton_10->x(), lessonTenY - lessonTenBody->GetPosition().y + rowHeightOffset);

    timer.singleShot(10, this, &lessonselectwindow::updateWorld);
}

bool lessonselectwindow::eventFilter(QObject *object, QEvent *event){
    if (event->type() == QEvent::Enter) {
        if (object == ui->lessonButton_1){
            jumpButton(lessonOneBody);
        }

        if (object == ui->lessonButton_2) {
            jumpButton(lessonTwoBody);
        }

        if (object == ui->lessonButton_3) {
            jumpButton(lessonThreeBody);
        }

        if (object == ui->lessonButton_4) {
            jumpButton(lessonFourBody);
        }

        if (object == ui->lessonButton_5) {
            jumpButton(lessonFiveBody);
        }

        if (object == ui->lessonButton_6) {
            jumpButton(lessonSixBody);
        }

        if (object == ui->lessonButton_7) {
            jumpButton(lessonSevenBody);
        }

        if (object == ui->lessonButton_8) {
            jumpButton(lessonEightBody);
        }

        if (object == ui->lessonButton_9) {
            jumpButton(lessonNineBody);
        }

        if (object == ui->lessonButton_10) {
            jumpButton(lessonTenBody);
        }
    }

    return false;
}

void lessonselectwindow::jumpButton(b2Body* jumpBody){
    jumpBody->SetTransform(b2Vec2(jumpBody->GetTransform().p.x, -28.0f), 0);
    jumpBody->SetAwake(true);
}

