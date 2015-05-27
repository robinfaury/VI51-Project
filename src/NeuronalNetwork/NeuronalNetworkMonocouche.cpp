#include "NeuronalNetworkMonocouche.h"


NeuronalNetworkMonocouche::NeuronalNetworkMonocouche(void)
{
}

NeuronalNetworkMonocouche::NeuronalNetworkMonocouche(std::string filenameSamplingData)
{
	srand (static_cast <unsigned> (time(0)));

	Parser parser;
	parser.loadLearningData(filenameSamplingData);
	this->samples = parser.getSamples();
	this->nbNeurones = 0;
	for (unsigned int i=0; i<this->samples.size(); ++i)
		if (this->nbNeurones < this->samples[i].getClasse())
			this->nbNeurones =this->samples[i].getClasse();
	++this->nbNeurones;
}

void NeuronalNetworkMonocouche::learning()
{
	std::cout<<std::endl<<"LEARNING"<<std::endl;
	for (unsigned int i=0; i<this->samples.size(); ++i)
		this->samples[i].to_string();

	for (int i=0; i<this->nbNeurones; ++i)
	{
		this->neurones.push_back(new Neurone(5));
		for (int j=0; j<10; ++j)
		{
			for (unsigned int k=0; k<this->samples.size(); ++k)
				this->neurones[i]->learn(this->samples[k].getData(), (*this->samples[k].getVectorClasse())[i]);
		}
	}
}

void NeuronalNetworkMonocouche::testing()
{
	std::cout<<std::endl<<"TESTING"<<std::endl;
	for (unsigned int i=0; i<this->samples.size(); ++i)
	{
		this->samples[i].to_string_CaracteristicVectorOnly();
		for (int j=0; j<this->nbNeurones-1; ++j)
			std::cout<<this->neurones[j]->activeNeurone(this->samples[i].getData())<<" ; ";
		std::cout<<this->neurones[this->nbNeurones-1]->activeNeurone(this->samples[i].getData())<<"]"<<std::endl;
	}
}

NeuronalNetworkMonocouche::~NeuronalNetworkMonocouche(void)
{
}
