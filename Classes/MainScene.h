#ifndef __MainScene__
#define __MainScene__

#include "cocos2d.h"
#include "Player.h"
#include "Progress.h"
#include "Background.h"

USING_NS_CC;

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

	bool onTouchBegan(Touch* touch, Event* event);

	void onTouchPause(Ref* sender);

	void onTouchResume();

	void setPhysicsWorld(PhysicsWorld* physicsWorld){_world=physicsWorld;}
	
	void toggleDebug(Ref* pSender);

	void onEnter();

	bool onContactBegin(const PhysicsContact& contact);

	void onContactSeperate(const PhysicsContact& contact);

	void clickEnemy(Ref* obj);

	void addRoles();

	void addUI();

	void addListener();

	void addObserver();

	void onExit();

	void gotoNextLevel(Ref* obj);

	void enemyDead(Ref* obj);

	void backgroundMoveEnd(Ref* obj);

	void showNextLevelItem();

	void enemyMove(float dt);

	void addEnemy();
	void addEnemyByLevel(int level);

	void updateHealth();

	void update(float dt);

	void initLevel();

	void addOneEnemy(Player::PlayerType type,const Vec2& pos);
private:
	Player* _player;
	//Player* _enemy1;
	//Player* _enemy2;
	EventListenerTouchOneByOne* _listener_touch;
	EventListenerPhysicsContact* _listener_contact;
	Progress* _progress;
	Menu* _menu;
	PhysicsWorld* _world;
	Background* _background;
	Vector<Player*> _enemys;
	int _maxLevel;
	int _level;
	std::vector<std::vector<Player::PlayerType>> _enemyTypes;
	std::vector<std::vector<Vec2>> _enemyPositions;
};

#endif // __MainScene__
