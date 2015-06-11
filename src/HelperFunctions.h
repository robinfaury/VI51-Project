#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include <iostream>
#include <string>
#include <sstream>

class HelperFunctions
{
public :
    static void safeChoice(std::string inputSentence, std::string errorSentence, int& choice);
    static void safeChoice(std::string inputSentence, std::string errorSentence, float& choice);
    static void safeChoice(std::string inputSentence, std::string errorSentence, std::string& choice);
    static void safeChoice(std::string inputSentence, std::string errorSentence, char& choice);
};

#endif
