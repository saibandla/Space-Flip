#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "platform/CCFileUtils.h"
#include "SKGameMetadata.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#include <bps/bps.h>
#include <bps/dialog.h>

#include <bps/paymentservice.h>
#include <bps/navigator.h>
#include <bps/navigator_invoke.h>
#define TRACK_NAME "track.wav"
#define SOUND_NAME "sound.wav"
#else

#define TRACK_NAME "track.mp3"
#define SOUND_NAME "sound.mp3"
#endif

using namespace cocos2d;
using namespace CocosDenshion;
int mainmenu=0;

#define _W CCDirector::sharedDirector()->getWinSize().width
#define _H CCDirector::sharedDirector()->getWinSize().height

#define _NAME(x) cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename( x ).c_str()

#define MAIN_MENU_TIME 1.05
#define TILE_MOVE_TIME 0.55


#define LEVEL_FIX 1

#define TOTAL_LEVELS 40

float KK = 1.0f;
float TSCALE = 1.0f;
static HelloWorld * cb = NULL;
// com.dracollc.spaceflip.unlockall
bool m_isWindowActive;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)

class CCPurchaseScene : public CCScene
{
public:

    static CCScene *create(void)
    {
    	return new CCPurchaseScene();
    }

    virtual void on_purchase_ok        (const char * sku, unsigned  request_id)
    {
    	if (cb)
    	{
    		cb->on_purchase_ok(sku, request_id);
    	}
    }
    virtual void on_purchase_restore_ok(const char * sku)
    {
    	if (cb)
    	{
    		cb->on_purchase_restore_ok(sku);
    	}
    }
    virtual void on_purchase_failed    (const char * sku, unsigned  request_id)
    {
    	if (cb)
    	{
    		cb->on_purchase_failed(sku, request_id);
    	}
    }
};
#endif

CCScene* HelloWorld::scene() {
	// 'scene' is an autorelease object
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
	CCScene *scene = CCPurchaseScene::create();
#else
	CCScene *scene = CCScene::create();
#endif

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance

void HelloWorld::toggleSound() {
	setSound(!isSound());
}

void HelloWorld::toggleMusic() {

	setMusic(!isMusic());
	if (isMusic()) {
		if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0);
		} else {
			std::string path =TRACK_NAME;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
					path.c_str(), true);
		}
	} else {
		if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0);
		}
	}
}

void HelloWorld::setMusic(bool on) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey("music", on ? 1 : 0);
	CCUserDefault::sharedUserDefault()->flush();
}

void HelloWorld::setSound(bool on) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey("sound", on ? 1 : 0);
	CCUserDefault::sharedUserDefault()->flush();
}

bool HelloWorld::isMusic() {
	return 0 != CCUserDefault::sharedUserDefault()->getIntegerForKey("music", 1);
}

bool HelloWorld::isSound() {
	return 0 != CCUserDefault::sharedUserDefault()->getIntegerForKey("sound", 1);
}

void HelloWorld::do_facebook()
{
	char buffer[256];
	sprintf(buffer, "Bet you can’t beat my score on Space Flip: %d points. Try it if you dare.", getTotalScore());
	sk::game_services::share_game_score_facebook(buffer);
}

void HelloWorld::facebook()
{
	playSound();
	sk::game_services::open_url("http://www.facebook.com/SpaceFlipGame");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	sk::game_services::open_url("http://www.facebook.com/SpaceFlipGame");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char buffer[256];
	sprintf(buffer, "\"Bet you can’t beat my score on Space Flip: %d points. Try it if you dare.\"", getTotalScore());
	sk::game_services::open_yes_no_dialog(DIALOG_CONFIRM_FACEBOOK, this, buffer, "Publish it on Facebook?", "Publish", "Cancel");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	do_facebook();
#else
	
#endif
}

void HelloWorld::twitter()
{
	playSound();
	sk::game_services::open_url("http://twitter.com/SpaceFlipGame");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	sk::game_services::open_url("http://twitter.com/SpaceFlipGame");
#else
//	char buffer[256];
//	sprintf(buffer, "Bet you can’t beat my score on Space Flip: %d points. Try it if you dare.", getTotalScore());
//	sk::game_services::share_game_score_twitter(buffer);
#endif
}

void HelloWorld::level() {

	if (showSolution)
	{
		return;
	}
	if (!levelBtn->isVisible()
			|| CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
					levelBtn) != 0) {
		return;
	}

	for (int i = 0; i < stiles.size(); ++i) {
		if (stiles[i]) {
			if (CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
					stiles[i]) != 0) {
				stiles[i]->setVisible(false);
			}
		}
	}
	initDelays();
	tilesUp = true;

	playSound();
	bool hide = false;

	float d1 = !hide ? 100 : -100;
	GLubyte op2 = !hide ? 56 : 150;

	levelBtn->runAction(CCEaseBackOut::create(
			CCMoveTo::create(MAIN_MENU_TIME,
							 ccpAdd(
					levelBtn->getPosition(), CCPoint(1.5 * d1, 0))
					)));
	levelBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	levelBtn->setVisible(gs == GS_Gameplay);
	
	
	giftBtn->runAction(
						CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
										 ccpAdd(giftBtn->getPosition() , CCPoint(1.5 * d1, 0))
										 )));
	giftBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	giftBtn->setVisible(gs == GS_Gameplay);
	
	

	infoBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(infoBtn->getPosition() , CCPoint(-1.5 * d1, 0))
					))); 
	infoBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	infoBtn->setVisible(gs == GS_Gameplay);
	
	
	
	
	
	solutionBtn->runAction(
					   CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
										ccpAdd(solutionBtn->getPosition(), CCPoint(-1.5 * d1, 0))
										))); 
	solutionBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	solutionBtn->setVisible(gs == GS_Gameplay);
	
	

	labelStat->runAction(CCFadeTo::create(MAIN_MENU_TIME, 0));

	runWithDelay(MAIN_MENU_TIME, callfunc_selector(HelloWorld::levelsDown));

	replayBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, 0));

}

void HelloWorld::levelsDown() {
	setLevels();
	addSprites();
	initDelays();
	tilesUp = false;
	labelStat->runAction(CCFadeTo::create(MAIN_MENU_TIME, 150));

	char buffer[32] = { 0 };
	sprintf(buffer, "Total score: %d", getTotalScore());
	labelStat->setString(buffer);
#ifndef NO_IN_APPS
	labelUnlockOrRestore->setString("Unlock levels or restore purchase");
	labelUnlockOrRestore->runAction(CCFadeTo::create(MAIN_MENU_TIME, 150));
#endif
}

void HelloWorld::solution()
{

	if (showSolution)
	{
		return;
	}
	int h = getHints();
	if(h==0)
	{
		HelloWorld::gift();
		return;
	}
	playSound();
	if (0 == h)
	{
		sk::game_services::buy_product(INAPP_HINT_PACK_GAME_ID);
		return;
	}
	setHints(h - 1);
	updateHints();
	showSolution = true;
	if (!solutionBtn->isVisible()
		|| CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
																							solutionBtn) != 0) {
			return;
		}
	labelLevel->setString("Solution:");
	startShowLevel();
}

void HelloWorld::info() {
	if (showSolution)
	{
		return;
	}
	if (!infoBtn->isVisible()
			|| CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
					infoBtn) != 0) {
		return;
	}
	playSound();
	labelLevel->setString("Make all tiles white.");
	startShowLevel();

}

void HelloWorld::pro()
{
	playSound();
	sk::game_services::open_url("http://appworld.blackberry.com/webstore/vendor/59434/");
#ifndef SK_PAID	
	sk::game_services::open_url("http://appworld.blackberry.com/webstore/vendor/59434/");
#endif
}

bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}
	cb = this;
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	showSolution = false;
	KK = _W < 715 ? 0.5f : 1.0f;
	closeBtn = CCMenuItemImage::create("back.png", "back.png", this,
			menu_selector(HelloWorld::menuCloseCallback));

	closeBtn->setOpacity(0);
	closeBtn->setPosition(ccp(75 * KK, _H - 50 * KK));

	closeBtn->setScale(KK);

	replayBtn = CCMenuItemImage::create("refresh.png", "refresh.png", this,
			menu_selector(HelloWorld::replay));

	levelBtn = CCMenuItemImage::create("list.png", "list.png", this,
			menu_selector(HelloWorld::level));
	
	giftBtn = CCMenuItemImage::create("gift.png", "gift.png", this,
									   menu_selector(HelloWorld::gift1));
	

	infoBtn = CCMenuItemImage::create("info.png", "info.png", this,
			menu_selector(HelloWorld::info));

	infoBtn->setOpacity(0);
	infoBtn->setScale(KK);
	

	solutionBtn = CCMenuItemImage::create("help.png", "help.png", this,menu_selector(HelloWorld::solution));
	


	solutionBtn->setOpacity(0);
	solutionBtn->setScale(KK);
	
	const char * path0 = "game_center_large.png";
	const char * path1 = "game_center_large_pressed.png";
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	if (!sk::game_services::mac_gamecenter_supported())
	{
		path0 = path1 = "pro.png";
	}
