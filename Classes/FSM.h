#ifndef __FSM__
#define __FSM__

#include "cocos2d.h"

class FSM :public cocos2d::Ref
{
public:

	bool init();
	//Create FSM with a initial state name and optional callback function
	static FSM* create(std::string state, std::function<void()> onEnter = nullptr);
	
	FSM(std::string state, std::function<void()> onEnter = nullptr);
	//add state into FSM
	FSM* addState(std::string state, std::function<void()> onEnter = nullptr);
	//add Event into FSM
	FSM* addEvent(std::string eventName, std::string from, std::string to);
	//check if state is already in FSM
	bool isContainState(std::string stateName);
	//print a list of states
	void printState();
	//do the event
	void doEvent(std::string eventName);
	//check if the event can change state
	bool canDoEvent(std::string eventName);
	//set the onEnter callback for a specified state
	void setOnEnter(std::string state, std::function<void()> onEnter);

	std::string getState(){return _currentState;}
private:
	//change state and run callback.
	void changeToState(std::string state);
private:
	std::set<std::string> _states;
	std::unordered_map<std::string,std::unordered_map<std::string,std::string>> _events;
	std::unordered_map<std::string,std::function<void()>> _onEnters;
	std::string _currentState;
	std::string _previousState;
};

#endif