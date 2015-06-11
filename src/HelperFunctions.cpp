#include "HelperFunctions.h"

void HelperFunctions::safeChoice(std::string inputSentence, std::string errorSentence, int& choice)
{
    std::cout << inputSentence;
    for (;;)
    {
        std::string input;
        std::getline(std::cin,input);

        std::stringstream myStream(input);
        if (myStream >> choice)
            break;
        else
        {
            input.clear();
            std::cout << errorSentence << std::endl;
        }
    }
}

void HelperFunctions::safeChoice(std::string inputSentence, std::string errorSentence, float& choice)
{
    std::cout << inputSentence;
    for (;;)
    {
        std::string input;
        std::getline(std::cin,input);

        std::stringstream myStream(input);
        if (myStream >> choice)
            break;
        else
        {
            input.clear();
            std::cout << errorSentence << std::endl;
        }
    }
}

void HelperFunctions::safeChoice(std::string inputSentence, std::string errorSentence, std::string& choice)
{
    std::cout << inputSentence;
    for (;;)
    {
        std::string input;
        std::getline(std::cin,input);

        std::stringstream myStream(input);
        if (myStream >> choice)
            break;
        else
        {
            input.clear();
            std::cout << errorSentence << std::endl;
        }
    }
}

void HelperFunctions::safeChoice(std::string inputSentence, std::string errorSentence, char& choice)
{
    std::cout << inputSentence;
    for (;;)
    {
        std::string input;
        std::getline(std::cin,input);


        std::stringstream myStream(input);
        if (myStream >> choice)
            break;
        else
        {
            input.clear();
            std::cout << errorSentence << std::endl;
        }
    }
}