#endif



	gamecenterBtn = CCMenuItemImage::create("game_center_large.png",
			"game_center_large_pressed.png", this,
			menu_selector(HelloWorld::showGameCenter));
	gamecenterBtn->setPosition(ccp(_W * .5, _H * .8 + KK * 500));
	gamecenterBtn->setOpacity(180);
	gamecenterBtn->setScale(KK);

	CCMenuItemImage * s0 = CCMenuItemImage::create("sound.png", "sound.png");
	CCMenuItemImage * s1 = CCMenuItemImage::create("sound_off.png", "sound_off.png");
	s0->setOpacity(170);
	s1->setOpacity(170);
	soundBtn = CCMenuItemToggle::createWithTarget(this,
			menu_selector(HelloWorld::toggleSound),
			s0,
			s1, NULL);
	soundBtn->setPosition(KK * 175, _H + KK * (-50 + 300));
	soundBtn->setOpacity(170);
	soundBtn->setScale(KK * 0.5f);

	
	CCMenuItemImage * m0 = CCMenuItemImage::create("music.png", "music.png");
	CCMenuItemImage * m1 = CCMenuItemImage::create("music_off.png", "music_off.png");
	m0->setOpacity(170);
	m1->setOpacity(170);
	
	musicBtn = CCMenuItemToggle::createWithTarget(this,
			menu_selector(HelloWorld::toggleMusic),
			m0,
			m1, NULL);
	musicBtn->setPosition(75 * KK, _H + KK * (-50 + 300));
	musicBtn->setOpacity(170);
	musicBtn->setScale(KK * 0.5f);
	
	
	proBtn = CCMenuItemImage::create("pro.png", "pro.png", this,
										  menu_selector(HelloWorld::pro));
	proBtn->setPosition(_W - KK * 75, _H + KK * (-50 + 300));
	proBtn->setOpacity(200);
	proBtn->setScale(KK * 0.4f);
	
	int dd = 0;
	
#ifdef SK_PAID
	dd = 100;
	proBtn->setVisible(false);
#endif

	facebookBtn = CCMenuItemImage::create("facebook.png", "facebook.png", this,
			menu_selector(HelloWorld::facebook));
	facebookBtn->setPosition(_W - KK * (275 - dd), _H + KK * (-50 + 300));
	facebookBtn->setOpacity(200);
	facebookBtn->setScale(KK * 0.5f);

	twitterBtn = CCMenuItemImage::create("twitter.png", "twitter.png", this,
			menu_selector(HelloWorld::twitter));
	twitterBtn->setPosition(_W - (175 - dd) * KK, _H + KK * (-50 + 300));
	twitterBtn->setOpacity(200);
	twitterBtn->setScale(KK * 0.5f);

	musicBtn->setSelectedIndex(isMusic() ? 0 : 1);
	soundBtn->setSelectedIndex(isSound() ? 0 : 1);

	replayBtn->setOpacity(0);
	replayBtn->setPosition(ccp(_W - KK * 75, _H - KK * 50));
	replayBtn->setScale(KK);

	levelBtn->setPosition(ccp(_W - KK * 75, KK * 50));
	giftBtn->setPosition(ccp(_W - KK * 75, _H * 0.5));
	
	infoBtn->setPosition(ccp(75 * KK, 50 * KK));
	solutionBtn->setPosition(ccp(75 * KK, _H * 0.5));
	
	levelBtn->setScale(KK);
	levelBtn->setOpacity(0);
	
	giftBtn->setScale(KK);
	giftBtn->setOpacity(0);
	
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(closeBtn, replayBtn,gamecenterBtn,

			soundBtn, musicBtn, proBtn, facebookBtn, twitterBtn, levelBtn, infoBtn, solutionBtn, giftBtn,
			NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	CCLabelTTF* pLabel = CCLabelTTF::create("", "Thonburi", 34);

	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition(ccp(size.width / 2, size.height - 20));

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("bg_1024.png");

	pSprite->setScale(sqrt(_W * _W + _H * _H) / 1000.0f);

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(size.width / 2, size.height / 2));

	// add the sprite as a child to this layerccTouchesBegan
	this->addChild(pSprite, 0);
	if(CCDirector::sharedDirector()->getWinSize().width>1000)
	{
	CCAnimation * animation = CCAnimation::create();

	animation->addSpriteFrameWithFileName(_NAME("0.png"));
	animation->addSpriteFrameWithFileName(_NAME("1.png"));
	animation->addSpriteFrameWithFileName(_NAME("2.png"));
	animation->addSpriteFrameWithFileName(_NAME("3.png"));
	animation->addSpriteFrameWithFileName(_NAME("4.png"));
	animation->setDelayPerUnit(0.075f);
	action_w2r = CCAnimate::create(animation);
	action_w2r->retain();

	animation = CCAnimation::create();
	animation->addSpriteFrameWithFileName(_NAME("4.png"));
	animation->addSpriteFrameWithFileName(_NAME("5.png"));
	animation->addSpriteFrameWithFileName(_NAME("6.png"));
	animation->addSpriteFrameWithFileName(_NAME("7.png"));
	animation->addSpriteFrameWithFileName(_NAME("0.png"));
	animation->setDelayPerUnit(0.075f);
	action_r2w = CCAnimate::create(animation);
	action_r2w->retain();

	CCRotateBy * rotate = CCRotateBy::create(220.0f, 360.0f);
	pSprite->runAction(CCRepeatForever::create(rotate));
	}
	else
	{
		CCAnimation * animation = CCAnimation::create();

			animation->addSpriteFrameWithFileName(_NAME("0.png"));
			animation->addSpriteFrameWithFileName(_NAME("1.png"));
			animation->addSpriteFrameWithFileName(_NAME("2.png"));
			animation->addSpriteFrameWithFileName(_NAME("3.png"));
			animation->addSpriteFrameWithFileName(_NAME("4.png"));
			animation->setDelayPerUnit(0.075f);
			action_w2r = CCAnimate::create(animation);
			action_w2r->retain();

			animation = CCAnimation::create();
			animation->addSpriteFrameWithFileName(_NAME("4.png"));
			animation->addSpriteFrameWithFileName(_NAME("5.png"));
			animation->addSpriteFrameWithFileName(_NAME("6.png"));
			animation->addSpriteFrameWithFileName(_NAME("7.png"));
			animation->addSpriteFrameWithFileName(_NAME("0.png"));
			animation->setDelayPerUnit(0.075f);
			action_r2w = CCAnimate::create(animation);
			action_r2w->retain();

			CCRotateBy * rotate = CCRotateBy::create(220.0f, 360.0f);
			pSprite->runAction(CCRepeatForever::create(rotate));
	}
	setTouchEnabled(true);

	menuPlay = CCLabelBMFont::create("Play", "font.fnt");
	menuPlay->setPosition(_W * 0.5 + KK * 1000, _H * 0.56);
	menuPlay->setScale(KK);
	menuPlay->setOpacity(50);
	this->addChild(menuPlay, 4);
	
	
	menuMoreGames = CCLabelBMFont::create("More Games", "font.fnt");
	menuMoreGames->setPosition(_W * 0.5 - KK * 1000, _H * 0.4);
	menuMoreGames->setScale(KK);
	menuMoreGames->setOpacity(50);
	this->addChild(menuMoreGames, 4);
	
	

	menuLevels = CCLabelBMFont::create("Levels", "font.fnt");
	menuLevels->setPosition(_W * 0.5 + KK * 1000, _H * 0.25);
	menuLevels->setOpacity(50);
	menuLevels->setScale(KK);
	this->addChild(menuLevels, 4);

	menuReset = CCLabelBMFont::create("Reset", "font.fnt");
	menuReset->setPosition(_W * 0.5 - KK * 1000,

			_H *0.25
			);
	menuReset->setOpacity(50);
	this->addChild(menuReset, 4);

	labelStat = CCLabelBMFont::create("                                                                       ", "font.fnt");
	labelStat->setPosition(_W * 0.5, _H * 0.925);
	labelStat->setOpacity(200);
	this->addChild(labelStat, 4);
	labelStat->setScale(
			KK * 0.5f * ((_W >= 680) && (_W <= 800) ? 0.75f : 1.0f));

#ifndef NO_IN_APPS

	labelUnlockOrRestore = CCLabelBMFont::create("                                                                       ", "font.fnt");
	labelUnlockOrRestore->setPosition(_W * 0.5, _H * (1 - 0.925));
	labelUnlockOrRestore->setOpacity(150);
	labelUnlockOrRestore->setScale(KK *0.5f);
	
	this->addChild(labelUnlockOrRestore, 4);

	if (sk::game_services::is_all_unlocked())
	{
		labelUnlockOrRestore->setVisible(false);
	}
#endif

	labelLevel = CCLabelBMFont::create("", "font.fnt");
	labelLevel->setPosition(_W * 0.5 - KK * 1000, _H * 0.5);
	labelLevel->setOpacity(50);
	labelLevel->setScale(KK);
	this->addChild(labelLevel, 4);
	
	lblNumHints = CCLabelBMFont::create("", "font.fnt");
	lblNumHints->setScale(_W == 480 ? KK : KK * 0.5f);
	lblNumHints->setAlignment(kCCTextAlignmentCenter);
	lblNumHints->setAnchorPoint(CCPoint(-0.25, -1));
	solutionBtn->addChild(lblNumHints);

	gs = GS_Started;

	mainMenu(true);
	runWithDelay(MAIN_MENU_TIME, callfunc_selector(HelloWorld::setMainMenu));

	scheduleUpdate();

	unsigned int data = 0xffffffff;
	t.initWithData(&data, kCCTexture2DPixelFormat_RGBA8888, 1, 1, CCSize(1, 1));
	//fullscreenQuad = CCSprite::createWithTexture(&t, CCRect(0, 0, _W, _H));

	//fullscreenQuad->setPosition(cocos2d::CCPoint(_W * 0.5f, _H * 0.5f));
//	hideFullscreenQuad();
	//addChild(fullscreenQuad, 1000);

	if (CCUserDefault::sharedUserDefault()->getIntegerForKey("music", 1) != 0) {
		//const char * path =
			//	cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(
				//		TRACK_NAME);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
	//	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(TRACK_NAME);

		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
	    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(TRACK_NAME, true);
	}

#ifdef AD_ENABLED
	_playedLevels = 0;
