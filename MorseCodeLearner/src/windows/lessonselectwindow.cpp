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
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2BodyDef lessonOneBodyDefinition;
    lessonOneBodyDefinition.type = b2_dynamicBody;
    lessonOneBodyDefinition.position.Set(8.0f, 0.0f);
    lessonOneBody = world.CreateBody(&lessonOneBodyDefinition);

    b2BodyDef lessonTwoBodyDefinition;
    lessonTwoBodyDefinition.type = b2_dynamicBody;
    lessonTwoBodyDefinition.position.Set(16.0f, 0.0f);
    lessonTwoBody = world.CreateBody(&lessonTwoBodyDefinition);

    b2FixtureDef bodyFixtureDefinition;
    bodyFixtureDefinition.shape = &dynamicBox;
    bodyFixtureDefinition.density = 10.0f;
    bodyFixtureDefinition.friction = 0.3f;

    lessonOneBody->CreateFixture(&bodyFixtureDefinition);
    lessonTwoBody->CreateFixture(&bodyFixtureDefinition);

    lessonOneY = ui->lessonButton_1->y();
    lessonTwoY = ui->lessonButton_2->y();

    timer.singleShot(10, this, &lessonselectwindow::updateWorld);
}

void lessonselectwindow::updateWorld() {
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    ui->lessonButton_1->move(ui->lessonButton_1->x(), lessonOneY - lessonOneBody->GetPosition().y);
    ui->lessonButton_2->move(ui->lessonButton_2->x(), lessonTwoY - lessonTwoBody->GetPosition().y);

    timer.singleShot(10, this, &lessonselectwindow::updateWorld);
}

bool lessonselectwindow::eventFilter(QObject *object, QEvent *event){
    if (event->type() == QEvent::Enter) {
        if (object == ui->lessonButton_1){
            jumpLessonOne();
        }

        if (object == ui->lessonButton_2) {
            jumpLessonTwo();
        }
    }

    return false;
}

void lessonselectwindow::jumpLessonOne(){
    lessonOneBody->SetTransform(b2Vec2(lessonOneBody->GetTransform().p.x, 8.0f), 0);
    lessonOneBody->SetAwake(true);
}

void lessonselectwindow::jumpLessonTwo(){
    lessonTwoBody->SetTransform(b2Vec2(lessonTwoBody->GetTransform().p.x, 8.0f), 0);
    lessonTwoBody->SetAwake(true);
}

void lessonselectwindow::jumpLessonThree(){

}

void lessonselectwindow::jumpLessonFour(){

}

void lessonselectwindow::jumpLessonFive(){

}

void lessonselectwindow::jumpLessonSix(){

}

void lessonselectwindow::jumpLessonSeven(){

}

void lessonselectwindow::jumpLessonEight(){

}

void lessonselectwindow::jumpLessonNine(){

}

void lessonselectwindow::jumpLessonTen(){

}

