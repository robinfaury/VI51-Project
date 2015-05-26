#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <vector>
#include <iostream>

class Sample
{
private:
	std::pair<std::vector<float>, std::vector<int> > data;
	int classe;

	std::vector<int> castIntToBinary(int value);

public:
	Sample(void);

	void addData(float value);
	void setClassification(int classe);

	std::vector<float>* getData();
	std::vector<int>* getVectorClasse();
	int getClasse();

	void to_string();
	void to_string_CaracteristicVectorOnly();

	~Sample(void);
};

#endif