#endif
	updateHints();
	return true;
}
void HelloWorld::gift1()
{
	playSound();
	HelloWorld::pro();
}
void HelloWorld::gift()
{
	playSound();
	dialog_instance_t alert_dialog = NULL;
				int shutdown = 0;

				bps_initialize();

				dialog_request_events(0);    //0 indicates that all events are requested
				navigator_request_events(0);
				if (dialog_create_alert(&alert_dialog) != BPS_SUCCESS) {
				    fprintf(stderr, "Failed to create alert dialog.\n");

				}
				dialog_set_size(alert_dialog,DIALOG_SIZE_LARGE);

				if(dialog_set_title_text(alert_dialog, "Purchase 10 Hints for Rs:10 ")!=BPS_SUCCESS)
				{
					fprintf(stderr, "Failed to set alert dialog title text.\n");
							      dialog_destroy(alert_dialog);
							      alert_dialog = 0;
				}
				if (dialog_set_alert_message_text(alert_dialog, "Please tap 'Ok' to continue")
				    != BPS_SUCCESS) {
				      fprintf(stderr, "Failed to set alert dialog message text.\n");
				      dialog_destroy(alert_dialog);
				      alert_dialog = 0;

				}

				char* cancel_button_context = "cancel";

				if (dialog_add_button(alert_dialog, "Cancel", true,
				    cancel_button_context, true) != BPS_SUCCESS) {
				      fprintf(stderr, "Failed to add button to alert dialog.\n");
				      dialog_destroy(alert_dialog);
				      alert_dialog = 0;

				}

				if (dialog_add_button(alert_dialog, "Ok", true, 0, true)
				    != BPS_SUCCESS) {
				      fprintf(stderr, "Failed to add button to alert dialog.\n");
				      dialog_destroy(alert_dialog);
				      alert_dialog = 0;

				}

				if (dialog_show(alert_dialog) != BPS_SUCCESS) {
				    fprintf(stderr, "Failed to show alert dialog.\n");
				    dialog_destroy(alert_dialog);
				    alert_dialog = 0;

				}

				while (!shutdown) {
				    bps_event_t *event = NULL;
				    bps_get_event(&event, -1);    // -1 means that the function waits
				                                  // for an event before returning

				    if (event) {
				    	if (bps_event_get_domain(event) == navigator_get_domain())
				    		   							    			{
				    		   							    		fprintf(stderr,"\nhacked by bhargav code:%d",bps_event_get_code(event));

				    		   							    				switch (bps_event_get_code(event))
				    		   							    				{
				    		   							    					case NAVIGATOR_APP_STOPPING:
				    		   							    												exit(1);		    					    				//	exit(0);
				    		   							    					    					    return;
				    		   							    					 break;
				    		   							    					case 11:
				    		   							    							fprintf(stderr,"hacked by bhargav");
				    		   							    					    	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
				    		   							    					    	break;
				    		   							    					case 10:
				    		   							    						SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
				    		   							    						break;
				    		   							    				}
				    		   							    			}
				        if (bps_event_get_domain(event) == dialog_get_domain()) {

				            int selectedIndex =
				                dialog_event_get_selected_index(event);
				            const char* label =
				                dialog_event_get_selected_label(event);
				            const char* context =
				                dialog_event_get_selected_context(event);
				           if(selectedIndex==1)
				            {
				        	   space_flip_purchase_hint();
				            	return;
				            }
				            else
				            {
				            	return;
				            }
				            // Process dialog event here
				        }
				    }

				}

				if (alert_dialog) {
						    dialog_destroy(alert_dialog);
						}
				bps_shutdown();
}

void HelloWorld::updateHints()
{
	char text[16] = {0};
	int h = getHints();
	sprintf(text, "x%02d", h);
	lblNumHints->setString(text);
}

HelloWorld::HelloWorld()
{
	min=0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( TRACK_NAME );
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect         ( SOUND_NAME);
#endif
	sk::game_services::register_ui_purchase_observer(this);
}
char buffer[16] = { 0 };

void HelloWorld::showGameCenter()
{
	playSound();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	if (!sk::game_services::mac_gamecenter_supported())
	{
		CCMessageBox("Thanks for purchase! You rock!!!", "Upgrade your Mac OS to 10.8+ to enable Game Center!");
		return;
	}
#endif
	//sk::game_services::show_leaderboard(DEFAULT_LEADERBOARD_GAME_ID);
}

void HelloWorld::hideFullscreenQuad() {
	//fullscreenQuad->setOpacity(0);
	//fullscreenQuad->setVisible(false);
}

void HelloWorld::addSprites() {


	fprintf(stderr,"\nenter40");
	float d;
	float scale;
	float scaleY = 1.0f;
	float scaleTilesY = 0.5f;
	if(CCDirector::sharedDirector()->getWinSize().width>1000)
	{
	if (GS_Levels == gs) {
		tiles.init(10, 4);
		d = _H * 0.5f / tiles.height();
		scale = 1.4f * d / 200.0f;
		scaleY = 1.4f;
	} else {
		d = _H * 0.85f / tiles.height();
		scale = 1.2f * d / 255.0f;
		scaleTilesY = 0.45f;
		TSCALE = scale;
	}
	}
	else
	{
		if (GS_Levels == gs) {
				tiles.init(10, 4);
				d = _H * 0.35f / tiles.height();
				scale = 1.4f * d / 200.0f;
				scaleY = 1.4f;
			} else {
				d = _H * 0.65f / tiles.height();
				scale = 1.2f * d / 255.0f;
				scaleTilesY = 0.45f;
				TSCALE = scale;
			}
	}
	for (int i = 0; i < stiles.size(); ++i) {
		if (stiles[i]) {
			stiles[i]->removeFromParentAndCleanup(true);
			stiles[i] = 0;
		}
	}

	stiles.resize(tiles.width() * tiles.height());
	for (int i = 0; i < tiles.width(); ++i) {
		for (int j = 0; j < tiles.height(); ++j) {
			float x,y;
			if(CCDirector::sharedDirector()->getWinSize().width>1000)
								{
			 x= _W * .5 + d * (i - 0.5f * (tiles.width() - 1));
			 y =_H* scaleTilesY+ scaleY * d * (j - 0.5f * (tiles.height() - 1)) + _H;
								}else
								{

			 x = _W * .5 + d * (i - 0.55f * (tiles.width() - 1))+10;
			 y =_H* scaleTilesY+ scaleY * d * (j - 0.5f * (tiles.height() - 1)) + _H;
								}
			if(	GS_Gameplay == gs)
			{
				if (tiles.at(i, j))
				{
					if(CCDirector::sharedDirector()->getWinSize().width>1000)
					{

					CCSprite* animSprite = CCSprite::create(colors.at(i, j) ? _NAME("0.png") : _NAME("4.png"));

					animSprite->setScale(scale);

					animSprite->setPosition(ccp(x, y ));
					animSprite->setOpacity(200);

					stiles[tiles.index(i, j)] = animSprite;

					this->addChild(animSprite, 2);
					}
					else
					{

						CCSprite* animSprite = CCSprite::create(colors.at(i, j) ? _NAME("0.png") : _NAME("4.png"));

											animSprite->setScale(scale);

											animSprite->setPosition(ccp(x, y ));
											animSprite->setOpacity(200);

											stiles[tiles.index(i, j)] = animSprite;

											this->addChild(animSprite, 2);
					}
				}
				else
				{
					stiles[tiles.index(i, j)] = 0;
				}
			}
			else
			{
				bool active = isActive(i + tiles.width() * j);
				char buff[16] = {0};
				int level = i + j * tiles.width();
				sprintf(buff, "%d", int(LEVEL_FIX + level));
				CCLabelBMFont* animSprite = CCLabelBMFont::create(buff, "font.fnt");

				if (active)
				{
					this->addChild(animSprite);
					animSprite->setPosition(ccp(x, y));
					stiles[tiles.index(i, j)] = animSprite;
				}
				else
				{
					CCSprite* animSprite1 = CCSprite::create("lock.png");
					stiles[tiles.index(i, j)] = animSprite1;
					animSprite1->setScale(KK *0.5f);
					animSprite1->setPosition(ccp(x, y));
					this->addChild(animSprite1, 2);
					animSprite1->addChild(animSprite);
					animSprite1->setOpacity(100);
				}

				animSprite->setColor(ccWHITE);
				animSprite->setOpacity(isActive(level) ? 200 : 100);
				animSprite->setScale(scale * (!active ? 1.7f / KK: 1.0f));
			}
		}
	}
	initDelays();
	tilesUp = false;
}

bool HelloWorld::isActive(int level) {
	//return true;
	if (level == 0) {
		return true;
	}
#ifndef NO_IN_APPS
	if (sk::game_services::is_all_unlocked())
	{
		return true;
	}
#endif
	char buffer[16] = { 0 };
	sprintf(buffer, "active%d", level);
	int active = CCUserDefault::sharedUserDefault()->getIntegerForKey(buffer,
			0);
	return active != 0;
}

void HelloWorld::updateStatLabel() {
	int level = CCUserDefault::sharedUserDefault()->getIntegerForKey("level",
			0) + LEVEL_FIX;
	char buffer[64] = { 0 };
	sprintf(buffer, "Level:%d Score:%d Total:%d", level, _score,
			getTotalScore());
	labelStat->setString(buffer);

}

void HelloWorld::setScore(int level, int score) {
	char buffer[16] = { 0 };
	sprintf(buffer, "score%d", level);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(buffer, score);
	CCUserDefault::sharedUserDefault()->flush();
}

int HelloWorld::getScore(int level) {
	char buffer[16] = { 0 };
	sprintf(buffer, "score%d", level);
	int score = CCUserDefault::sharedUserDefault()->getIntegerForKey(buffer, 0);
	return score;
}

int HelloWorld::getTotalScore() {
	int res = 0;
	for (int i = 0; i < TOTAL_LEVELS; ++i) {
		res += getScore(i);
	}
	return res;
}

void  HelloWorld::setHints(int hint)
{
	if (hint < 0)
	{
		hint = 0;
	}
	CCUserDefault::sharedUserDefault()->setIntegerForKey("hints", hint);
	CCUserDefault::sharedUserDefault()->flush();
}

int  HelloWorld::getHints()
{
	int hints = CCUserDefault::sharedUserDefault()->getIntegerForKey("hints",3);
	return hints;
}


void HelloWorld::setActive(int level, bool active) {
	char buffer[16] = { 0 };
	sprintf(buffer, "active%d", level);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(buffer,
			active ? 1 : 0);

	if (!active) {
		setScore(level, 0);
	}

	CCUserDefault::sharedUserDefault()->flush();
}

