#ifndef __Player__
#define __Player__
#include "cocos2d.h"
#include "FSM.h"
#include "Progress.h"

USING_NS_CC;


class Player : public Sprite
{
public:
	enum PlayerType
	{
		PLAYER,
		ENEMY1,
		ENEMY2,
		BOSS
	};

	enum ActionTag
	{
		WALKTO_TAG =100
	};

	enum AnimationType
	{
		WALKING = 0,
		ATTACKING,
		DEAD,
		BEINGHIT,
		SKILL
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

	PlayerType getPlayerType(){return _type;}

	void setCanAttack(bool canAttack){_isCanAttack=canAttack;}
	
	bool isCanAttack(){return _isCanAttack;}

	Animate* getAnimateByType(AnimationType type);

	bool onTouch(Touch* touch, Event* event);

	void attack();

	void beHit(int attack);

	int getAttack(){return _attack;}

	std::string getState(){return _fsm->getState();};

	void addAttacker(Player* attacker);

	void removeAttacker(Player* attacker);

	bool isInRange(Player* enemy);
	void moveProgress(float dx);

	Vec2 getBestAttackPosition(const Vec2& pos);

	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _maxHealth, MaxHealth);

private:
	PlayerType _type;  
	std::string _name;
	int _animationNum; 
	std::vector<int> _animationFrameNum;
	std::vector<std::string> _animationNames;
	float _speed;
	FSM* _fsm;
	Progress* _progress;
	bool _isShowBar;
	bool _isCanAttack;
	EventListenerTouchOneByOne* _listener;
	//int _health;
	//int _maxHealth;
	int _attack;
	Vector<Player*> _attackers;
	bool _flip;
	bool _flipped;
};

#endif