#include "Player.h"


bool Player::initWithPlayerType(PlayerType type)
{
	std::string sfName = "";
	_type = type;
	int animationFrameNum[5] ={4, 4, 4, 2, 4};
	int animationFrameNum2[5] ={3, 3, 3, 2, 0};

	//setup according to PlayerType
	switch(type)
	{
	case PlayerType::PLAYER:
		sfName = "player1-1-1.png";
		_name = "player1";
		_animationNum = 5;
		_animationFrameNum.assign(animationFrameNum, animationFrameNum + 5);
		break;
	case PlayerType::ENEMY1:
		sfName = "enemy1-1-1.png";
		_name = "enemy1";
		_animationNum = 4;
		_animationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
		break;
	case PlayerType::ENEMY2:
		sfName = "enemy2-1-1.png";
		_name = "enemy2";
		_animationNum = 4;
		_animationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
		break;
	}
	this->initWithSpriteFrameName(sfName);

	std::string animationNames[] = {"walk", "attack", "dead", "hit", "skill"};
	_animationNames.assign(animationNames, animationNames + 5);

	//load animation
	this->addAnimation();

	return true;
}


Player* Player::create(PlayerType type)
{
	Player* player = new Player();
	if(player && player->initWithPlayerType(type))
	{
		player->autorelease();
		return player;
	}
	else
	{
		delete player;
		player = NULL;
		return NULL;
	}
}


void Player::addAnimation()
{
	// check if already loaded
	auto animation = AnimationCache::getInstance()->getAnimation(String::createWithFormat("%s-%s",_name.c_str(),
						_animationNames[0])->getCString());
	if(animation)
		return;
	
	for(int i=0; i<_animationNum; i++)
	{
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.2);
		//put frames into animation
		for(int j = 0; j< _animationFrameNum[i] ; j++)
		{
			auto sfName =String::createWithFormat("%s-%d-%d.png",_name.c_str(), i+1, j+1)->getCString();
//			log(sfName);
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName));
		}
		// put the animation into cache
		AnimationCache::getInstance()->addAnimation(animation, String::createWithFormat("%s-%s",_name.c_str(), 
					_animationNames[i].c_str())->getCString());
	}


}


void Player::playAnimationForever(int index)
{
	if(index <0 || index >= _animationNum)
	{
		log("illegal animation index!");
		return;
	}
	auto str = String::createWithFormat("%s-%s",_name.c_str(), _animationNames[index].c_str())->getCString();
	auto animation = AnimationCache::getInstance()->getAnimation(str);
	auto animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
}