void HelloWorld::setLevel(int level) {
	mainmenu=1;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("level", level);
	CCUserDefault::sharedUserDefault()->flush();
}

void HelloWorld::initDelays() {
	delays.resize(stiles.size());
	for (int i = 0; i < delays.size(); ++i) {
		float delay = 0.05 + 0.4 * 0.01 * (rand() % 100);
		delays[i] = delay;
	}
}

void HelloWorld::update(float dt) {
	cocos2d::CCLayer::update(dt);
	for (int i = 0; i < delays.size(); ++i) {
		if (!tiles.at(i) && gs == GS_Gameplay) {
			continue;
		}

		if (delays[i] < 0) {
			if (delays[i] > -1000) {
				CCNode * animSprite = stiles[i];
				if (animSprite
						&& CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
								animSprite) == 0 && animSprite->isVisible()) {
					CCMoveTo * moveAction = CCMoveTo::create(TILE_MOVE_TIME,
							CCPoint(animSprite->getPositionX(),
									animSprite->getPositionY()
											+ (tilesUp ? _H : -_H))
							);
					animSprite->runAction(CCEaseBackOut::create(moveAction));
				} else {
					if (animSprite) {
						animSprite->setVisible(false);
					}
				}
				delays[i] = -1000;
			}
		} else {
			delays[i] -= dt;
		}
	}

}

void HelloWorld::moveTile() {
	printf("MoveTile ");
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	if (showSolution)
	{
		return;
	}
	if (CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(closeBtn) != 0)
	{
		return;
	}
	if (tilesUp)
	{
		return;
	}
#ifndef NO_IN_APPS	
	if (sk::game_services::is_all_unlocked())
	{
		labelUnlockOrRestore->setVisible(false);
	}
#endif
	for (int i = 0; i < stiles.size(); ++i)
	{
		if (stiles[i])
		{			
			{
				stiles[i]->setVisible(false);
			}
		}
	}
	labelLevel->setVisible(false);
	mainMenu(true);
	initDelays();
	tilesUp = true;
	runWithDelay(MAIN_MENU_TIME, callfunc_selector(HelloWorld::setMainMenu));
}

void HelloWorld::animateToggle(int x, int y) {
	if (!tiles.is_valid_index(x, y) || !tiles.at(x, y)) {
		return;
	}
	int ind = x + y * tiles.width();
	stiles[ind]->runAction(
			colors.at(ind) ?
					(CCAction*) action_w2r->copy() :
					(CCAction*) action_r2w->copy());
}

void HelloWorld::fadeOutFullscreenQuad() {
	//fullscreenQuad->runAction(CCFadeTo::create(MAIN_MENU_TIME * 0.5f, 0));

}

bool HelloWorld::flipToReplay()
{
	bool changed = false;
	for (int i = 0; i < colors.width(); ++i) {
		for (int j = 0; j < colors.height(); ++j) {
			if (colorsOrig.at(i, j) != colors.at(i, j)) {
				animateToggle(i, j);
				colors.set(i, j, colorsOrig.at(i, j));
				changed = true;
			}
			
		}
	}
	return changed;
}

void HelloWorld::replay() {
	if (gs != GS_Gameplay) {
		return;
	}
//	fullscreenQuad->setVisible(true);
//	fullscreenQuad->runAction(CCFadeTo::create(MAIN_MENU_TIME * 0.5f, 255));
	bool changed = flipToReplay();
	if (changed) {
		playSound();
		startShowLevel();
		_score = 10000;

		labelLevel->setString(levelBuffer);
		updateStatLabel();
		
		sk::game_services::on_level_ended();
		
	}
	//runWithDelay(MAIN_MENU_TIME, callfunc_selector(HelloWorld::fadeOutFullscreenQuad));
}

void HelloWorld::doGameplay() {
	setGamePlay();
	loadLevel(getLevelDef());
	addSprites();
	labelStat->runAction(CCFadeTo::create(MAIN_MENU_TIME, 150));
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event) {

	if (showSolution)
	{
		return;
	}
	CCTouch* touch = (CCTouch*) touches->anyObject();
	CCPoint location = touch->getLocation();

	for (int i = 0; i < stiles.size(); ++i) {
		if (!tiles.at(i) && gs == GS_Gameplay) {
			continue;
		}
		if (GS_Gameplay != gs) {
			continue;
		}
		if (0 == stiles[i]) {
			continue;
		}
		if (stiles[i]->boundingBox().containsPoint(location)
				&& CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
						stiles[i]) == 0) {
			stiles[i]->setScale(TSCALE * 0.9);
			break;
		}
	}

}



static char* get_window_group_id()
{
	// S3DWindowGroup
   static char s_window_group_id[sizeof("") + sizeof(double)] = "";

      snprintf(s_window_group_id, sizeof(s_window_group_id), "%d", getpid());

   return s_window_group_id;
}


void HelloWorld::nextLevel() {
	int ind = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 0);

	if (getScore(ind) < _score)
	{
		setScore(ind, _score);
		sk::game_services::submit_score(DEFAULT_LEADERBOARD_GAME_ID, getTotalScore());
	}

	if (ind == (TOTAL_LEVELS - 1)) {
		ind = 0;
	} else {
		++ind;
	}

	setActive(ind, true);
	setLevel(ind);
	loadLevel(getLevelDef());
	addSprites();

	sk::game_services::on_level_ended();
}






void HelloWorld::on_purchase_ok(const char * sku, unsigned request_id)
{
	fprintf(stderr,"\ngot request ok");
}

void HelloWorld::on_purchase_restore_ok(const char * sku)
{

}

void HelloWorld::on_purchase_failed(const char * sku, unsigned request_id)
{

}



void HelloWorld::callUnlock()
{
	fprintf(stderr,"unlock called");
	sk::game_services::buy_product(INAPP_UNLOCKALL_GAME_ID);

	//sk::game_services::restore_purchases();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	igInApps::Purchase("com.llcdraco.flippmaniafree.unlockall", ig_interface::DIALOG_PURCHASE_UNLOCK, this);
#endif
	
	dialog_instance_t alert_dialog = NULL;
			int shutdown = 0;

			bps_initialize();

			dialog_request_events(0);
			navigator_request_events(0);//0 indicates that all events are requested

			if (dialog_create_alert(&alert_dialog) != BPS_SUCCESS) {
			    fprintf(stderr, "Failed to create alert dialog.\n");

			}
			dialog_set_size(alert_dialog,DIALOG_SIZE_LARGE);

			if(dialog_set_title_text(alert_dialog, "Unlock all levels for Rs 59.79")!=BPS_SUCCESS)
			{
				fprintf(stderr, "Failed to set alert dialog title text.\n");
						      dialog_destroy(alert_dialog);
						      alert_dialog = 0;
			}
			if (dialog_set_alert_message_text(alert_dialog, "Please tap 'Ok' to continue")
			    != BPS_SUCCESS) {
			      fprintf(stderr, "Failed to set alert dialog message text.\n");
			      dialog_destroy(alert_dialog);
			      alert_dialog = 0;

			}

			char* cancel_button_context = "cancle";

			if (dialog_add_button(alert_dialog, "Cancel", true,
			    cancel_button_context, true) != BPS_SUCCESS) {
			      fprintf(stderr, "Failed to add button to alert dialog.\n");
			      dialog_destroy(alert_dialog);
			      alert_dialog = 0;

			}

			if (dialog_add_button(alert_dialog, "Ok", true, 0, true)
			    != BPS_SUCCESS) {
			      fprintf(stderr, "Failed to add button to alert dialog.\n");
			      dialog_destroy(alert_dialog);
			      alert_dialog = 0;

			}

			if (dialog_show(alert_dialog) != BPS_SUCCESS) {
			    fprintf(stderr, "Failed to show alert dialog.\n");
			    dialog_destroy(alert_dialog);
			    alert_dialog = 0;

			}

			while (!shutdown) {
			    bps_event_t *event = NULL;
			    bps_get_event(&event, -1);    // -1 means that the function waits
			                                  // for an event before returning

			    if (event) {
			    	if (bps_event_get_domain(event) == navigator_get_domain())
			    			    			{
			    			    		fprintf(stderr,"\nhacked by bhargav code:%d",bps_event_get_code(event));

			    			    				switch (bps_event_get_code(event))
			    			    				{
			    			    					case NAVIGATOR_APP_STOPPING:
			    			    												exit(1);		    					    				//	exit(0);
			    			    					    					    return;
			    			    					    					    break;

			    			    					    					    				case 11:

			    			    					    					    					fprintf(stderr,"hacked by bhargav");
			    			    					    					    								CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();



			    			    					    					    					break;

			    			    					    					    				case 10:


			    			    					    					    								SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();


			    			    					    					    					break;






			    			    					    					    			}
			    			    					    					    		}
			        if (bps_event_get_domain(event) == dialog_get_domain()) {

			            int selectedIndex =
			                dialog_event_get_selected_index(event);
			            const char* label =
			                dialog_event_get_selected_label(event);
			            const char* context =
			                dialog_event_get_selected_context(event);
			            if(selectedIndex==1)
			            {
			            	HelloWorld::purchaselock();
			            	return;
			            }
			            else
			            {
			            	return;
			            }
			            // Process dialog event here
			        }
			    }
			}

			if (alert_dialog) {
					    dialog_destroy(alert_dialog);
					}
			bps_shutdown();

//   bps_event_t *event = NULL;
//
//   			while(true)
//   			{
//   				bps_get_event(&event, 1);
//
//   				if (event) {
//   					if (bps_event_get_domain(event) == paymentservice_get_domain())
//   					{
//   						if (SUCCESS_RESPONSE == paymentservice_event_get_response_code(event))
//   						{
//   							if (PURCHASE_RESPONSE == bps_event_get_code(event))
//   							{
//   								// Handle a successful purchase here
//   								const char* digital_good = paymentservice_event_get_digital_good_id(event, 0);
//   								const char* digital_sku = paymentservice_event_get_digital_good_sku(event, 0);
//
//   								HelloWorld::on_purchase_ok(digital_sku, request_id); //comment this one if you don't want to put all info in console
//   								int result = atoi(digital_good);
//
//   								return ;
//   							}
//   							else
//   							{
//   								// Handle a successful query for past purchases here
//   								int numPurchases = paymentservice_event_get_number_purchases(event);
//   								fprintf(stderr, "Handle a successful query for past purchases here");
//
//   								return;
//   							}
//   						}
//   						else
//   						{
//   							int error_id = paymentservice_event_get_error_id(event);
//   							const char* error_text = paymentservice_event_get_error_text(event);
//   							HelloWorld::on_purchase_failed(digital_good_sku, request_id); //comment this one if you don't want to put all info in console
//
//   							return;
//   						}
//   					}
//   				}
//   			}




}

