#ifndef __FSM__
#define __FSM__

#include "cocos2d.h"

namespace fsm
{
	class Event
	{
	public:
		std::string name;
		std::string from;
		std::string to;
		Event(std::string name, std::string from, std::string to)
		{
			this->name = name;
			this->from = from;
			this->to = to;
		}
	};

	class State
	{
	public:
		std::string name;
		std::function<void()> onEnter;
		std::function<void()> onExit;
		State(std::string name)
		{
			this->name = name;
			this->onEnter = [](){};
			this->onExit = [](){};
		}
	};

	class FSM :public cocos2d::Ref
	{
	public:
		bool init();
		CREATE_FUNC(FSM);

		FSM(){}
		void addState(State state);
//		void addState(std::vector<std::string> states);
		void printState();
	private:
		std::vector<State> _states;
		std::vector<Event> _events;
	};



};



#endif