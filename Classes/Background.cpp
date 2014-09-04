#include "Background.h"
#include <string>



bool Background::init()
{
	_spriteA = Sprite::create("image/background.png");
	_spriteB = Sprite::create("image/background2.png");
	
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto vSize = Director::getInstance()->getVisibleSize();
	_spriteA->setPosition(origin + Vec2(vSize.width/2, vSize.height/2));
	_spriteB->setPosition(origin + Vec2(vSize.width/2, vSize.height/2) + Vec2(vSize.width, 0));
	this->addChild(_spriteA,-10);
	this->addChild(_spriteB,-20);

	_isMoving = false;
	return true;
}

void Background::move(const char* direction, Sprite* withSprite)
{
	if(this->_isMoving)
	{
		log("Moving...");
		return;
	}
	auto vSize = Director::getInstance()->getVisibleSize();
	float movex = 0;
	if(std::string(direction)=="left")
		movex = -vSize.width;
	else
		movex = vSize.width;

	auto seq = Sequence::create(MoveBy::create( 2.0, Vec2( movex, 0)),
								CallFunc::create(CC_CALLBACK_0(Background::onMoveEnd,this)),
								nullptr);
	_spriteA->runAction(MoveBy::create( 2.0, Vec2( movex, 0)));
	_spriteB->runAction(seq);
	log("Moving...2");
	if(withSprite)
		withSprite->runAction(MoveBy::create( 2.0, Vec2( movex, 0)));

	_isMoving = true;

}

void Background::onMoveEnd()
{
	_isMoving = false;
	auto ax = _spriteA->getPosition();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto vSize = Director::getInstance()->getVisibleSize();
	if(ax.x < origin.x)
	{
		_spriteA->setPosition( Vec2(vSize.width/2, vSize.height/2) + Vec2(vSize.width, 0));
	}		
	else
	{
		_spriteB->setPosition( Vec2(vSize.width/2, vSize.height/2) + Vec2(vSize.width, 0));
	}
		

	NotificationCenter::getInstance()->postNotification("backgroundMoveEnd");
}