#ifndef DIFFICULTYHANDLER_H
#define DIFFICULTYHANDLER_H

#include <QObject>
#include <string>
#include <vector>

using std::vector;
using std::string;

/**
 * The difficulty handler.
 * Tracks current difficulty for practice mode, and generates practice problem strings for the current difficulty.
 *
 * @name Taylor Marlow, Chandler Eyre
 * @date 4/24/2025
 */
class DifficultyHandler : public QObject {
    Q_OBJECT

public: enum difficulty {EASY, MEDIUM, HARD}; // declared here for private chosen difficulty

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

    /**
     * Fills the dictionary from a file.
     * These will be the words used in medium and hard strings.
     */
    void fillDictionary();

    /**
     * Generates an easy string (one character or number)
     * @return - Easy string
     */
    QString getEasyString();

    /**
     * Generates a medium string (one word or multi-digit number)
     * @return - Medium string
     */
    QString getMediumString();

    /**
     * Generates a hard string (5 words/callsigns)
     * @return - Hard string
     */
    QString getHardString();

    /**
     * Generates a callsign: 1 letter, 1 number, then 4 more letters.
     * @return - A callsign
     */
    QString generateCallSign();

public:
    /**
     * Difficulty handler constructor.
     */
    DifficultyHandler();

    /**
     * @return The current difficulty
     */
    difficulty getDifficulty();

public slots:
    /**
     * Sets the difficulty
     * @param difficulty - easy, medium, or hard
     */
    void setDifficulty(QString difficulty);

    /**
     * Generates practice problem string using current difficulty.
     * @return - The practice problem string
     */
    QString getPracticeString();

signals:
    /**
     * Sends practice problem string.
     * @param practiceText - Practice problem string to send
     */
    void sendPracticeString(QString practiceText);
};

#endif // DIFFICULTYHANDLER_H
