#include "HelloWorldScene.h"



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//load frames into cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/role.plist","image/role.pvr.ccz");

	//load background
	Sprite* background = Sprite::create("image/background.png");
	background->setPosition(origin + visibleSize/2);
	this->addChild(background);

	//add player
	_player = Player::create(Player::PlayerType::PLAYER);
	_player->setPosition(origin.x + _player->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(_player);


	//add enemy1
	_enemy1 = Player::create(Player::PlayerType::ENEMY1);
	_enemy1->setPosition(origin.x + visibleSize.width - _player->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(_enemy1);

	//test animation
//	_player->playAnimationForever(1);
	_enemy1->playAnimationForever(1);

	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 pos = this->convertToNodeSpace(touch->getLocation());
	_player->walkTo(pos);
	log("HelloWorld::onTouchBegan");
	return true;
}