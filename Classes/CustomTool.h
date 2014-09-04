#ifndef __CustomTool__
#define __CustomTool__
#include "cocos2d.h"

USING_NS_CC;

class CustomTool : public Ref
{
public:
	static CustomTool* getInstance();
	/*
	create MenueItemImage with SpriteFrameName
	*/
	static MenuItemImage* createMenuItemImage(const char* normal, const char* selected, ccMenuCallback callback);
private:
	static CustomTool* _tool;
};

#endif