#ifndef __Progress__
#define __Progress__
#include "cocos2d.h"
USING_NS_CC;

class Progress : public Sprite
{
public:
	bool init(const char* background, const char* fillname);
	/*
	the inputs are SpriteFrame Names.
	they should be loaded into SpriteFrameCache before calling this.
	*/
	static Progress* create(const char* background, const char* fill);

	void setFill(ProgressTimer* fill){_fill=fill;}

	void setProgress(float percentage){_fill->setPercentage(percentage);}

private:
	ProgressTimer* _fill;
};
#endif