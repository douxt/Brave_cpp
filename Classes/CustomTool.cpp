#include "CustomTool.h"

CustomTool* CustomTool::_tool;

CustomTool* CustomTool::getInstance()
{
	if(!_tool)
	{
		_tool = new CustomTool();
	}

	return _tool;
}

MenuItemImage* CustomTool::createMenuItemImage(const char* normal, const char* selected, ccMenuCallback callback)
{
	auto item = MenuItemImage::create();
	auto nsf = SpriteFrameCache::getInstance()->getSpriteFrameByName(normal);
	auto ssf = SpriteFrameCache::getInstance()->getSpriteFrameByName(selected);
	item->setNormalSpriteFrame(nsf);
	item->setSelectedSpriteFrame(ssf);
	item->setCallback(callback);
	return item;
}