#include "Player.h"


bool Player::initWithPlayerType(PlayerType type)
{
	std::string sfName = "";
	_type = type;
	_speed = 100;
	_isShowBar = true;
	_isCanAttack =false;
	_health = 100;
	_maxHealth =100;
	_attack = 100;
	_flip = false;
	int animationFrameNum[5] ={4, 4, 4, 2, 4};
	int animationFrameNum2[5] ={3, 3, 3, 2, 0};
	int animationFrameNum3[5] ={1, 5, 4, 2, 0};

	auto size = this->getContentSize();
	auto body = PhysicsBody::create();

	//setup according to PlayerType
	switch(type)
	{
	case PlayerType::PLAYER:
		sfName = "player1-1-1.png";
		_name = "player1";
		_animationNum = 5;
		_animationFrameNum.assign(animationFrameNum, animationFrameNum + 5);
		_speed = 125;
		_isShowBar = false;

		_flip = true;
//		this->setAnchorPoint(Vec2(0, 0));

		break;
	case PlayerType::ENEMY1:
		sfName = "enemy1-1-1.png";
		_name = "enemy1";
		_animationNum = 4;
		_animationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
//		this->setAnchorPoint(Vec2(200, 72));
		_attack = 8;
		break;
	case PlayerType::ENEMY2:
		sfName = "enemy2-1-1.png";
		_name = "enemy2";
		_animationNum = 4;
		_animationFrameNum.assign(animationFrameNum2, animationFrameNum2 + 5);
//		this->setAnchorPoint(Vec2(200, 75));
		_attack = 8;
		break;
	case PlayerType::BOSS:
		sfName = "df1-1-1.png";
		_name = "df1";
		_animationNum = 4;
		_animationFrameNum.assign(animationFrameNum3, animationFrameNum3 + 5);
//		this->setAnchorPoint(Vec2(200, 75));
		_attack = 33;
		break;
	}
	this->initWithSpriteFrameName(sfName);
	//auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName);
	//auto texture = sf->getTexture();
	//this->initWithTexture(texture, Rect(0, 38, 247, 163));
	std::string animationNames[] = {"walk", "attack", "dead", "hit", "skill"};
	_animationNames.assign(animationNames, animationNames + 5);
	//load animation
	this->addAnimation();

	size = this->getContentSize();
	log("size: %f, %f ",size.width, size.height);
	body->addShape(PhysicsShapeBox::create(Size(size.width/3, size.height/2)));
	body->setCategoryBitmask(2);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(1);

	_flipped = _flip;
	if(type == PlayerType::PLAYER)
	{
		body->setCategoryBitmask(1);
		body->setContactTestBitmask(2);
		_flipped = !_flip;
	}

	this->setPhysicsBody(body);



	this->initFSM();


	_progress = Progress::create("small-enemy-progress-bg.png","small-enemy-progress-fill.png");
//	_progress->setPosition( size.width*2/3, size.height + _progress->getContentSize().height/2);
	_progress->setPosition( 200 , 200);
	this->addChild(_progress);
	if(!_isShowBar)
	{
		_progress->setVisible(false);
	}

	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(Player::onTouch,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener,this);

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
		animation->setDelayPerUnit(0.2f);
		//put frames into animation
		for(int j = 0; j< _animationFrameNum[i] ; j++)
		{
			auto sfName =String::createWithFormat("%s-%d-%d.png",_name.c_str(), i+1, j+1)->getCString();
//			log(sfName);
			auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName);
//			auto texture = sf->getTexture();
//			auto offset = sf->getOffset();
//			auto rect = sf->getRect();
//			sf = SpriteFrame::createWithTexture(texture, Rect(0 + rect.getMinX(), 38 + rect.getMinY(), 247, 163), true, offset, Size(247,163)); 
////			this->initWithTexture(texture, Rect(0, 38, 247, 163));
			animation->addSpriteFrame(sf);
		}
		// put the animation into cache
		AnimationCache::getInstance()->addAnimation(animation, String::createWithFormat("%s-%s",_name.c_str(), 
					_animationNames[i].c_str())->getCString());
	}
}


void Player::playAnimationForever(int index)
{
	auto act = this->getActionByTag(index);
	if(act)
		return;

	for(int i=0;i<5;i++)
	{
		this->stopActionByTag(i);
	}
	if(index <0 || index >= _animationNum)
	{
		log("illegal animation index!");
		return;
	}
	auto str = String::createWithFormat("%s-%s",_name.c_str(), _animationNames[index].c_str())->getCString();
	auto animation = AnimationCache::getInstance()->getAnimation(str);
	auto animate = RepeatForever::create(Animate::create(animation));
	animate->setTag(index);
	this->runAction(animate);
}

Animate* Player::getAnimateByType(AnimationType type)
{
	if(type <0 || type >= _animationNum)
	{
		log("illegal animation index!");
		return nullptr;
	}
	auto str = String::createWithFormat("%s-%s",_name.c_str(), _animationNames[type].c_str())->getCString();
	auto animation = AnimationCache::getInstance()->getAnimation(str);
	auto animate = Animate::create(animation);
	animate->setTag(type);
	return animate;
}