void HelloWorld::purchaselock()
{
	 cb = this;

	   const char* digital_good_id = "32836899";
	   const char* digital_good_name = "Unlock Candy Flip";
	   const char* digital_good_sku = "comDSStudiosCandyFlipFreeUnlocksku";
	   const char* metadata = NULL;
	   const char* purchase_app_icon = "http://www.rim.com/products/appworld_3col.jpg";
	   const char* purchase_app_name = "Space Flip";

	   /*
	    * Define a request ID to hold the returned value from the purchase request.
	    */
	   unsigned request_id = 0;
	   paymentservice_request_events(0);
	   paymentservice_set_connection_mode(false);
	   /*
	    * initiate the application with a purchase of the sample digital good.
	    */
	   fprintf(stderr, "Before paymentservice_purchase_request.\n");

	   if (paymentservice_purchase_request(digital_good_id, digital_good_sku, digital_good_name,
	           metadata, purchase_app_name, purchase_app_icon, get_window_group_id(), &request_id) != BPS_SUCCESS) {
	       fprintf(stderr, "Error: purchase request failed.\n");
	   }
	   else
	   {
	       fprintf(stderr, "Ok: purchase request ok.\n");
	   }
	   bps_event_t *event = NULL;
	   while(true)
	   			{
	   				bps_get_event(&event, 1);

	   				if (event) {
	   					if (bps_event_get_domain(event) == navigator_get_domain())
	   							    			{
	   							    		fprintf(stderr,"\nhacked by bhargav code:%d",bps_event_get_code(event));

	   							    				switch (bps_event_get_code(event))
	   							    				{
	   							    					case NAVIGATOR_APP_STOPPING:
	   							    												exit(1);		    					    				//	exit(0);
	   							    					    					    return;
	   							    					    					    break;

	   							    					    					    				case 11:

	   							    					    					    					fprintf(stderr,"hacked by bhargav");
	   							    					    					    								CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();



	   							    					    					    					break;

	   							    					    					    				case 10:


	   							    					    					    								SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();


	   							    					    					    					break;






	   							    					    					    			}
	   							    					    					    		}
	   					if (bps_event_get_domain(event) == paymentservice_get_domain())
	   					{
	   						if (SUCCESS_RESPONSE == paymentservice_event_get_response_code(event))
	   						{
	   							if (PURCHASE_RESPONSE == bps_event_get_code(event))
	   							{
	   								// Handle a successful purchase here
	   								const char* digital_good = paymentservice_event_get_digital_good_id(event, 0);
	   								const char* digital_sku = paymentservice_event_get_digital_good_sku(event, 0);

	   								onPurchaseSuccess(event); //comment this one if you don't want to put all info in console
	   								//int result = atoi(digital_good);

	   								return ;
	   							}
	   							else
	   							{
	   								// Handle a successful query for past purchases here
	   								int numPurchases = paymentservice_event_get_number_purchases(event);
	   								fprintf(stderr, "Handle a successful query for past purchases here");

	   								return;
	   							}
	   						}
	   						else
	   						{
	   							int error_id = paymentservice_event_get_error_id(event);
	   							const char* error_text = paymentservice_event_get_error_text(event);
	   							failureCommon(event); //comment this one if you don't want to put all info in console

	   							return;
	   						}
	   					}
	   				}
	   			}
	   bps_shutdown();
}
void HelloWorld::onGetExistingPurchasesSuccess(bps_event_t *event)
{
    if (event == NULL) {
        fprintf(stderr, "Invalid event.\n");
        return;
    }

    unsigned request_id = paymentservice_event_get_request_id(event);
    int purchases = paymentservice_event_get_number_purchases(event);

    fprintf(stderr, "Get existing purchases success. Request ID: %d\n", request_id);
    fprintf(stderr, "Number of existing purchases: %d\n", purchases);
    fprintf(stderr, "Existing purchases:\n");

    int i = 0;
    for (i = 0; i<purchases; i++) {
        const char* date = paymentservice_event_get_date(event, i);
        const char* digital_good = paymentservice_event_get_digital_good_id(event, i);
        const char* digital_sku = paymentservice_event_get_digital_good_sku(event, i);
        const char* license_key = paymentservice_event_get_license_key(event, i);
        const char* metadata = paymentservice_event_get_metadata(event, i);
        const char* purchase_id = paymentservice_event_get_purchase_id(event, i);

        fprintf(stderr, "  Date: %s  PurchaseID: %s  DigitalGoodID: %s  SKU: %s  License: %s  Metadata: %s\n",
            date ? date : "N/A",
            purchase_id ? purchase_id : "N/A",
            digital_good ? digital_good : "N/A",
            digital_sku ? digital_sku : "N/A",
            license_key ? license_key : "N/A",
            metadata ? metadata : "N/A");
    }
    //this->unschedule(schedule_selector(HelloWorld::sched));
}

void HelloWorld::onPurchaseSuccess(bps_event_t *event)
{
    if (event == NULL) {
        fprintf(stderr, "Invalid event.\n");
        return;
    }

    unsigned request_id = paymentservice_event_get_request_id(event);
    const char* date = paymentservice_event_get_date(event, 0);
    const char* digital_good = paymentservice_event_get_digital_good_id(event, 0);
    const char* digital_sku = paymentservice_event_get_digital_good_sku(event, 0);
    const char* license_key = paymentservice_event_get_license_key(event, 0);
    const char* metadata = paymentservice_event_get_metadata(event, 0);
    const char* purchase_id = paymentservice_event_get_purchase_id(event, 0);

    fprintf(stderr, "Purchase success. Request Id: %d\n Date: %s\n DigitalGoodID: %s\n SKU: %s\n License: %s\n Metadata: %s\n PurchaseId: %s\n\n",
        request_id,
        date ? date : "N/A",
        digital_good ? digital_good : "N/A",
        digital_sku ? digital_sku : "N/A",
        license_key ? license_key : "N/A",
        metadata ? metadata : "N/A",
        purchase_id ? purchase_id : "N/A");
    //this->unschedule(schedule_selector(HelloWorld::sched));

    HelloWorld::unlockLevels();
}
void HelloWorld::unlockLevels()
{
	sk::game_services::on_unlock_all();
}
void HelloWorld::failureCommon(bps_event_t *event)
{
	fprintf(stderr,"\nentered in to failure");
    if (event == NULL) {
        fprintf(stderr, "Invalid event.\n");
        return;
    }

    unsigned request_id = paymentservice_event_get_request_id(event);
    int error_id = paymentservice_event_get_error_id(event);
    const char* error_text = paymentservice_event_get_error_text(event);

    fprintf(stderr, "Payment System error. Request ID: %d  Error ID: %d  Text: %s\n",
            request_id, error_id, error_text ? error_text : "N/A");
    //this->unschedule(schedule_selector(HelloWorld::sched));

}



void HelloWorld::playSound() {
	if (isSound()) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SOUND_NAME);
	}
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event) {

}

void HelloWorld::setLevels() {
	mainmenu=1;
	gs = GS_Levels;
}

void HelloWorld::resetLevels() {
	for (int i = 0; i < TOTAL_LEVELS; ++i) {
		setActive(i, false);
	}
	setLevel(0);
	sk::game_services::clear_unlock_all();
}


