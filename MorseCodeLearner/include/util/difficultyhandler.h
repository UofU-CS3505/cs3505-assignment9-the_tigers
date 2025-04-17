#ifndef DIFFICULTYHANDLER_H
#define DIFFICULTYHANDLER_H

#include <QObject>
#include <string>
#include <vector>

using std::vector;
using std::string;

class DifficultyHandler : public QObject {
    Q_OBJECT

public:
    DifficultyHandler();

    enum difficulty {EASY, MEDIUM, HARD};

    /**
     * @return the current difficulty
     */
    difficulty getDifficulty();

private:
    vector<string> easyOptions = {"a", "b", "c", "d", "e", "f", "g", "h",
                                  "i", "j", "k", "l", "m", "n", "o", "p",
                                  "q", "r", "s", "t", "u", "v", "w", "x",
                                  "y", "z", "1", "2", "3", "4", "5", "6",
                                  "7", "8", "9", "0"};

    // Dictionary sourced from ef.edu's most common English words list
    vector<string> dictionary;

    QString practiceText = "";
    difficulty chosenDifficulty = EASY;
    void fillDictionary();
    QString getEasyString();
    QString getMediumString();
    QString getHardString();
    QString generateCallSign();

public slots:
    void setDifficulty(QString difficulty);
    QString getPracticeString();

signals:
    void sendPracticeString(QString practiceText);

};

#endif // DIFFICULTYHANDLER_H
