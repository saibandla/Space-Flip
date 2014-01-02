#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "sk_bitset_2d.h"
#include "AppDelegate.h"
#include "sk_polygons.h"
#include "sk_game_services/sk_game_services.h"

using namespace cocos2d;

//#define NO_IN_APPS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
   #define HIGSCORE_DISABLED
#else

#endif


#define AD_ENABLED
#define NUM_LEVLES_PLAYED_FOR_AD 5


struct level_def
{
	std::vector<std::string> tiles;
	sk::polygon<int> transforms;
};

enum GameState
{
	GS_Started,
	GS_MainMenu,
	GS_LevelSelect,
	GS_Gameplay,
	GS_About,
	GS_Levels
};

class HelloWorld : public cocos2d::CCLayer, sk::game_services::ui_purchase_observer, sk::game_services::ui_dialog_observer
{
public:
	HelloWorld();

	 void onPurchaseSuccess(bps_event_t *event);
	 void failureCommon(bps_event_t *event);
	 void onGetExistingPurchasesSuccess(bps_event_t *event);
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
	
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    virtual void on_purchase_ok        (const char * sku, unsigned  request_id);
    virtual void on_purchase_restore_ok(const char * sku);
    virtual void on_purchase_failed    (const char * sku, unsigned  request_id);
#endif

    virtual void update(float dt);
	
	void replay();
	void hideFullscreenQuad();
	void levelsBack();
	void fadeOutFullscreenQuad();
	virtual void ui_on_purchase(unsigned int game_id, bool purchased);
	virtual void ui_on_yes_no_dialog(unsigned int dialog_id, bool result);

	void resetLevels();
	
#ifndef NO_IN_APPS
	void callUnlock();
	void unlockLevels();
#endif
	void space_flip_purchase_hint();
	void purchaselock();
	void gift();
	void levelsDown();
	void setLevelLabelToLevel();
	void info();
	void solution();
	void level();
	void do_facebook();
	void facebook();
	void pro();
	void twitter();
	void toggleSound();
	void toggleMusic();
	
	void setMusic(bool on);
	void setSound(bool on);
	bool isMusic();
	bool isSound();
	
	void showGameCenter();
	void updateStatLabel();
	void setLevel(int level);
	void nextLevel();
	void startShowLevel();
	void startHideLevel();
	void doGameplay();
	void toGameplay(bool hide);
	bool isActive(int level);
	void setActive(int level, bool active);
	void setScore(int level, int score);
	int getScore(int level);
	
	void setHints(int hint);
	int getHints();
	void gift1();
	void updateHints();
	int getTotalScore();
	void initDelays();
	void runWithDelay(float delay, SEL_CallFunc func);
	void playSound();
	
	void nextSolutionItem();
	
	bool flipToReplay();
	void moveTile();
	void setMainMenu();
	void setGamePlay();
	void setLevels();
	void mainMenu(bool show);
	void addSprites();
	void animateToggle(int x, int y);
	level_def getLevelDef();
	void loadLevel(const level_def& def);
	CCAnimate * action_w2r;
	CCAnimate * action_r2w;
	
	std::vector<CCNode*> stiles;
	std::vector<float> delays;
	
	sk::bitset_2d colors;
	sk::bitset_2d colorsOrig;
	sk::bitset_2d tiles;
	
	CCLabelBMFont * menuPlay;
	CCLabelBMFont * menuMoreGames;
	CCLabelBMFont * menuLevels;
	CCLabelBMFont * menuReset;
	
	CCLabelBMFont * lblNumHints;
	
	CCLabelBMFont * labelStat;
#ifndef NO_IN_APPS
	CCLabelBMFont * labelUnlockOrRestore;
#endif
	
	CCLabelBMFont * labelLevel;
	
	CCMenuItemImage * replayBtn;
	
	CCMenuItemImage * infoBtn;
	CCMenuItemImage * solutionBtn;
	CCMenuItemImage * giftBtn;
	CCMenuItemImage * levelBtn;
	
	CCMenuItemImage * closeBtn;

#ifndef HIGSCORE_DISABLED

#endif
	CCMenuItemImage * gamecenterBtn;
	CCMenuItemToggle * soundBtn;
	CCMenuItemToggle * musicBtn;
	
	
	CCMenuItemImage * proBtn;
	CCMenuItemImage * facebookBtn;
	CCMenuItemImage * twitterBtn;
	
	GameState gs;
	
	bool tilesUp;
	
	//CCSprite * fullscreenQuad;
	
	CCTexture2D t;
	
	bool showSolution;
	
	bool markBack;
	int _score;
	char levelBuffer[16];
	
#ifdef AD_ENABLED
	int _playedLevels;
#endif
	int min;
	sk::polygon<int> _solution;
	sk::polygon<int> _csolution;
};

#endif // __HELLOWORLD_SCENE_H__
