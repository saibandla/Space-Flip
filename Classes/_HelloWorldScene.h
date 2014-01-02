#ifndef __SK_MAIN_MENU_SCENE_H__
#define __SK_MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "sk_game_services/sk_game_services.h"

class SKMainMenu : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
	void on_take_photo(cocos2d::CCImage * photo);
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(SKMainMenu);
};

#endif // __SK_MAIN_MENU_SCENE_H__
