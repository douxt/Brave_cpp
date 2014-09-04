#include "MainScene.h"
#include "FSM.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "PauseLayer.h"

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    

	//load frames into cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/role.plist","image/role.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/ui.plist","image/ui.pvr.ccz");

	//load background
	Sprite* background = Sprite::create("image/background.png");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	//add player
	_player = Player::create(Player::PlayerType::PLAYER);
	_player->setPosition(VisibleRect::left().x + _player->getContentSize().width/2, VisibleRect::top().y/2);
	this->addChild(_player);


	//add enemy1
	_enemy1 = Player::create(Player::PlayerType::ENEMY1);
	_enemy1->setPosition(VisibleRect::right().x - _player->getContentSize().width/2, VisibleRect::top().y/2);
	this->addChild(_enemy1);

	//add enemy2
	_enemy2 = Player::create(Player::PlayerType::ENEMY2);
	_enemy2->setPosition(VisibleRect::right().x*2/3 - _player->getContentSize().width/2, VisibleRect::top().y/2);
	this->addChild(_enemy2);
	//test animation
//	_player->playAnimationForever(1);
	_enemy1->playAnimationForever(1);
	_enemy2->playAnimationForever(1);

	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

//	auto fsm = FSM::create("idle",[](){cocos2d::log("Enter idle");});

	_progress = Progress::create("player-progress-bg.png","player-progress-fill.png");
	_progress->setPosition(VisibleRect::left().x + _progress->getContentSize().width/2, VisibleRect::top().y - _progress->getContentSize().height/2);
	this->addChild(_progress);

	auto pauseItem = CustomTool::createMenuItemImage("pause1.png", "pause2.png", CC_CALLBACK_1(MainScene::onTouchPause,this));
	pauseItem->setTag(1);
	pauseItem->setPosition(VisibleRect::right().x - pauseItem->getContentSize().width/2, 
							VisibleRect::top().y - pauseItem->getContentSize().height/2);

	_menu = Menu::create(pauseItem, NULL);
	_menu->setPosition(0,0);
	this->addChild(_menu, 20);

    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
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

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 pos = this->convertToNodeSpace(touch->getLocation());
	_player->walkTo(pos);
//	log("MainScene::onTouchBegan");
	return true;
}

void MainScene::onTouchPause(Ref* sender)
{
	_player->pause();
	_enemy1->pause();
	_enemy2->pause();
	auto layer = PauseLayer::create();
	this->addChild(layer);
}

void MainScene::onTouchResume()
{
	_player->resume();
	_enemy1->resume();
	_enemy2->resume();
}