#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <algorithm>

#include "Sample.h"

class Parser
{
private:
	std::vector<Sample> samples;


public:
	Parser(void);

	void loadLearningData(std::string filename);

	std::vector<Sample> getSamples();

	~Parser(void);
};

#endif
