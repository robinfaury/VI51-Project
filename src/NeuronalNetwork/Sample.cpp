#include "Sample.h"


Sample::Sample(void)
{
}

void Sample::addData(float value)
{
	this->data.first.push_back(value);
}

void Sample::setClassification(int classe)
{
	this->data.second.clear();
	this->data.second = castIntToBinary(classe);
	this->classe = classe;
}


std::vector<int> Sample::castIntToBinary(int value)
{
	std::vector<int> returnValue;
	for (int i=0; i<4; ++i)
		returnValue.push_back(0);
	returnValue[value] = 1;
	return returnValue;
}

void Sample::to_string()
{
	to_string_CaracteristicVectorOnly();
	for (unsigned int i=0; i<this->data.second.size()-1; ++i)
		std::cout<<this->data.second[i]<<" ; ";
	std::cout<<this->data.second[this->data.second.size()-1]<<"]"<<std::endl;
}

void Sample::to_string_CaracteristicVectorOnly()
{
	std::cout<<"Samples : Caracteristic Vector : [";
	for (unsigned int i=0; i<this->data.first.size()-1; ++i)
		std::cout<<this->data.first[i]<<" ; ";
	std::cout<<this->data.first[this->data.first.size()-1]<<"] -> [";
}

std::vector<float>* Sample::getData()
{
	return &this->data.first;
}

std::vector<int>* Sample::getVectorClasse()
{
	return &this->data.second;
}

int Sample::getClasse()
{
	return this->classe;
}

Sample::~Sample(void)
{
}
