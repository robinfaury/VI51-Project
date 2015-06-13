#include "ProblemStore.h"

ProblemStore::ProblemStore()
{

}

ProblemStore::~ProblemStore()
{
	clearProblemStore();
}

void ProblemStore::clearProblemStore()
{
	this->m_QValues.clear();
}

bool ProblemStore::initQValues(std::map<int, ProblemState*> problemStates)
{
	if (problemStates.empty())
	{
		return false;
	}

	this->clearProblemStore();


	std::map<std::string, float> actionsQvalues;
	actionsQvalues.insert(std::pair<std::string, float>("left", 0.0f));
	actionsQvalues.insert(std::pair<std::string, float>("right", 0.0f));
	actionsQvalues.insert(std::pair<std::string, float>("down", 0.0f));

	std::map<int, ProblemState*>::iterator it;
	for (it = problemStates.begin(); it != problemStates.end(); ++it)
	{
		m_QValues[it->first] = actionsQvalues;
	}

	return true;
} 

std::vector<std::string>* ProblemStore::getPossibleActions(int stateId)
{
	std::vector<std::string>* actions = new std::vector<std::string>();
	for (std::map<std::string, float>::iterator it = m_QValues[stateId].begin(); it != m_QValues[stateId].end(); ++it)
	{
		actions->push_back(it->first);
	}
	return actions;
}

std::vector<std::string>* ProblemStore::getPossibleActions(ProblemState* state)
{
	return getPossibleActions(state->getId());
}

// Gest best action based on the qvalues of the given state
std::string ProblemStore::getBestAction(int stateId)
{
	// Identical values checks
	bool allIdentical = true;
	float lastValue = m_QValues[stateId].begin()->second;
	float bestValue = m_QValues[stateId].begin()->second;

	std::string bestAction = m_QValues[stateId].begin()->first;

	for (std::map<std::string, float>::iterator it = m_QValues[stateId].begin(); it != m_QValues[stateId].end(); ++it)
	{
		// Checking if all values are identical
		if (allIdentical && lastValue != it->second)
		{
			allIdentical = false;
		}
		// Else, checking to see if new value is higher than before
		if (it->second > bestValue)
		{
			bestValue = it->second;
			bestAction = it->first;
		}
		lastValue = it->second;
	}
	
	//If all values are identical, return a random action
	if (allIdentical)
	{
		bestAction = this->getOneOf(this->getPossibleActions(stateId));
	}

	return bestAction;
}

std::string ProblemStore::getBestAction(ProblemState* problemState)  // For the given state, returns the best possible action (the one with highest QValue)
{
	return getBestAction(problemState->getId());
}


// For this state and action, return the associated QValue
float ProblemStore::getQValue(int stateId, std::string action)
{
	return m_QValues[stateId][action];
}

float ProblemStore::getQValue(ProblemState* problemState, std::string action)    // For this state and action, return the associated QValue
{
	return getQValue(problemState->getId(), action);
}


void ProblemStore::updateQValue(int stateId, std::string action, float newQValue)
{
	m_QValues[stateId][action] = newQValue;
}

void ProblemStore::updateQValue(ProblemState* problemState, std::string action, float newQValue)
{
	updateQValue(problemState->getId(), action, newQValue);
}

std::string ProblemStore::getQValuesReport()
{
	std::string report = "----- QValues -----\n";
	std::map <int, std::map<std::string, float> >::iterator it;
	for (it = m_QValues.begin(); it != m_QValues.end(); ++it)
	{
		std::map<std::string, float>::iterator it2;
		report += "state " + std::to_string((*it).first) + "\n";
		for (it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
		{
			report += "\taction : " + (*it2).first + "\n";
			report += "\tQvalue : " + std::to_string((*it2).second) + "\n";
		}
	}
	return report;
}


std::string ProblemStore::getOneOf(std::vector<std::string>* possibleActions)
{
	if (possibleActions->empty())
		return NULL;

	return (*possibleActions)[rand() % possibleActions->size()];
}

// Load / save
void ProblemStore::serialize(pugi::xml_node* node)
{
	if (node == NULL)
		return;

	node->set_name("ProblemStore");

	pugi::xml_node QValues = node->append_child("QValues");
	pugi::xml_node tempState;
	pugi::xml_node tempValues;
	pugi::xml_node tempQValue;

	//Iterating on every q value
	for (std::map <int, std::map<std::string, float>>::iterator it = this->m_QValues.begin(); it != this->m_QValues.end(); ++it)
	{
		tempState = QValues.append_child("State");

		tempState.append_attribute("stateId").set_value(it->first);
		tempValues = tempState.append_child("Values");

		for (std::map<std::string, float>::iterator itValues = it->second.begin(); itValues != it->second.end(); ++itValues)
		{
			tempQValue = tempValues.append_child("QValue");

			tempQValue.append_attribute("action").set_value(itValues->first.data());
			tempQValue.append_attribute("value").set_value(itValues->second);
		}
	}
}

void ProblemStore::unserialize(pugi::xml_node* node)
{
	if (node == NULL)
		return;

	// Clearing any previous learning
	this->m_QValues.clear();

	pugi::xml_node QValues = node->child("QValues");

	// Nodes used for iterating
	pugi::xml_node tempState;
	pugi::xml_node tempValues;
	pugi::xml_node tempQValue;

	tempState = QValues.first_child();

	// Temorary variables to store results
	int stateId;
	std::string action;
	float qValue;
	std::map<std::string, float> values;

	// Iterating on every node
	while (tempState != NULL)
	{
		// Reading id
		stateId = tempState.attribute("stateId").as_int();

		//Iterating on every action/value pair
		tempQValue = tempState.first_child();
		while (tempQValue != NULL)
		{
			// Reading values
			action = tempQValue.attribute("action").as_string();
			qValue = tempQValue.attribute("value").as_float();

			values.insert(std::pair<std::string, float>(action, qValue));
			tempQValue = tempQValue.next_sibling();
		}

		//Inserting resulting values
		this->m_QValues[stateId] = values;
		
		// Preparing next iteration
		values.clear();
		tempState = tempState.next_sibling();
	}
}