#ifndef DIFFICULTYHANDLER_H
#define DIFFICULTYHANDLER_H

#include <QObject>

using std::vector;
using std::string;

class DifficultyHandler : public QObject {
    Q_OBJECT
private:
    enum difficulty {EASY, MEDIUM, HARD};

    vector<string> easyOptions = {"a", "b", "c", "d", "e", "f", "g", "h",
                                  "i", "j", "k", "l", "m", "n", "o", "p",
                                  "q", "r", "s", "t", "u", "v", "w", "x",
                                  "y", "z", "1", "2", "3", "4", "5", "6",
                                  "7", "8", "9", "0"};

    // Dictionary sourced from ef.edu's most common English words list
    vector<string> dictionary;

public:
    DifficultyHandler();

private:
    enum difficulty chosenDifficulty = EASY;
    string practiceText;

    void fillDictionary();
    string getEasyString();
    string getMediumString();
    string getHardString();
    string generateCallSign();

public slots:
    void setDifficulty(string difficulty);
    string getPracticeString();

signals:
    void sendPracticeString(string practiceText);

};

#endif // DIFFICULTYHANDLER_H
