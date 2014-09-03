#include "FSM.h"

namespace fsm
{
	void FSM::addState(State state)
	{
		_states.push_back(state);
	}

	//void FSM::addState(std::vector<std::string> states)
	//{
	//	_states.insert(_states.end(),states.begin(),states.end());
	//}

	void FSM::printState()
	{
	//	std::for_each(_states.begin(), _states.end(), &FSM::print);

		for(auto iter = _states.begin(); iter!=_states.end(); iter++)
		{
			cocos2d::log(iter->name.c_str());
		}
	}


	bool FSM::init()
	{
		this->addState(State("sth"));
		this->printState();
		return true;
	}
};

