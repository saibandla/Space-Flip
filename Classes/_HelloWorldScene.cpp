#include "_HelloWorldScene.h"
#include "SKGameMetadata.h"
#include <spine/spine.h>
USING_NS_CC;

CCScene* SKMainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SKMainMenu *layer = SKMainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SKMainMenu::init()
{
#ifdef SK_PAID
	CCLOG("PAID version of the Game");
#else
	CCLOG("FREE version of the Game");
#endif
	
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(SKMainMenu::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);


//	spine::Atlas *atlas = spine::Atlas::create("data/spineboy.atlas");
//
//	spine::SkeletonJson json(atlas);
//	json.scale = 0.6f; // note : this is how you scale the animation
//
//	spine::SkeletonData *skeletonData = json.readSkeletonDataFile("data/spineboy-skeleton.json");
//
//	spine::Animation *animation = json.readAnimationFile("data/spineboy-walk.json", skeletonData);
//
//	spine::CCSkeleton* skeletonNode = new spine::CCSkeleton(skeletonData);
//	skeletonNode->state->setAnimation(animation, true);
////
//	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//	skeletonNode->setPosition(ccp(winSize.width / 2, 20));
//	addChild(skeletonNode, 100);
//
    return true;
}

void SKMainMenu::on_take_photo(cocos2d::CCImage * photo)
{
	if (photo)
	{
		CCTexture2D * t = new CCTexture2D();
		bool ok = t->initWithImage(photo);
		CCAssert(ok, "You must restric maximum side of texture so this converion work");
		t->retain();
		CCSprite * s = CCSprite::createWithTexture(t);
		addChild(s);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		s->setPosition(CCPoint(winSize.width / 2, winSize.height /  2));
	}
	else
	{
		
	}
}

void SKMainMenu::menuCloseCallback(CCObject* pSender)
{
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (sk::game_services::device_can_take_photo())
	{
		CCMessageBox("Camera is supported", "Wow");
		return;
	}	
#endif
	
//	sk::game_services::take_photo_info info;
////	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
////	info.width = winSize.width / 3;
////	info.height = winSize.height / 3;
//	if (sk::game_services::take_photo(this, info))
//	{
//
//	}
//	else
//	{
//		CCMessageBox("Camera is not supported", "Error");
//	}
}
