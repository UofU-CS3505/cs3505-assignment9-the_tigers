#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "Box2D/Box2D.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWindow), showingHelp(false)
    , world(b2Vec2(0, -20))
    , timer(this)
{
    ui->setupUi(this);

    ui->settingsnav->setIcon(QIcon(":/icons/settings.svg"));
    ui->settingsnav->setIconSize(QSize(52, 52));

    ui->settingsHelpLabel->hide();
    ui->learnHelpLabel->hide();
    ui->practiceHelpLabel->hide();
    ui->translateHelpLabel->hide();

    QObject::connect(ui->translatenav, &QPushButton::clicked, this, [=]() {
        emit goToTranslatorPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->practicenav, &QPushButton::clicked, this, [=]() {
        emit goToPracticePage();
        userOnThisPage = false;
    });
    QObject::connect(ui->learningnav, &QPushButton::clicked, this, [=]() {
        emit goToLessonPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->settingsnav, &QPushButton::clicked, this, [=]() {
        emit goToSettingsPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->helpnav, &QPushButton::clicked, this, &MenuWindow::toggleHelp);

    ui->practicenav->installEventFilter(this);
    ui->learningnav->installEventFilter(this);
    ui->translatenav->installEventFilter(this);
    ui->settingsnav->installEventFilter(this);
    ui->helpnav->installEventFilter(this);

    setupWorld();
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool MenuWindow::getUserOnThisPage() {
    return userOnThisPage;
}

void MenuWindow::toggleHelp() {
    if (!showingHelp) {
        ui->settingsHelpLabel->show();
        ui->learnHelpLabel->show();
        ui->practiceHelpLabel->show();
        ui->translateHelpLabel->show();
        showingHelp = true;
    } else {
        ui->settingsHelpLabel->hide();
        ui->learnHelpLabel->hide();
        ui->practiceHelpLabel->hide();
        ui->translateHelpLabel->hide();
        showingHelp = false;
    }
}

void MenuWindow::setupWorld(){
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2BodyDef practiceBodyDefinition;
    practiceBodyDefinition.type = b2_dynamicBody;
    practiceBodyDefinition.position.Set(8.0f, 0.0f);
    practiceBody = world.CreateBody(&practiceBodyDefinition);

    b2BodyDef lessonBodyDefinition;
    lessonBodyDefinition.type = b2_dynamicBody;
    lessonBodyDefinition.position.Set(16.0f, 0.0f);
    lessonBody = world.CreateBody(&lessonBodyDefinition);

    b2BodyDef translatorBodyDefinition;
    translatorBodyDefinition.type = b2_dynamicBody;
    translatorBodyDefinition.position.Set(24.0f, 0.0f);
    translatorBody = world.CreateBody(&translatorBodyDefinition);

    b2BodyDef settingsBodyDefinition;
    settingsBodyDefinition.type = b2_dynamicBody;
    settingsBodyDefinition.position.Set(32.0f, 0.0f);
    settingsBody = world.CreateBody(&settingsBodyDefinition);

    b2BodyDef helpBodyDefinition;
    helpBodyDefinition.type = b2_dynamicBody;
    helpBodyDefinition.position.Set(40.0f, 0.0f);
    helpBody = world.CreateBody(&helpBodyDefinition);

    b2FixtureDef bodyFixtureDefinition;
    bodyFixtureDefinition.shape = &dynamicBox;
    bodyFixtureDefinition.density = 10.0f;
    bodyFixtureDefinition.friction = 0.3f;

    practiceBody->CreateFixture(&bodyFixtureDefinition);
    lessonBody->CreateFixture(&bodyFixtureDefinition);
    translatorBody->CreateFixture(&bodyFixtureDefinition);
    settingsBody->CreateFixture(&bodyFixtureDefinition);
    helpBody->CreateFixture(&bodyFixtureDefinition);

    practiceButtonY = ui->practicenav->y();
    lessonButtonY = ui->learningnav->y();
    translatorButtonY = ui->translatenav->y();
    settingsButtonY = ui->settingsnav->y();
    helpButtonY = ui->helpnav->y();

    timer.singleShot(10, this, &MenuWindow::updateWorld);
}

void MenuWindow::updateWorld(){
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    ui->practicenav->move(ui->practicenav->x(), practiceButtonY - practiceBody->GetPosition().y);
    ui->learningnav->move(ui->learningnav->x(), lessonButtonY - lessonBody->GetPosition().y);
    ui->translatenav->move(ui->translatenav->x(), translatorButtonY - translatorBody->GetPosition().y);
    ui->settingsnav->move(ui->settingsnav->x(), settingsButtonY - settingsBody->GetPosition().y);
    ui->helpnav->move(ui->helpnav->x(), helpButtonY - helpBody->GetPosition().y);

    timer.singleShot(10, this, &MenuWindow::updateWorld);
}

void MenuWindow::jumpPractice(){
    practiceBody->SetTransform(b2Vec2(practiceBody->GetTransform().p.x, 8.0f), 0);
    practiceBody->SetAwake(true);
}

void MenuWindow::jumpLesson(){
    lessonBody->SetTransform(b2Vec2(lessonBody->GetTransform().p.x, 8.0f), 0);
    lessonBody->SetAwake(true);
}

void MenuWindow::jumpTranslator(){
    translatorBody->SetTransform(b2Vec2(translatorBody->GetTransform().p.x, 8.0f), 0);
    translatorBody->SetAwake(true);
}

void MenuWindow::jumpSettings(){
    settingsBody->SetTransform(b2Vec2(settingsBody->GetTransform().p.x, 8.0f), 0);
    settingsBody->SetAwake(true);
}

void MenuWindow::jumpHelp(){
    helpBody->SetTransform(b2Vec2(helpBody->GetTransform().p.x, 8.0f), 0);
    helpBody->SetAwake(true);
}

bool MenuWindow::eventFilter(QObject *object, QEvent *event){
    if(object == ui->practicenav && event->type() == QEvent::HoverEnter){
        jumpPractice();
    }

    if(object == ui->learningnav && event->type() == QEvent::HoverEnter){
        jumpLesson();
    }

    if(object == ui->translatenav && event->type() == QEvent::HoverEnter){
        jumpTranslator();
    }

    if(object == ui->settingsnav && event->type() == QEvent::HoverEnter){
        jumpSettings();
    }

    if(object == ui->helpnav && event->type() == QEvent::HoverEnter){
        jumpHelp();
    }

    return false;
}
