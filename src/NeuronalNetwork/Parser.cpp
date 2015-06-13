#include "Parser.h"


Parser::Parser(void)
{
}

void Parser::loadLearningData(std::string filename)
{
	std::ifstream file(filename.c_str());

	if (file.fail())
	{
		std::cout<<"ERROR : filename no valide"<<std::endl;
		return;
	}

	std::string currentLine;
	std::getline(file, currentLine);
	size_t sizeCaracteristicVector = std::count(currentLine.begin(), currentLine.end(), ' ') + 1;
	int lines = (std::count(std::istreambuf_iterator<char>( file ), std::istreambuf_iterator<char>(), '\n')+2)/2;

	file.seekg(0);
	float value;
	while(lines--)
	{
		Sample sample;
		for (unsigned int i=0; i<sizeCaracteristicVector; ++i)
		{
			file >> value;
			sample.addData(value);
		}

		file >> value;
		sample.setClassification(static_cast<int>(value));
		if (value == 3)
				this->samples.push_back(sample);
		else
			this->samples.push_back(sample);
	}
}



std::vector<Sample> Parser::getSamples()
{
	return this->samples;
}

Parser::~Parser(void)
{
}
