#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWindow), showingHelp(false)
    , world(b2Vec2(0, -20))
    , timer(this)
{
    ui->setupUi(this);

    ui->settingsnav->setIcon(QIcon(":/icons/settings.png"));
    ui->settingsnav->setIconSize(QSize(51, 51));

    ui->referencenav->setIcon(QIcon(":/icons/encodings.png"));
    ui->referencenav->setIconSize(QSize(56, 56));

    ui->helpnav->setIcon(QIcon(":/icons/help.png"));
    ui->helpnav->setIconSize(QSize(50, 50));

    ui->settingsHelpLabel->hide();
    ui->learnHelpLabel->hide();
    ui->practiceHelpLabel->hide();
    ui->translateHelpLabel->hide();
    ui->encodingsHelpLabel->hide();

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
    QObject::connect(ui->referencenav, &QPushButton::clicked, this, [=]() {
        emit goToReferencePage();
        userOnThisPage = false;
    });
    QObject::connect(ui->helpnav, &QPushButton::clicked, this, &MenuWindow::toggleHelp);

    ui->practicenav->installEventFilter(this);
    ui->learningnav->installEventFilter(this);
    ui->translatenav->installEventFilter(this);
    ui->settingsnav->installEventFilter(this);
    ui->helpnav->installEventFilter(this);
    ui->referencenav->installEventFilter(this);

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
        ui->encodingsHelpLabel->show();
        showingHelp = true;
    } else {
        ui->settingsHelpLabel->hide();
        ui->learnHelpLabel->hide();
        ui->practiceHelpLabel->hide();
        ui->translateHelpLabel->hide();
        ui->encodingsHelpLabel->hide();
        showingHelp = false;
    }
}

void MenuWindow::setupWorld() {
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

    b2BodyDef referenceBodyDefinition;
    referenceBodyDefinition.type = b2_dynamicBody;
    referenceBodyDefinition.position.Set(48.0f, 0.0f);
    referenceBody = world.CreateBody(&referenceBodyDefinition);

    b2FixtureDef bodyFixtureDefinition;
    bodyFixtureDefinition.shape = &dynamicBox;
    bodyFixtureDefinition.density = 10.0f;
    bodyFixtureDefinition.friction = 0.3f;

    practiceBody->CreateFixture(&bodyFixtureDefinition);
    lessonBody->CreateFixture(&bodyFixtureDefinition);
    translatorBody->CreateFixture(&bodyFixtureDefinition);
    settingsBody->CreateFixture(&bodyFixtureDefinition);
    helpBody->CreateFixture(&bodyFixtureDefinition);
    referenceBody->CreateFixture(&bodyFixtureDefinition);

    practiceButtonY = ui->practicenav->y();
    lessonButtonY = ui->learningnav->y();
    translatorButtonY = ui->translatenav->y();
    settingsButtonY = ui->settingsnav->y();
    helpButtonY = ui->helpnav->y();
    referenceButtonY = ui->referencenav->y();

    timer.singleShot(10, this, &MenuWindow::updateWorld);
}

void MenuWindow::updateWorld() {
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    world.Step(timeStep, velocityIterations, positionIterations);

    ui->practicenav->move(ui->practicenav->x(), practiceButtonY - practiceBody->GetPosition().y);
    ui->learningnav->move(ui->learningnav->x(), lessonButtonY - lessonBody->GetPosition().y);
    ui->translatenav->move(ui->translatenav->x(), translatorButtonY - translatorBody->GetPosition().y);
    ui->settingsnav->move(ui->settingsnav->x(), settingsButtonY - settingsBody->GetPosition().y);
    ui->helpnav->move(ui->helpnav->x(), helpButtonY - helpBody->GetPosition().y);
    ui->referencenav->move(ui->referencenav->x(), referenceButtonY - referenceBody->GetPosition().y);

    timer.singleShot(10, this, &MenuWindow::updateWorld);
}

void MenuWindow::jumpButton(b2Body* jumpBody){
    jumpBody->SetTransform(b2Vec2(jumpBody->GetTransform().p.x, 8.0f), 0);
    jumpBody->SetAwake(true);
}

bool MenuWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Enter) {
        if (object == ui->practicenav){
            jumpButton(practiceBody);
        }

        if (object == ui->learningnav) {
            jumpButton(lessonBody);
        }

        if (object == ui->translatenav) {
            jumpButton(translatorBody);
        }

        if (object == ui->settingsnav) {
            jumpButton(settingsBody);
        }

        if (object == ui->helpnav) {
            jumpButton(helpBody);
        }

        if (object == ui->referencenav) {
            jumpButton(referenceBody);
        }
    }

    return false;
}