void reset_callback(bool yes) {
	if (cb) {
		if (yes) {
			cb->resetLevels();
		}
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	
	if (showSolution)
	{
		return;
	}
	navigator_request_events(0);
	CCTouch* touch = (CCTouch*) touches->anyObject();
	CCPoint location = touch->getLocation();

	CCSize s = menuPlay->getContentSize();
	location = ccpSub(location, menuPlay->getPosition());

	if (abs(location.x) < s.width * .75 * KK
			&& abs(location.y) < s.height * .75 * KK) {
		setGamePlay();
		loadLevel(getLevelDef());
		addSprites();
		mainMenu(false);
		return;
	}
	
	s = menuMoreGames->getContentSize();
	location = ccpSub(touch->getLocation(), menuMoreGames->getPosition());
	
	if (abs(location.x) < s.width * .75 * KK
		&& abs(location.y) < s.height * .75 * KK) {
		HelloWorld::pro();
		return;
	}

	s = menuReset->getContentSize();
	location = ccpSub(touch->getLocation(), menuReset->getPosition());

	if (abs(location.x) < s.width * .65 * KK
			&& abs(location.y) < s.height * .65 * KK) {
		playSound();
		fprintf(stderr,"\nresetcalled");
		dialog_instance_t alert_dialog = NULL;
		 int shutdown = 0;

		bps_initialize();

		dialog_request_events(0);    //0 indicates that all events are requested
		navigator_request_events(0);
		if (dialog_create_alert(&alert_dialog) != BPS_SUCCESS) {
		    fprintf(stderr, "Failed to create alert dialog.\n");

		}
		if(dialog_set_title_text(alert_dialog, "Warning")!=BPS_SUCCESS)
		{
			fprintf(stderr, "Failed to set alert dialog title text.\n");
					      dialog_destroy(alert_dialog);
					      alert_dialog = 0;
		}
		if (dialog_set_alert_message_text(alert_dialog, "Are you sure you want to reset game progress?")
		    != BPS_SUCCESS) {
		      fprintf(stderr, "Failed to set alert dialog message text.\n");
		      dialog_destroy(alert_dialog);
		      alert_dialog = 0;

		}

		char* cancel_button_context = "cancle";

		if (dialog_add_button(alert_dialog, "No", true,
		    cancel_button_context, true) != BPS_SUCCESS) {
		      fprintf(stderr, "Failed to add button to alert dialog.\n");
		      dialog_destroy(alert_dialog);
		      alert_dialog = 0;

		}

		if (dialog_add_button(alert_dialog, "yes", true, 0, true)
		    != BPS_SUCCESS) {
		      fprintf(stderr, "Failed to add button to alert dialog.\n");
		      dialog_destroy(alert_dialog);
		      alert_dialog = 0;

		}

		if (dialog_show(alert_dialog) != BPS_SUCCESS) {
		    fprintf(stderr, "Failed to show alert dialog.\n");
		    dialog_destroy(alert_dialog);
		    alert_dialog = 0;

		}

		while (!shutdown) {

		    bps_event_t *event = NULL;
		    bps_get_event(&event, -1);    // -1 means that the function waits
		                                  // for an event before returning

		    if (event) {
		    	if (bps_event_get_domain(event) == navigator_get_domain())
		    			{
		    		fprintf(stderr,"\nhacked by bhargav code:%d",bps_event_get_code(event));

		    				switch (bps_event_get_code(event))
		    				{
		    					case NAVIGATOR_APP_STOPPING:
		    												exit(1);		    					    				//	exit(0);
		    					    					    return;
		    					    					    break;

		    					    					    				case 11:

		    					    					    					fprintf(stderr,"hacked by bhargav");
		    					    					    								CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();



		    					    					    					break;

		    					    					    				case 10:


		    					    					    								SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();


		    					    					    					break;






		    					    					    			}
		    					    					    		}
		        if (bps_event_get_domain(event) == dialog_get_domain()) {

		            int selectedIndex =
		                dialog_event_get_selected_index(event);
		            const char* label =
		                dialog_event_get_selected_label(event);
		            const char* context =
		                dialog_event_get_selected_context(event);
		            if(selectedIndex==1)
		            {

		            	HelloWorld::resetLevels();
		            	return;
		            }
		            else
		            {

		            	return;
		            }

		            // Process dialog event here
		        }
		    }
		}

		if (alert_dialog) {
		    dialog_destroy(alert_dialog);
		}
		bps_shutdown();
		sk::game_services::open_yes_no_dialog(DIALOG_RESET_GAME_PROGRESS, this, "Are you sure you want to reset game progress?", "Warning");
		
#if 0
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ig_interface::SystemNotificationDialog::openOkayCancelDialog(this,
				ig_interface::DIALOG_RESET_PROGRESS, "Attention!",
				"");
#endif




		
#endif
		return;
	}

	s = menuLevels->getContentSize();
	location = ccpSub(touch->getLocation(), menuLevels->getPosition());

	if (abs(location.x) < s.width * .75 * KK
			&& abs(location.y) < s.height * .75 * KK) {
		setLevels();
		addSprites();
		mainMenu(false);
		return;
	}

#ifndef NO_IN_APPS

	if (!sk::game_services::is_all_unlocked() && GS_Levels == gs)
	{
		s = labelUnlockOrRestore->getContentSize();
		location = ccpSub(touch->getLocation(), labelUnlockOrRestore->getPosition());

		if (abs(location.x) < s.width * .75 * KK && abs(location.y) < s.height * .75 * KK)
		{
			playSound();
			callUnlock();
			return;
		}
	}

#endif

	location = touch->getLocation();
	if(mainmenu==1)
	{
	for (int i = 0; i < stiles.size(); ++i) {
		if (!tiles.at(i) && gs == GS_Gameplay) {
			continue;
		}
		if (0 == stiles[i]) {
			continue;
		}
		if (GS_Gameplay == gs) {
			stiles[i]->setScale(TSCALE);
		}

		if (stiles[i]->boundingBox().containsPoint(location)
				&& CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
						stiles[i]) == 0) {
			int x = i % tiles.width();
			int y = i / tiles.height();

			if (gs != GS_Gameplay) {

				if (isActive(i)) {
					setLevel(i);
					initDelays();
					tilesUp = true;

					GLubyte op2 = 150;

					replayBtn->setOpacity(0);
					replayBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
					replayBtn->setVisible(true);

					//labelStat->setString("");
					//labelStat->setOpacity(50);
					labelStat->runAction(CCFadeTo::create(MAIN_MENU_TIME, 0));
					labelStat->setVisible(true);

					runWithDelay(MAIN_MENU_TIME,
							callfunc_selector(HelloWorld::doGameplay));

					labelUnlockOrRestore->runAction(CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,ccpAdd(labelUnlockOrRestore->getPosition(), CCPoint(0, -100)))));

#ifndef NO_IN_APPS
					labelUnlockOrRestore->runAction(CCFadeTo::create(MAIN_MENU_TIME, 0));
#endif

					bool hide = true;

					float d1 = !hide ? 100 : -100;
					GLubyte op22 = !hide ? 56 : 150;

					levelBtn->setPosition(ccp(_W + KK * 75, KK * 50));
					giftBtn->setPosition(ccp(_W + KK * 75, _H * 0.5));

					infoBtn->setPosition(ccp(-75 * KK, 50 * KK));
					solutionBtn->setPosition(ccp(-75 * KK, _H * 0.5));
					
					
					giftBtn->runAction(
										CCEaseBackOut::create(CCMoveTo::create(2 * MAIN_MENU_TIME,
														 ccpAdd(giftBtn->getPosition(),
														 CCPoint(1.5 * d1 * KK, 0))
														 ))); 
					giftBtn->runAction(
										CCFadeTo::create(2 * MAIN_MENU_TIME, op22));
					giftBtn->setVisible(true);

					levelBtn->runAction(
							CCEaseBackOut::create(CCMoveTo::create(2 * MAIN_MENU_TIME,
									ccpAdd(levelBtn->getPosition()
											, CCPoint(1.5 * d1 * KK, 0))
									))); 
					levelBtn->runAction(
							CCFadeTo::create(2 * MAIN_MENU_TIME, op22));
					levelBtn->setVisible(true);

					infoBtn->runAction(
							CCEaseBackOut::create(CCMoveTo::create(2 * MAIN_MENU_TIME,
									ccpAdd(infoBtn->getPosition()
											, CCPoint(-1.5 * d1 * KK, 0))
									))); 
					infoBtn->runAction(
							CCFadeTo::create(2 * MAIN_MENU_TIME, op22));
					infoBtn->setVisible(true);
					
					
					solutionBtn->runAction(
									   CCEaseBackOut::create(CCMoveTo::create(2 * MAIN_MENU_TIME,
														ccpAdd(solutionBtn->getPosition()
														, CCPoint(-1.5 * d1 * KK, 0))
														)));
					solutionBtn->runAction(
									   CCFadeTo::create(2 * MAIN_MENU_TIME, op22));
					solutionBtn->setVisible(true);
					
					
					

				} else {
#ifndef NO_IN_APPS
					callUnlock();
#endif
				}
				playSound();
			} else {
				animateToggle(x, y);
				animateToggle(x + 1, y);
				animateToggle(x - 1, y);
				animateToggle(x, y + 1);
				animateToggle(x, y - 1);
				colors.toggle_cross_safe(x, y);

				bool was_false = false;
				if (_score > 100) {
					_score -= 25;
				} else if (_score >= 0) {
					_score -= 1;
				}

				updateStatLabel();
				for (int j = 0; j < stiles.size(); ++j) {
					if (tiles.at(j)) {
						if (!colors.at(j)) {
							was_false = true;
							break;
						}
					}
				}

				if (!was_false) {
					initDelays();
					tilesUp = true;
					runWithDelay(MAIN_MENU_TIME,
							callfunc_selector(HelloWorld::nextLevel));

				}
				playSound();
			}
			break;
		}
	}
	}
}

void HelloWorld::levelsBack() {
	addSprites();
	initDelays();
	tilesUp = false;
}

void HelloWorld::ui_on_purchase(unsigned int game_id, bool purchased)
{
	if (!purchased)
	{
		return;
	}
	if (INAPP_UNLOCKALL_GAME_ID == game_id)
	{
		for (int i = 0; i < TOTAL_LEVELS; ++i)
		{
			setActive(i, true);
		}		
		if (gs == GS_Levels)
		{
			initDelays();
			tilesUp = true;
			runWithDelay(MAIN_MENU_TIME * 1.1f, callfunc_selector(HelloWorld::levelsBack));
			playSound();
			labelUnlockOrRestore->runAction(CCFadeTo::create(MAIN_MENU_TIME, 0));
		}
	}
}

void HelloWorld::ui_on_yes_no_dialog(unsigned int dialog_id, bool result)
{
	if (!result)
	{
		return;
	}
	if (DIALOG_RESET_GAME_PROGRESS == dialog_id)
	{
		resetLevels();
	}
	else
	if (DIALOG_CONFIRM_FACEBOOK == dialog_id)
	{
		do_facebook();
	}
}

