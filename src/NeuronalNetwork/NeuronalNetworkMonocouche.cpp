#include "NeuronalNetworkMonocouche.h"


NeuronalNetworkMonocouche::NeuronalNetworkMonocouche(void)
{
}

NeuronalNetworkMonocouche::NeuronalNetworkMonocouche(std::string filenameSamplingData)
{
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
	std::cout<<std::endl<<"TESTING ALL SAMPLES"<<std::endl;
	for (unsigned int i=0; i<this->samples.size(); ++i)
		testing(this->samples[i]);
}

std::vector<float> NeuronalNetworkMonocouche::testing(Sample caracteristicVector)
{
	std::vector<float> output;
	std::cout<<std::endl<<"TESTING A VECTOR"<<std::endl;
	caracteristicVector.to_string_CaracteristicVectorOnly();
	for (int i=0; i<this->nbNeurones-1; ++i)
	{
		output.push_back(this->neurones[i]->activeNeurone(caracteristicVector.getData()));
		std::cout<<output[output.size()-1]<<" ; ";
	}
	output.push_back(this->neurones[this->nbNeurones-1]->activeNeurone(caracteristicVector.getData()));
	std::cout<<output[output.size()-1]<<"]"<<std::endl;
	return output;
}

NeuronalNetworkMonocouche::~NeuronalNetworkMonocouche(void)
{
}
