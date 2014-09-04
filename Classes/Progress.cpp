#include "Progress.h"

bool Progress::init(const char* background, const char* fillname)
{
	this->initWithSpriteFrameName(background);
	ProgressTimer* fill = ProgressTimer::create(Sprite::createWithSpriteFrameName(fillname));
	this->setFill(fill);
	this->addChild(fill);

	fill->setType(ProgressTimer::Type::BAR);
	fill->setMidpoint(Point(0,0.5));
	fill->setBarChangeRate(Point(1.0, 0));
	fill->setPosition(this->getContentSize()/2);
	fill->setPercentage(100);
	return true;
}

Progress* Progress::create(const char* background, const char* fillname)
{
	Progress* progress = new Progress();
	if(progress && progress->init(background,fillname))
	{
		progress->autorelease();
		return progress;
	}
	else
	{
		delete progress;
		progress = NULL;
		return NULL;
	}
}