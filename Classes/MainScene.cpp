#include "MainScene.h"
#include "FSM.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "PauseLayer.h"
#include "GameOverLayer.h"

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

	initLevel();
	addRoles();
	addUI();
	addListener();
	addObserver();
	this->schedule(schedule_selector(MainScene::enemyMove), 3);

	this->scheduleUpdate();
//	_player->playAnimationForever(4);
    return true;
}

void MainScene::initLevel()
{
	_level = 0;
	_maxLevel = 2;
	std::vector<Player::PlayerType> types;
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	_enemyTypes.push_back(types);

	types.clear();
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	types.push_back(Player::ENEMY2);
	_enemyTypes.push_back(types);

	types.clear();
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	types.push_back(Player::BOSS);
	_enemyTypes.push_back(types);

	std::vector<Vec2> positions;
	positions.push_back(VisibleRect::center());
	positions.push_back(VisibleRect::right() - Vec2(200, 0));
	_enemyPositions.push_back(positions);

	positions.clear();
	positions.push_back(VisibleRect::center());
	positions.push_back(VisibleRect::right() - Vec2(200, 0) + Vec2(0, 200));
	positions.push_back(VisibleRect::right() - Vec2(200, 0) - Vec2(0, 200));
	_enemyPositions.push_back(positions);

	positions.clear();
	positions.push_back(VisibleRect::center() + Vec2(0, 200));
	positions.push_back(VisibleRect::center() - Vec2(0, 200));
	positions.push_back(VisibleRect::right() - Vec2(200, 0) + Vec2(0, 200));
	positions.push_back(VisibleRect::right() - Vec2(200, 0) - Vec2(0, 200));
	_enemyPositions.push_back(positions);

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
	if(_player)
	{
		_player->walkTo(pos);
	}
	
//	log("MainScene::onTouchBegan");
	return true;
}

void MainScene::onTouchPause(Ref* sender)
{
	Director::getInstance()->pause();
	auto layer = PauseLayer::create();
	this->addChild(layer,10000);
}

void MainScene::onTouchResume()
{
	Director::getInstance()->resume();
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
	auto playerA = dynamic_cast<Player*>(contact.getShapeA()->getBody()->getNode());
	auto playerB = dynamic_cast<Player*>(contact.getShapeB()->getBody()->getNode());
	auto typeA = playerA->getPlayerType();
	auto typeB = playerB->getPlayerType(); 
	if(typeA == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeB must belong to an enemy		
		log("contact enemy!");
		playerB->setCanAttack(true);
		playerA->addAttacker(playerB);
	}
	if(typeB == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeA must belong to an enemy		
		log("contact enemy!");
		playerA->setCanAttack(true);
		playerB->addAttacker(playerA);
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
		playerA->removeAttacker(playerB);
	}

	if(typeB == Player::PlayerType::PLAYER)
	{
		// only one player so ShapeA must belong to an enemy		
		log("leave enemy!");
		playerA->setCanAttack(false);
		playerB->removeAttacker(playerA);
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
	if(_player == nullptr)
	{
		log("player null");
		return;
	}
	if(enemy->isCanAttack())
	{
		if(_player->getState() != "attacking")
		{
			_player->attack();
			enemy->beHit(_player->getAttack());
		}

	}	
	else
	{
		_player->walkTo(enemy->getPosition());
	}
}

void MainScene::addRoles()
{
	//add player
	_player = Player::create(Player::PlayerType::PLAYER);
	_player->setPosition(VisibleRect::left().x + _player->getContentSize().width/2, VisibleRect::top().y/2);
	this->addChild(_player,10);
	addEnemyByLevel(0);
}

void MainScene::addEnemy()
{
	//add enemy1
	auto _enemy1 = Player::create(Player::PlayerType::ENEMY1);
	_enemy1->setPosition(VisibleRect::right().x - _player->getContentSize().width/2, VisibleRect::top().y/3);
	this->addChild(_enemy1,10);
	_enemys.pushBack(_enemy1);
	//add enemy2
	auto _enemy2 = Player::create(Player::PlayerType::ENEMY2);
	_enemy2->setPosition(VisibleRect::right().x*2/3 - _player->getContentSize().width/2, VisibleRect::top().y/2);
	this->addChild(_enemy2,10);
	_enemys.pushBack(_enemy2);
	//
	auto boss = Player::create(Player::PlayerType::BOSS);
	boss->setPosition(VisibleRect::right().x - _player->getContentSize().width/2, VisibleRect::top().y/3*2);
	this->addChild(boss,10);
	_enemys.pushBack(boss);
}

void MainScene::addUI()
{
	//backgound setup
	_background = Background::create();
	_background->setPosition(0,0);
	this->addChild(_background, -100);

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

	auto goItem = CustomTool::createMenuItemImage("go.png", "go.png", CC_CALLBACK_1(MainScene::gotoNextLevel,this));
	goItem->setVisible(false);
	goItem->setTag(2);
	goItem->setPosition(VisibleRect::right().x - goItem->getContentSize().width/2, VisibleRect::center().y);
	_menu = Menu::create(pauseItem, debugItem, goItem, NULL);
	_menu->setPosition(0,0);
	this->addChild(_menu, 20);

}

void MainScene::addListener()
{
	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

	_listener_contact = EventListenerPhysicsContact::create();
	_listener_contact->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin,this);
	_listener_contact->onContactSeperate = CC_CALLBACK_1(MainScene::onContactSeperate,this);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_contact, 10);
}

