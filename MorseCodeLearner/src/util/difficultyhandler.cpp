#include <QObject>
#include <QFile>
#include <QDebug>
#include "difficultyhandler.h"

DifficultyHandler::DifficultyHandler() {
    fillDictionary();
    std::srand(std::time(0));
}

void DifficultyHandler::fillDictionary() {
    QFile file("../dictionary.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString line;
    while(!file.atEnd()){
        line = file.readLine();
        dictionary.push_back(line.toStdString());
    }
}

string DifficultyHandler::getEasyString() {
    int stringIndex = rand() % easyOptions.size();
    practiceText = easyOptions.at(stringIndex);

    return practiceText;
}

string DifficultyHandler::getMediumString() {
    int randomSelection = rand() % 2;
    practiceText = "";

    if(randomSelection == 0){
        int randomPracticeIndex = rand() % dictionary.size();
        practiceText = dictionary.at(randomPracticeIndex);
    }

    else {
        // Gets a number between 100 and 10,000,000
        int randomPracticeNumber = (rand() % 9999901) + 100;
        practiceText = std::to_string(randomPracticeNumber);
    }

    return practiceText;
}

string DifficultyHandler::getHardString() {
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

    return practiceText;
}

string DifficultyHandler::generateCallSign() {
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
            callSign.append(std::to_string(letter));
        }
    }

    return callSign;
}

string DifficultyHandler::getPracticeString() {
    switch(chosenDifficulty){
    case EASY:
        return getEasyString();
        break;

    case MEDIUM:
        return getMediumString();
        break;

    case HARD:
        return getHardString();
        break;
    }
}

void DifficultyHandler::setDifficulty(string difficulty) {
    if (difficulty == "easy"){
        chosenDifficulty = EASY;
    }

    else if( difficulty == "medium"){
        chosenDifficulty = MEDIUM;
    }

    else if( difficulty == "hard"){
        chosenDifficulty = HARD;
    }
}
