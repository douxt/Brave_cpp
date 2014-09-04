#ifndef __StartScene__
#define __StartScene__
#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Layer
{
public:
	bool init();
	static Scene* createScene();
	void onStart(Ref* obj);
	CREATE_FUNC(StartScene);
};

#endif