void HelloWorld::space_flip_purchase_hint()
{
	const char* digital_good_id = "32836900";
			   const char* digital_good_name = "Hint Candy Flip";
			   const char* digital_good_sku = "comDSStudiosCandyFlipFreeHintsku";
			   const char* metadata = NULL;
			   const char* purchase_app_icon = "http://www.rim.com/products/appworld_3col.jpg";
			   const char* purchase_app_name = "Space Flip";

			   /*
			    * Define a request ID to hold the returned value from the purchase request.
			    */
			   unsigned request_id = 0;
			   paymentservice_request_events(0);
			   paymentservice_set_connection_mode(false);
			   /*
			    * initiate the application with a purchase of the sample digital good.
			    */
			   fprintf(stderr, "Before paymentservice_purchase_request.\n");

			   if (paymentservice_purchase_request(digital_good_id, digital_good_sku, digital_good_name,
			           metadata, purchase_app_name, purchase_app_icon, get_window_group_id(), &request_id) != BPS_SUCCESS) {
			       fprintf(stderr, "Error: purchase request failed.\n");
			   }
			   else
			   {
			       fprintf(stderr, "Ok: purchase request ok.\n");
			   }
			   bps_event_t *event = NULL;
			   while(true)
			   			{
			   				bps_get_event(&event, 1);

			   				if (event) {
			   					if (bps_event_get_domain(event) == paymentservice_get_domain())
			   					{
			   						if (SUCCESS_RESPONSE == paymentservice_event_get_response_code(event))
			   						{
			   							if (PURCHASE_RESPONSE == bps_event_get_code(event))
			   							{
			   								// Handle a successful purchase here
			   								const char* digital_good = paymentservice_event_get_digital_good_id(event, 0);
			   								const char* digital_sku = paymentservice_event_get_digital_good_sku(event, 0);
			   								cb->setHints(cb->getHints() + 10);
			   									cb->updateHints();
			   								//HelloWorld::onPurchaseSuccess(event); //comment this one if you don't want to put all info in console
			   								//int result = atoi(digital_good);

			   								return ;
			   							}
			   							else
			   							{
			   								// Handle a successful query for past purchases here
			   								int numPurchases = paymentservice_event_get_number_purchases(event);
			   								fprintf(stderr, "Handle a successful query for past purchases here");

			   								return;
			   							}
			   						}
			   						else
			   						{
			   							int error_id = paymentservice_event_get_error_id(event);
			   							const char* error_text = paymentservice_event_get_error_text(event);
			   							HelloWorld::failureCommon(event); //comment this one if you don't want to put all info in console

			   							return;
			   						}
			   					}
			   				}
			   			}
			   bps_shutdown();
}
void space_flip_purchase_hints()
{

	cb->setHints(cb->getHints() + 10);
				   									cb->updateHints();
}

void HelloWorld::toGameplay(bool hide) {
	float d1 = !hide ? 100 : -100;
	GLubyte op2 = !hide ? 56 : 150;
	mainmenu=1;
	if (hide) {
		giftBtn->setPosition(ccp(_W + KK * 75, _H * 0.5));
		levelBtn->setPosition(ccp(_W + KK * 75, KK * 50));
		infoBtn->setPosition(ccp(-75 * KK, 50 * KK));
		solutionBtn->setPosition(ccp(-75 * KK, _H * 0.5));
	} else {
		giftBtn->setPosition(ccp(_W - KK * 75, _H * 0.5));
		levelBtn->setPosition(ccp(_W - KK * 75, KK * 50));
		infoBtn->setPosition(ccp(75 * KK, 50 * KK));
		solutionBtn->setPosition(ccp(-75 * KK, _H * 0.5));
	}

	levelBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(levelBtn->getPosition(), CCPoint(1.5 * d1 * KK, 0))
					))); 
	levelBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	levelBtn->setVisible(gs == GS_Gameplay);
	
	giftBtn->runAction(
						CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
										 ccpAdd(giftBtn->getPosition(), CCPoint(1.5 * d1 * KK, 0))
										 ))); 
	giftBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	giftBtn->setVisible(gs == GS_Gameplay);

	infoBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(infoBtn->getPosition() , CCPoint(-1.5 * d1 * KK, 0))
					)));
	infoBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	infoBtn->setVisible(gs == GS_Gameplay);

	solutionBtn->runAction(
					   CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
										ccpAdd(solutionBtn->getPosition(), CCPoint(-1.5 * d1 * KK, 0))
										))); 
	solutionBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	solutionBtn->setVisible(gs == GS_Gameplay);
	
	
	
	replayBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(replayBtn->getPosition(), CCPoint(1.5 * d1, 0))
					))); 
	replayBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));

	replayBtn->setVisible(gs == GS_Gameplay);

	labelStat->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(labelStat->getPosition(), CCPoint(0, d1))
					))); 
	labelStat->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
	


#ifndef NO_IN_APPS
	labelUnlockOrRestore->runAction(CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME, ccpAdd(labelUnlockOrRestore->getPosition(), CCPoint(0, -d1)))));
#endif


	//labelStat->setVisible(gs == GS_Gameplay);
	if (gs == GS_Levels)
	{
		char buffer[64] = { 0 };
		sprintf(buffer, "Total score: %d", getTotalScore());
		labelStat->setString(buffer);
#ifndef NO_IN_APPS
		labelUnlockOrRestore->setString("Unlock levels or restore purchase");
		labelUnlockOrRestore->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));
#endif

	} else {
#ifndef NO_IN_APPS
		labelUnlockOrRestore->setOpacity(0);
#endif
	}

	closeBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(closeBtn->getPosition(), CCPoint(-1.5 * d1, 0))
					)));
	closeBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op2));

}

void HelloWorld::mainMenu(bool show) {
	mainmenu=0;
	if (show) {
		labelLevel->setVisible(false);
	}

	float d = !show ? KK * 1000 : KK * -1000;

	GLubyte op = show ? 200 : 56;
	float dd;
	if (!show) {
		dd = 0;
	} else {
		dd = -d;
	}

	menuPlay->setPosition(_W * 0.5 + dd, _H * 0.56);
	menuMoreGames->setPosition(_W * 0.5 - dd, _H * 0.4);
	
	menuLevels->setPosition(_W * 0.5 + dd, _H * 0.25);
	menuReset->setPosition(_W * 0.5 - dd, _H *0.125);


	gamecenterBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(gamecenterBtn->getPosition()
							, CCPoint(0, show ? KK * -500 : KK * 500))
					))); 

	soundBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(soundBtn->getPosition()
							, CCPoint(0, show ? KK * -300 : KK * 300))
					)));
	musicBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(musicBtn->getPosition()
							, CCPoint(0, show ? KK * -300 : KK * 300))
					)));
	
	
	
	proBtn->runAction(
						   CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
																  ccpAdd(proBtn->getPosition()
																		 , CCPoint(0, show ? KK * -300 : KK * 300))
																  )));

	facebookBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(facebookBtn->getPosition()
							, CCPoint(0, show ? KK * -300 : KK * 300))
					))); 
	twitterBtn->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(twitterBtn->getPosition()
							, CCPoint(0, show ? KK * -300 : KK * 300))
					))); 

	menuPlay->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(menuPlay->getPosition() , CCPoint(d, 0))
					))); 
	
	menuMoreGames->runAction(
						CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
										 ccpAdd(menuMoreGames->getPosition(), CCPoint(-d, 0)))
										 )); 
 	
	
	musicBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	soundBtn->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	
	
	menuPlay->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	menuMoreGames->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	
	menuLevels->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(menuLevels->getPosition(), CCPoint(d, 0)))
					)); 
	menuLevels->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	menuReset->runAction(
			CCEaseBackOut::create(CCMoveTo::create(MAIN_MENU_TIME,
					ccpAdd(menuReset->getPosition(), CCPoint(-d, 0))
					)));
	menuReset->runAction(CCFadeTo::create(MAIN_MENU_TIME, op));
	menuReset->setScale(KK

			);

	toGameplay(!show);

	if (gs == GS_Started) {
		runWithDelay(0.5f, callfunc_selector(HelloWorld::playSound));
	} else {
		playSound();
	}
}

void HelloWorld::setMainMenu() {
	mainmenu=0;
	gs = GS_MainMenu;
}

void HelloWorld::setGamePlay() {

	gs = GS_Gameplay;
}

void HelloWorld::runWithDelay(float delay, SEL_CallFunc func) {
	CCDelayTime *delayAction = CCDelayTime::create(delay);
	CCCallFunc *callSelectorAction = CCCallFunc::create(this, func);
	this->runAction(CCSequence::create(delayAction, callSelectorAction, NULL));
}

void HelloWorld::startShowLevel() {
	labelLevel->stopAllActions();
	labelLevel->setPosition(_W * 0.5 - KK * 1000, _H * 0.5);
	
	labelLevel->setVisible(gs == GS_Gameplay && !tilesUp);
	labelLevel->runAction(
			CCEaseBackOut::create(CCMoveTo::create(TILE_MOVE_TIME,
					ccpAdd(labelLevel->getPosition() , CCPoint(KK * 1000, 0)))
					));
	labelLevel->runAction(CCFadeTo::create(TILE_MOVE_TIME, 200));

	runWithDelay(TILE_MOVE_TIME * 2,
			callfunc_selector(HelloWorld::startHideLevel));
}

void HelloWorld::startHideLevel() {
	if (CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget(
			labelLevel) == 0) {
		labelLevel->runAction(
				CCEaseBackIn::create(CCMoveTo::create(TILE_MOVE_TIME,
						ccpAdd(labelLevel->getPosition() , CCPoint(KK * 1000, 0))
						)));
		labelLevel->runAction(CCFadeTo::create(TILE_MOVE_TIME, 50));
		labelLevel->setVisible(gs == GS_Gameplay && !tilesUp);
		if (showSolution)
		{
			flipToReplay();
			runWithDelay(1, callfunc_selector(HelloWorld::nextSolutionItem));
			_csolution = _solution;
			std::random_shuffle(_csolution.begin(), _csolution.end());
		}
			
	}

}

void HelloWorld::nextSolutionItem()
{
	if (_csolution.size() == 0)
	{
		showSolution = false;
		
		_score = 0;
		initDelays();
		tilesUp = true;
		runWithDelay(MAIN_MENU_TIME,
					 callfunc_selector(HelloWorld::nextLevel));

		playSound();
	}
	else
	{
		int x = _csolution.back_x();
		int y = _csolution.back_y();
		animateToggle(x, y);
		animateToggle(x + 1, y);
		animateToggle(x - 1, y);
		animateToggle(x, y + 1);
		animateToggle(x, y - 1);
		playSound();
		colors.toggle_cross_safe(x, y);
		_csolution.pop_back();
		runWithDelay(0.5, callfunc_selector(HelloWorld::nextSolutionItem));
	}
}

void setupLevel(std::vector<std::string>& level, int n)
{
	std::string o(n, '2');
	
	for (int i = 0; i < n; ++i)
	{
		level.push_back(o);
	}
}

