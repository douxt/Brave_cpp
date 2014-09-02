#include "HelloWorldScene.h"
#include "Player.h"

USING_NS_CC;

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
	Player* player = Player::create(Player::PlayerType::PLAYER);
	player->setPosition(origin.x + player->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(player);


	//add enemy1
	Player* enemy1 = Player::create(Player::PlayerType::ENEMY1);
	enemy1->setPosition(origin.x + visibleSize.width - player->getContentSize().width/2, origin.y + visibleSize.height/2);
	this->addChild(enemy1);

	//test animation
	player->playAnimationForever(1);
	enemy1->playAnimationForever(1);

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