void MainScene::addObserver()
{

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::clickEnemy),"clickEnemy",nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::enemyDead),"enemyDead",nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::backgroundMoveEnd),"backgroundMoveEnd",nullptr);
}


void MainScene::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListener(_listener_touch);
	_eventDispatcher->removeEventListener(_listener_contact);
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void MainScene::gotoNextLevel(Ref* obj)
{
	auto goItem = this->_menu->getChildByTag(2);
	goItem->setVisible(false);
	goItem->stopAllActions();

	_background->move("left",_player);
}

void MainScene::enemyDead(Ref* obj)
{
	auto player= dynamic_cast<Player*>(obj);
	if(Player::PlayerType::PLAYER == player->getPlayerType())
	{
		_player = nullptr;
		auto layer = GameOverLayer::create();
		layer->setText("You Died!");
		this->addChild(layer,10000);
	}
	else
	{
		_enemys.eraseObject(player,true);
		log("onEnemyDead:%d", _enemys.size());
		if(_enemys.size() == 0)
		{
			if(_level == 2)
			{
				auto layer = GameOverLayer::create();
				layer->setText("You Win!");
				this->addChild(layer,10000);
				return;
			}
			showNextLevelItem();
		}
	}
}

void MainScene::backgroundMoveEnd(Ref* obj)
{
	_level++;
	addEnemyByLevel(_level);
	log("adding enemy...");
}

void MainScene::showNextLevelItem()
{
	auto goItem = this->_menu->getChildByTag(2);
	goItem->setVisible(true);
	goItem->runAction(RepeatForever::create(Blink::create(1,1)));
}

void MainScene::enemyMove(float dt)
{
	for(auto enemy: _enemys)
	{
		if("dead" != enemy->getState()&&_player && "dead" != _player->getState())
		{
			if(_player->isInRange(enemy))
			{
				if("attacking" != enemy->getState())
				{
					enemy->attack();
					_player->beHit(enemy->getAttack());
					this->updateHealth();
				}
			}else
			{
				enemy->walkTo(_player->getBestAttackPosition(enemy->getPosition()));
			}
		}
	}

}

void MainScene::updateHealth()
{
	if(_player)
	{
		auto health = _player->getHealth();
		auto maxHealth = _player->getMaxHealth();
		_progress->setProgress((float)health/maxHealth*100.0);
	}
}

void MainScene::update(float dt)
{
	if(_player)
		_player->setZOrder(VisibleRect::top().y - _player->getPosition().y);
	for(auto enemy : _enemys)
	{
		enemy->setZOrder(VisibleRect::top().y - enemy->getPosition().y);
	}
}

void MainScene::addEnemyByLevel(int level)
{
	if(level > _maxLevel)
	{
		return;
	}

	auto types = _enemyTypes[level];
	auto positions = _enemyPositions[level];

	for(int i=0; i< types.size();i++)
	{
		addOneEnemy(types[i], positions[i]);
	}
}

void MainScene::addOneEnemy(Player::PlayerType type,const Vec2& pos)
{
	auto enemy = Player::create(type);
	enemy->setPosition(pos);
	this->addChild(enemy,10);
	_enemys.pushBack(enemy);
}

