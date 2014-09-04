#include "StartScene.h"
#include "MainScene.h"
#include "VisibleRect.h"
#include "CustomTool.h"

bool StartScene::init()
{
	if(!Layer::init())
		return false;
	log("StartLayer::init");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/role.plist","image/role.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/ui.plist","image/ui.pvr.ccz");
	auto background = Sprite::create("image/start-bg.jpg");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto item = CustomTool::createMenuItemImage("start1.png", "start2.png", CC_CALLBACK_1(StartScene::onStart,this));
	auto menu = Menu::createWithItem(item);
	this->addChild(menu);
	return true;
}


Scene* StartScene::createScene()
{
    auto scene = Scene::create();   
    auto layer = StartScene::create();
    scene->addChild(layer);
    return scene;
}


void StartScene::onStart(Ref* obj)
{
	log("StartLayer::onStart");
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}