void Player::initFSM()
{
	_fsm = FSM::create("idle");
	_fsm->retain();
	auto onIdle =[&]()
	{
		log("onIdle: Enter idle");
		this->stopActionByTag(WALKING);
		auto sfName = String::createWithFormat("%s-1-1.png", _name.c_str());
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sfName->getCString());
		this->setSpriteFrame(spriteFrame);
	};
	_fsm->setOnEnter("idle",onIdle);

	auto onAttacking =[&]()
	{
		log("onAttacking: Enter Attacking");
		auto animate = getAnimateByType(ATTACKING);
		auto func = [&]()
		{
			this->_fsm->doEvent("stop");
		};
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(animate, callback, nullptr);
		this->runAction(seq);
	};
	_fsm->setOnEnter("attacking",onAttacking);
	
	auto onBeingHit = [&]()
	{
		log("onBeingHit: Enter BeingHit");
		auto animate = getAnimateByType(BEINGHIT);
		auto func = [&]()
		{
			this->_fsm->doEvent("stop");
		};
		auto wait = DelayTime::create(0.6f);
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(wait,animate, callback, nullptr);
		this->runAction(seq);
	};
	_fsm->setOnEnter("beingHit",onBeingHit);

	auto onDead = [&]()
	{
		this->setCanAttack(false);
		log("onDead: Enter Dead");
		auto animate = getAnimateByType(DEAD);
		auto func = [&]()
		{
			log("A charactor died!");
			NotificationCenter::getInstance()->postNotification("enemyDead",this);
			this->removeFromParentAndCleanup(true);
		};
		auto blink = Blink::create(3,5);
		auto callback = CallFunc::create(func);
		auto seq = Sequence::create(animate, blink, callback, nullptr);
		this->stopAllActions();
		this->runAction(seq);
		_progress->setVisible(false);
	};
	_fsm->setOnEnter("dead",onDead);
}



void Player::walkTo(Vec2 dest)
{
	std::function<void()> onWalk = CC_CALLBACK_0(Player::onWalk, this, dest);
	_fsm->setOnEnter("walking", onWalk);
	_fsm->doEvent("walk");
}


void Player::onWalk(Vec2 dest)
{
	log("onIdle: Enter walk");
	this->stopActionByTag(WALKTO_TAG);
	auto curPos = this->getPosition();

	if(curPos.x > dest.x)
	{
		if(_flipped != _flip)
		{
			this->setFlippedX(_flip);
			_flipped = _flip;
			this->setPosition(curPos.x - 88, curPos.y);
			this->moveProgress(88);
		}	
	}
	else
	{		
		if(_flipped == _flip)
		{
			this->setFlippedX(!_flip);
			_flipped = !_flip;
			this->setPosition(curPos.x + 88, curPos.y);
			this->moveProgress(-88);
		}	
	}

	auto diff = dest - curPos;
	auto time = diff.getLength()/_speed;
	auto move = MoveTo::create(time, dest);
	auto func = [&]()
	{
		this->_fsm->doEvent("stop");
	};
	auto callback = CallFunc::create(func);
	auto seq = Sequence::create(move, callback, nullptr);
	seq->setTag(WALKTO_TAG);
	this->runAction(seq);
	this->playAnimationForever(WALKING);
}

void Player::onExit()
{
	Sprite::onExit();
	_fsm->release();
	_eventDispatcher->removeEventListener(_listener);
}

bool Player::onTouch(Touch* touch, Event* event)
{
	auto pos = this->convertToNodeSpace(touch->getLocation());
	log("Touching: %f, %f...", pos.x, pos.y);
	if(_type == PLAYER)
		return false;

	log("Player: touch detected!");
	auto size = this->getContentSize();
	auto rect = Rect(size.width/2, 0, size.width, size.height);
	if(rect.containsPoint(pos))
	{
		NotificationCenter::getInstance()->postNotification("clickEnemy",this);
		log("enemy touched!");
		return true;
	}
	log("enemy not touched!");
	return false;
}

void Player::attack()
{
	_fsm->doEvent("attack");
}

void Player::beHit(int attack)
{
	_health -= attack;
	if(_health <= 0)
	{
		_health = 0;
		this->_progress->setProgress((float)_health/_maxHealth*100);
		_fsm->doEvent("die");
		if(_type == PlayerType::PLAYER)
		{

		}
		return;
	}
	else
	{
		this->_progress->setProgress((float)_health/_maxHealth*100);
		_fsm->doEvent("beHit");
	}
}

void Player::addAttacker(Player* attacker)
{
	_attackers.pushBack(attacker);
}

void Player::removeAttacker(Player* attacker)
{
	_attackers.eraseObject(attacker);
}

bool Player::isInRange(Player* enemy)
{
	return _attackers.contains(enemy);
}

void Player::moveProgress(float dx)
{
	auto curPos = _progress->getPosition();
	_progress->setPosition(curPos.x + dx, curPos.y);
}

Vec2 Player::getBestAttackPosition(const Vec2& pos)
{
	auto curPos = this->getPosition();
	auto pos1 = curPos + Vec2(50, 0);
	auto pos2 = curPos - Vec2(50, 0);
	auto diff1 = pos1 - pos;
	auto diff2 = pos2 - pos;
	return diff1.length()>diff2.length()?pos2:pos1;
}