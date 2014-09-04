#ifndef __MainScene__
#define __MainScene__

#include "cocos2d.h"
#include "Player.h"
#include "Progress.h"

USING_NS_CC;

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

	bool onTouchBegan(Touch* touch, Event* event);

	void onTouchPause(Ref* sender);
	void onTouchResume();

private:
	Player* _player;
	Player* _enemy1;
	Player* _enemy2;
	EventListenerTouchOneByOne* _listener_touch;
	Progress* _progress;
	Menu* _menu;
};

#endif // __MainScene__
