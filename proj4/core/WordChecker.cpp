// WordChecker.cpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <string>
#include <algorithm>



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    if (words.contains(word))
    {
        return true;
    } else {
        return false;
    }
    
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> toReturn;

    //1: Swap adjacent pairs
    for (int i = 1; i < word.size(); ++i)
    {
        std::string testWord = word;
        std::swap(testWord[i - 1], testWord[i]);
        if (wordExists(testWord) && !std::count(toReturn.begin(), toReturn.end(), testWord)) {
            toReturn.push_back(testWord);
        }
    }

    //2: Insert A-Z between characters
    for (int i = 0; i <= word.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j)
        {
            std::string testWord = word;
            testWord.insert(i, 1, alphabet.at(j));
            if (wordExists(testWord) && !std::count(toReturn.begin(), toReturn.end(), testWord)) {
                toReturn.push_back(testWord);
            }
        }
    }

    //3: Delete each character
    for (int i = 0; i < word.size(); ++i) {
        std::string testWord = word;
        testWord.erase(testWord.begin() + i);
        if (wordExists(testWord) && !std::count(toReturn.begin(), toReturn.end(), testWord)) {
            toReturn.push_back(testWord);
        }
    }

    //4: Replace each character with A-Z
    for (int i = 0; i < word.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j)
        {
            std::string testWord = word;
            testWord[i] = alphabet.at(j);
            if (wordExists(testWord) && !std::count(toReturn.begin(), toReturn.end(), testWord)) {
                toReturn.push_back(testWord);
            }
        }
    }

    //5: Split into word pairs
    for (int i = 1; i < word.size(); ++i)
    {
        std::string testWord1 = word.substr(0, i);
        std::string testWord2 = word.substr(i, word.size() - 1);

        if (wordExists(testWord1) && wordExists(testWord2)) {
            if (!std::count(toReturn.begin(), toReturn.end(), testWord1)) {
                toReturn.push_back(testWord1);
            }
            if (!std::count(toReturn.begin(), toReturn.end(), testWord2)) {
                toReturn.push_back(testWord2);
            }
        }
    }

    return toReturn;
}

