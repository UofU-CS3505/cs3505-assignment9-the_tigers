#include <QObject>
#include <QFile>
#include <QDebug>
#include "difficultyhandler.h"

DifficultyHandler::DifficultyHandler() : chosenDifficulty(EASY) {
    fillDictionary();
    std::srand(std::time(0));
}

void DifficultyHandler::fillDictionary() {
    QFile file(":/textfiles/dictionary.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString line;
    while(!file.atEnd()){
        line = file.readLine().trimmed();
        dictionary.push_back(line.toStdString());
    }
}

QString DifficultyHandler::getEasyString() {
    int stringIndex = rand() % easyOptions.size();
    practiceText = QString::fromStdString(easyOptions.at(stringIndex));

    return practiceText;
}

QString DifficultyHandler::getMediumString() {
    int randomSelection = rand() % 2;
    practiceText = "";

    if(randomSelection == 0) {
        int randomPracticeIndex = rand() % dictionary.size();
        practiceText = QString::fromStdString(dictionary.at(randomPracticeIndex));
    }

    else {
        // Gets a number between 100 and 10,000,000
        int randomPracticeNumber = (rand() % 9999901) + 100;
        practiceText = QString::fromStdString(std::to_string(randomPracticeNumber));
    }

    return practiceText;
}

QString DifficultyHandler::getHardString() {
    int randomSelection;
    int randomPracticeIndex;
    practiceText = "";

    for(int i = 0; i < 5; i++){
        randomSelection = rand() % 2;

        if (randomSelection == 0) {
            randomPracticeIndex = rand() % dictionary.size();
            practiceText.append(dictionary.at(randomPracticeIndex));
        }

        else {
            practiceText.append(generateCallSign());
        }

        if (i != 4) {
            practiceText.append(" ");
        }
    }

    qDebug() << practiceText;

    return practiceText;
}

QString DifficultyHandler::generateCallSign() {
    string callSign = "";

    char letter;
    int randomDigit;

    for(int i = 0; i < 5; i++){
        if(i == 1){
            randomDigit = rand() % 10;
            callSign.append(std::to_string(randomDigit));
        }

        else {
            letter = (rand() % 26) + 'a';
            callSign += letter;
        }
    }

    return QString::fromStdString(callSign);
}

QString DifficultyHandler::getPracticeString() {
    QString practiceString;

    switch(chosenDifficulty) {
    case EASY:
        practiceString = getEasyString();
        break;

    case MEDIUM:
        practiceString = getMediumString();
        break;

    case HARD:
        practiceString = getHardString();
        break;
    }

    return practiceString;
}

void DifficultyHandler::setDifficulty(QString difficulty) {
    difficulty = difficulty.toLower();

    if (difficulty == "easy") {
        chosenDifficulty = EASY;
    }

    else if( difficulty == "medium") {
        chosenDifficulty = MEDIUM;
    }

    else if( difficulty == "hard") {
        chosenDifficulty = HARD;
    }
}
