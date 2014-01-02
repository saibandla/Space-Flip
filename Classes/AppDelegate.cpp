#include "sk_game_services/sk_game_services.h"

#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SKGameMetadata.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"


USING_NS_CC;
using namespace CocosDenshion;

void space_flip_purchase_hints();

namespace sk
{
	namespace game_services
	{
		
		bool disable_all_inerestials()
		{
			// !!!! must be false in production code
			return false;
		}
		
		bool disable_banner_ads()
		{
			// !!!! be very careful with this code
			return true;
		}
		
		supported_orientations get_supported_orientation()
		{
			return SO_LANDSCAPE_ONLY;
		}
		
		bool disable_retina_ipad()
		{
			return true;
		}
		
		bool is_game_paid()
		{
#ifdef SK_PAID
			return true;
#else
			return false;
#endif
		}		
		
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		
		int desktop_prefered_windowed_height()
		{
			return 768;
		}
		
		const char * desktop_title()
		{
			return game_title();
		}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
      
        mac_mouse_handlers get_mac_mouse_handler()
        {
            return MAC_MOUSE_HANDLER_DRAGON;
        }
        
#endif
        
#ifndef SK_PAID
		const char * get_admob_mediation_id()
		{
			return ADMOB_MEDIATION_ID;
		}
#endif // #ifndef SK_PAID

		const char * get_cb_app_id()
		{
			return CB_APP_ID;
		}
		
		const char * get_cb_app_signature()
		{
			return CB_APP_SIGNATURE;
		}
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		
		const char * get_flurry_app_key()
		{
			return FLURRY_APP_KEY;
		}
		

		
		const char * get_revmob_app_id()
		{
			return REVMOB_APP_ID;
		}
		
		const char * get_adcolony_app_id()
		{
			return ADCOLONY_APPLICATION_ID;
		}
		
		const char * get_adcolony_zone()
		{
			return ADCOLONY_ZONE;
		}
		
		const char * get_playheaven_token()
		{
			return PLAYHEAVEN_TOKEN;
		}
		
		const char * get_playheaven_secret()
		{
			return PLAYHEAVEN_SECRET;
		}
		
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		
		const char * game_title()
		{
#ifndef SK_PAID
			return "Space Flip Free";
#else
			return "Space Flip";
#endif
		}
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)		
		const char * android_base64_encoded_public_key()
		{
			return ANDROID_BASE64_ENCODED_PUBLIC_KEY;
		}
		
		const char * android_share_score_subtitle()
		{
			return "play.google.com";
		}
		
		const char * android_share_score_description()
		{
			static char buffer[512];
			sprintf(buffer, "Get %s on Google Play. See screenshots and ratings, and read customer reivews", game_title());
			return buffer;
		}
		
		const char * android_share_score_picture_url()
		{
#ifndef SK_PAID
			return "http://a1864.phobos.apple.com/us/r1000/065/Purple2/v4/61/ea/14/61ea14c3-7f67-75d1-550d-c2b67210f611/mzl.ixwtajje.png";
#else
			return "http://a1524.phobos.apple.com/us/r1000/091/Purple/v4/d5/d0/92/d5d092c9-f0b7-af0a-649b-5b4e0bda2ec3/mzl.xyxlcukw.png";
#endif
		}
#endif
		
		void game_do_resume()
		{
			CCDirector::sharedDirector()->startAnimation();
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		
		void game_do_pause()
		{
			CCDirector::sharedDirector()->stopAnimation();
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}
		
} // namespace game_services
} // namspace sk

AppDelegate::AppDelegate() 
{

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{
	sk::game_services::init_info info;
	info.app_store_url = APP_STORE_URL;
	info.add_inapp(INAPP_UNLOCKALL_GAME_ID, INAPP_UNLOCKALL_STORE_ID, sk::game_services::on_unlock_all);
	
	info.add_leaderboard(DEFAULT_LEADERBOARD_GAME_ID, DEFAULT_LEADERBOARD_STORE_ID);
	info.add_inapp(INAPP_HINT_PACK_GAME_ID, INAPP_HINT_PACK_STORE_ID, space_flip_purchase_hints);
	
	
	sk::game_services::init(info);
	
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
	CCFileUtils::sharedFileUtils()->addSearchPath("assets");
	
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{

  fprintf(stderr,"minimized");

		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
//	if (!sk::game_services::is_interestial_shown())
//	{


		//sk::game_services::game_do_pause();

	//}
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	fprintf(stderr,"maximized");
//	sk::game_services::set_app_is_goint_to_bg(false);
////	if (!sk::game_services::is_interestial_shown())
////	{
////		sk::game_services::game_do_resume();
////	}
//
//	sk::game_services::game_do_resume();
//
//#ifndef SK_PAID
//	if (!sk::game_services::is_ads_removed() && !sk::game_services::is_interestial_shown())
//	{
//		sk::game_services::on_gift_pressed();
//	}
//#endif
}
