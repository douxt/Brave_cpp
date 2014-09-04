#ifndef __PauseLayer__
#define __PauseLayer__
#include "cocos2d.h"

USING_NS_CC;

class PauseLayer : public LayerColor
{
public:
	bool init();
	CREATE_FUNC(PauseLayer);

	void addUI();
	void addTouch();
	void home(Ref* obj);
	void back(Ref* obj);
private:
	EventListenerTouchOneByOne* _listener;
};

#endif