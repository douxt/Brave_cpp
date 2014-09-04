#include "MainScene.h"
#include "FSM.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "PauseLayer.h"

Scene* MainScene::createScene()
{
    // init with physics
    auto scene = Scene::createWithPhysics();
    auto layer = MainScene::create();
	//set physics world
	layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
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
	//_enemy1->playAnimationForever(1);
	//_enemy2->playAnimationForever(1);

	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

	_listener_contact = EventListenerPhysicsContact::create();
	_listener_contact->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin,this);
	_listener_contact->onContactSeperate = CC_CALLBACK_1(MainScene::onContactSeperate,this);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_contact, 10);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::clickEnemy),"clickEnemy",nullptr);

//	auto fsm = FSM::create("idle",[](){cocos2d::log("Enter idle");});

	_progress = Progress::create("player-progress-bg.png","player-progress-fill.png");
	_progress->setPosition(VisibleRect::left().x + _progress->getContentSize().width/2, VisibleRect::top().y - _progress->getContentSize().height/2);
	this->addChild(_progress);

	auto pauseItem = CustomTool::createMenuItemImage("pause1.png", "pause2.png", CC_CALLBACK_1(MainScene::onTouchPause,this));
	pauseItem->setTag(1);
	pauseItem->setPosition(VisibleRect::right().x - pauseItem->getContentSize().width/2, 
							VisibleRect::top().y - pauseItem->getContentSize().height/2);

	auto debugItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
										CC_CALLBACK_1(MainScene::toggleDebug, this));
    debugItem->setScale(2.0);
	debugItem->setPosition(Vec2(VisibleRect::right().x - debugItem->getContentSize().width - pauseItem->getContentSize().width ,
		VisibleRect::top().y - debugItem->getContentSize().height));

	_menu = Menu::create(pauseItem, debugItem, NULL);
	_menu->setPosition(0,0);
	this->addChild(_menu);

    return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	// set gravity to zero
	_world->setGravity(Vec2(0, 0));
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
	this->addChild(layer,100);
}

void MainScene::onTouchResume()
{
	_player->resume();
	_enemy1->resume();
	_enemy2->resume();
}

void MainScene::toggleDebug(Ref* pSender)
{
	if(_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_NONE)
	{
		_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
	else
	{
		_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}
		
}

bool MainScene::onContactBegin(const PhysicsContact& contact)
{
	auto playerA = (Player*)contact.getShapeA()->getBody()->getNode();
	auto playerB = (Player*)contact.getShapeB()->getBody()->getNode();
	auto typeA = playerA->getPlayerType();
	auto typeB = playerB->getPlayerType(); 
	if(typeA == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeB must belong to an enemy		
		log("contact enemy!");
		playerB->setCanAttack(true);
	}
	if(typeB == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeA must belong to an enemy		
		log("contact enemy!");
		playerA->setCanAttack(true);
	}
	return true;
}

void MainScene::onContactSeperate(const PhysicsContact& contact)
{
	auto playerA = (Player*)contact.getShapeA()->getBody()->getNode();
	auto playerB = (Player*)contact.getShapeB()->getBody()->getNode();
	auto typeA = playerA->getPlayerType();
	auto typeB = playerB->getPlayerType(); 
	if(typeA == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeB must belong to an enemy		
		log("leave enemy!");
		playerB->setCanAttack(false);
	}

	if(typeB == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeA must belong to an enemy		
		log("leave enemy!");
		playerA->setCanAttack(false);
	}
}

void MainScene::clickEnemy(Ref* obj)
{
	log("click enemy message received!");
	auto enemy = (Player*)obj;
	if(enemy == nullptr)
	{
		log("enemy null");
		return;
	}
	if(enemy->isCanAttack())
	{
		_player->attack();
		enemy->beHit(_player->getAttack());
	}	
	else
	{
		_player->walkTo(enemy->getPosition());
	}
}