void do_push(sk::polygon<int>& p, int i, int j)
{
	bool found = false;
	for (int k = 0; k < p.size(); ++k)
	{
		if (p[k].x == j && p[k].y == i)
		{
			found = true;
			if (k != (p.size() - 1))
			{
				p[k] = p.back();
			}
			p.pop_back();
			break;
		}
	}
	
	if (!found)
	{
		p.push_back(j, i);
	}
	
}
int do_flip(level_def& ld, int i, int j)
{
	if (i < 0 || j < 0) return 0;
	std::vector<std::string>& level = ld.tiles;
	int s = level.size();
	if (i >= s || j >= s) return 0;
	if (level[i][j] == '1')
	{
		level[i][j] = '2';
		return 1;
	}
	else
	if (level[i][j] == '2')
	{
		level[i][j] = '1';
		return 1;
	}
	return 0;
	
}


void flip(level_def& level, int i, int j)
{
	int res = 0;
	res |= do_flip(level, i, j);
	res |= do_flip(level, i + 1, j);
	res |= do_flip(level, i - 1, j);
	res |= do_flip(level, i, j - 1);
	res |= do_flip(level, i, j + 1);
	
	if (res)
	{
		do_push(level.transforms, i, j);
	}
}

void add_diag(level_def& ld)
{
	int s = ld.tiles.size();
	for (int i = 0; i < s; ++i)
	{
		flip(ld, i, i);
	}
}

void add_diag_a(level_def& ld)
{
	int s = ld.tiles.size();
	for (int i = 0; i < s; ++i)
	{
		flip(ld, i, s - i - 1);
	}
}

void add_row(level_def& ld, int r)
{
	int s = ld.tiles.size();
	for (int i = 0; i < s; ++i)
	{
		flip(ld, i, r);
	}
}

void add_column(level_def& ld, int r)
{
	int s = ld.tiles.size();
	for (int i = 0; i < s; ++i)
	{
		flip(ld, r, i);
	}
}

void add_corners(level_def& ld, int oofset = 0)
{
	int s = ld.tiles.size() - 1 - oofset;
	

	flip(ld, oofset, oofset);
	flip(ld, oofset, s);
	flip(ld, s, oofset);
	flip(ld, s, s);

}

void add_centers(level_def& ld, int offset = 0)
{
	int s = ld.tiles.size();
	int h = s >> 1;
	--s;
	
	
	flip(ld, h, offset);
	flip(ld, h, s - offset);
	
	flip(ld, offset, h);
	flip(ld, s - offset, h);

	
}


level_def HelloWorld::getLevelDef() {

	int ind = CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 0);

	level_def res;
	std::vector<std::string>& level = res.tiles;
	
	int s = 5;
	
	if (ind < 5)
	{
		s = 3;
	}
	else
	if (ind < 15)
	{
		s = 4;
	}
	setupLevel(level, s);
	if (ind == 0) {
		flip(res, 1, 1);
	} else if (ind == 1) {
		flip(res, 1, 2);
		flip(res, 2, 1);
	} else if (ind == 2) {
		add_diag(res);
	} else if (ind == 3) {
		flip(res, 1, 0);
		flip(res, 1, 2);
		flip(res, 0, 1);
		flip(res, 2, 1);
	} else if (ind == 4) {
		flip(res, 0, 1);
		flip(res, 1, 2);
		flip(res, 1, 0);
		flip(res, 2, 2);
		flip(res, 2, 0);
	} else if (ind == 5) {
		add_row(res, 0);
		add_column(res, 3);
	} else if (ind == 6) {
		add_diag(res);
		add_diag_a(res);
	} else if (ind == 7) {
		add_row(res, 1);
		add_diag(res);
	} else if (ind == 8) {

		add_row(res, 1);
		add_row(res, 2);
		add_diag(res);

	} else if (ind == 9) {
		add_corners(res);
		add_diag_a(res);
		add_diag(res);
		flip(res, 2, 2);
		flip(res, 3, 0);
		flip(res, 0, 3);
	} else if (ind == 10) {
		add_diag(res);
		flip(res, 1, 2);
		flip(res, 2, 1);
		flip(res, 3, 1);
		flip(res, 0, 2);
	} else if (ind == 11) {
		
		add_diag(res);
		flip(res, 1, 2);
		flip(res, 2, 1);
		flip(res, 3, 1);
		flip(res, 0, 2);
		add_corners(res);
		flip(res, 1, 1);
		flip(res, 2, 2);
	} else if (ind == 12) {
		flip(res, 1, 0 );
		flip(res, 2, 2);
		flip(res, 2, 3);
		flip(res, 2, 1);
		flip(res, 3, 2);

	} else if (ind == 13) {

		flip(res, 1, 0 );
		flip(res, 2, 2);
		flip(res, 2, 3);
		flip(res, 2, 1);
		flip(res, 3, 2);
		add_diag(res);
	} else if (ind == 14) {

		flip(res, 1, 0 );
		flip(res, 2, 2);
		flip(res, 2, 3);
		flip(res, 2, 1);
		flip(res, 3, 2);
		add_column(res, 0);
		add_row(res, 3);
	} else if (ind == 15) { /// 5*5
		
		add_column(res, 1);
		add_column(res, 3);
		add_row(res, 1);
		add_row(res, 3);
		add_diag_a(res);
		
	} else if (ind == 16) {

		add_diag_a(res);
		add_column(res, 0);
		flip(res, 1, 1 );
		flip(res, 0, 4 );
		flip(res, 2, 1 );
	} else if (ind == 17) {

		add_corners(res);
		add_corners(res,1);

	} else if (ind == 18) {
		add_column(res, 1);
		add_column(res, 3);
		add_row(res, 1);
		add_row(res, 3);
		add_corners(res,1);
	} else if (ind == 19) {

		add_column(res, 1);
		add_column(res, 3);
		add_row(res, 1);
		add_row(res, 3);
		add_centers(res, 1);
	} else if (ind == 20) {
		add_centers(res, 1);
		add_corners(res, 1);
	} else if (ind == 21) {

		
		add_centers(res, 1);
		add_corners(res);
		
	} else if (ind == 22) {
		add_row(res, 1);
		add_row(res, 3);
		add_column(res, 2);

	} else if (ind == 23) {
		add_column(res, 2);
		flip(res, 0, 0 );
		flip(res, 2, 2);
		flip(res, 4, 4);
	} else if (ind == 24) {
		add_column(res, 2);
		flip(res, 0, 0 );
		flip(res, 2, 3);
		flip(res, 2, 1);
	} else if (ind == 25) {
		add_column(res, 2);
		flip(res, 0, 1 );
		flip(res, 2, 3);
		flip(res, 4, 2);
		flip(res, 3, 2);
	} else if (ind == 26) {
		add_row(res, 1);
		add_row(res, 3);
		add_column(res, 3);
	} else if (ind == 27) {
		add_row(res, 1);
		add_row(res, 3);
		add_column(res, 3);
		flip(res, 0, 0 );
		flip(res, 2, 3);
		flip(res, 2, 1);
	} else if (ind == 28) {
		add_row(res, 1);
		add_row(res, 3);
		flip(res, 0, 1 );
		flip(res, 2, 3);
		flip(res, 4, 2);
		flip(res, 3, 2);
		add_column(res, 3);
	} else if (ind == 29) {
		add_row(res, 1);
		add_row(res, 3);
		add_column(res, 3);
		flip(res, 0, 1 );
		flip(res, 2, 3);
		flip(res, 3, 2);
	}

	else if (ind == 30) {

		add_row(res, 1);
		add_row(res, 3);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		
	} else if (ind == 31) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		
	} else if (ind == 32) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 1, 3);
		flip(res, 3, 1);
	} else if (ind == 33) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 3, 2);
		flip(res, 1, 2);
		
	} else if (ind == 34) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 0, 0);		
	} else if (ind == 35) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		add_diag_a(res);
		
	} else if (ind == 36) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 3, 4);
		flip(res, 2, 2);
				flip(res, 1, 4);
	} else if (ind == 37) {

		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 3, 2);
		flip(res, 1, 2);
		flip(res, 3, 4);
		flip(res, 2, 2);
		flip(res, 1, 4);
	} else if (ind == 38) {
		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 3, 2);
		flip(res, 1, 2);
		flip(res, 3, 4);
		flip(res, 2, 2);
		flip(res, 1, 4);
		flip(res, 0, 0);
	} else if (ind == 39) {
		add_row(res, 0);
		add_row(res, 2);
		add_row(res, 4);
		add_column(res, 0);
		add_column(res, 2);
		add_column(res, 4);
		add_corners(res, 1);
		add_centers(res, 1);
		flip(res, 3, 2);
		flip(res, 1, 2);
		flip(res, 2, 2);
		flip(res, 1, 4);
		flip(res, 0, 0);
		flip(res, 0, 3);
	} else {

	}
	sprintf(levelBuffer, "Level %d", ind + LEVEL_FIX);
	labelLevel->setString(levelBuffer);
	runWithDelay(TILE_MOVE_TIME, callfunc_selector(HelloWorld::startShowLevel));
	_score = 10000;
	updateStatLabel();
	_solution = res.transforms;
	int ss = _solution.size();
	return res;
}

void HelloWorld::loadLevel(const level_def& def) {
	tiles.init(def.tiles.front().size(), def.tiles.size());
	colors.init(tiles.width(), tiles.height());
	for (int i = 0; i < tiles.width(); ++i) {
		for (int j = 0; j < tiles.height(); ++j) {
			if (def.tiles[j][i] != '0') {
				tiles.set(i, j, true);
			}
			if (def.tiles[j][i] == '2') {
				colors.set(i, j, true);
			}
		}
	}
	colorsOrig = colors;
	/*
	for (int i = 0; i < def.transforms.size(); ++i) {
		int x = def.transforms[i].x;
		int y = def.transforms[i].y;
		if (tiles.is_valid_index(x, y) && tiles.at(x, y)) {
			colors.toggle_cross_safe(x, y);
		}
	}*/

}

