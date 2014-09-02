#ifndef __Player__
#define __Player__
#include "cocos2d.h"

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

	bool initWithPlayerType(PlayerType type);

	static Player* create(PlayerType type);

	// load animation only when it's not loaded
	void addAnimation();
	//Repeat the selected animation forever
	void playAnimationForever(int index);

	void walkTo(Vec2 dest);
	
private:
	PlayerType _type;  
	std::string _name;
	int _animationNum; 
	std::vector<int> _animationFrameNum;
	std::vector<std::string> _animationNames;
	float _speed;
	Sequence* _seq;

};

#endif