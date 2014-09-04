#include "Background.h"
#include "VisibleRect.h"


bool Background::init()
{
	_spriteA = Sprite::create("image/background.png");
	_spriteB = Sprite::create("image/background2.png");
	
	_spriteA->setPosition(VisibleRect::center());
	_spriteB->setPosition(VisibleRect::center() + VisibleRect::right() - VisibleRect::left());
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
	auto pos = VisibleRect::center() + VisibleRect::right() - VisibleRect::left();

	if(ax.x < VisibleRect::left().x)
	{
		_spriteA->setPosition(pos);
	}		
	else
	{
		_spriteB->setPosition(pos);
	}
		

	NotificationCenter::getInstance()->postNotification("backgroundMoveEnd");
}