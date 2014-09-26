#ifndef __GameOverLayer__
#define __GameOverLayer__
#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(GameOverLayer);

	void home(Ref* obj);
	void setText(const std::string& text);
private:
	LabelTTF* _label;
};


#endif