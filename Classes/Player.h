#ifndef __Player__
#define __Player__
#include "cocos2d.h"
#include "FSM.h"

USING_NS_CC;


class Player : public Sprite
{
public:
	enum PlayerType
	{
		PLAYER,
		ENEMY1,
		ENEMY2
	};

	enum ActionTag
	{
		WALKTO_TAG =100
	};

	bool initWithPlayerType(PlayerType type);

	static Player* create(PlayerType type);

	// load animation only when it's not loaded
	void addAnimation();
	//Repeat the selected animation forever
	void playAnimationForever(int index);

	void walkTo(Vec2 dest);

	void initFSM();

	void onWalk(Vec2 dest);

	void onExit();
	
private:
	PlayerType _type;  
	std::string _name;
	int _animationNum; 
	std::vector<int> _animationFrameNum;
	std::vector<std::string> _animationNames;
	float _speed;
	FSM* _fsm;
};

#endif