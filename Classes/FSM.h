#ifndef __FSM__
#define __FSM__

#include "cocos2d.h"


class FSM :public cocos2d::Ref
{
public:
	bool init();
	static FSM* create(std::string state, std::function<void()> onEnter = nullptr);

	FSM(std::string state, std::function<void()> onEnter = nullptr);

	FSM* addState(std::string state, std::function<void()> onEnter = nullptr);

	FSM* addEvent(std::string eventName, std::string from, std::string to);

	bool isContainState(std::string stateName);

	void printState();

	void doEvent(std::string eventName);

	bool canDoEvent(std::string eventName);

	void setOnEnter(std::string state, std::function<void()> onEnter);
private:
	void changeToState(std::string state);
private:
	std::set<std::string> _states;
	std::unordered_map<std::string,std::unordered_map<std::string,std::string>> _events;
	std::string _currentState;
	std::string _previousState;
	std::unordered_map<std::string,std::function<void()>> _onEnter;
};


#endif