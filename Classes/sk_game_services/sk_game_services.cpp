#include "SimpleAudioEngine.h"
#include "sk_game_services.h"
#include "cocos2d.h"
#include "bps/navigator.h"
#include "bps/navigator_invoke.h"
using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void ios_gc_init();
	bool ios_gc_show_leaderboard(const char * leaderboard);
	void ios_gc_submit_score(const char * cat, unsigned int value);
	void ios_inapps_init();
	bool ios_buy_product(const char * store_id);
	void ios_restore_purchases();
	void ios_on_ads_remove();
	void ios_play_adcolony();
	void ios_show_cb();
	void ios_show_ph();
	void ios_show_revmob();
	void ios_open_url(const char * url);
	void ios_show_more_games();
	void ios_log_event(const char * event);
	void ios_share_facebook(const char * message);
	void ios_share_twitter (const char * message);
	void ios_share_image_facebook(cocos2d::CCImage * image, const char * message);
	void ios_save_image_to_photos(cocos2d::CCImage * image);
	bool ios_device_can_take_photo();
	void ios_take_image();
	void ios_start_yes_no_dialog(const std::string& message, const std::string& title, const std::string& yes_text, const std::string& no_text);
	void ios_share_email(const sk::game_services::email_info& info);
	bool ios_can_send_email();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	void mac_inapps_init();
	bool mac_buy_product(const char * store_id);
	void mac_restore_purchases();
	void mac_gc_init();
	bool mac_gc_show_leaderboard(const char * leaderboard);
	void mac_gc_submit_score(const char * cat, unsigned int value);
	void mac_open_url(const char * url);
	void mac_start_yes_no_dialog(const std::string& message, const std::string& title, const std::string& yes_text, const std::string& no_text);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool android_device_can_take_photo();
	bool android_take_photo();
	void android_start_yes_no_dialog(const std::string& message, const std::string& title, const std::string& yes_text, const std::string& no_text);
	void android_share_facebook_score(const char * message);
	bool android_share_image_facebook(cocos2d::CCImage * image, const char * message);
	bool android_save_image_to_photos(cocos2d::CCImage * image);
	void android_share_twitter (const char * message);
	void android_facebook_post_wall(const char * message, const char * url, const char * title, const char * subtitle, const char * description, const char * picture_url);
	void android_inapps_init();
	bool android_buy_product(const char * store_id);
	void android_restore_purchases();
	void android_share_email(const sk::game_services::email_info& info);
	bool android_can_send_email();
	bool android_gc_show_leaderboard(const char * leaderboard);
	void android_gc_submit_score(const char * cat, unsigned int value);
	void android_show_cb();
#endif

using namespace cocos2d;

namespace sk
{
	
namespace game_services
{
static bool APP_IS_GOING_TO_BG = false;
static bool PAUSED = false;
static init_info info;
	
	
static bool CB_SHOWN = false;
static bool RM_SHOWN = false;
static bool PH_SHOWN = false;
	
static image_observer * _image_observer = 0;
static ui_dialog_observer * _dialog_observer = 0;
static unsigned int _dialog_id = 0;
static image_info _image_info;
	
bool app_is_going_to_bg()
{
	return APP_IS_GOING_TO_BG;
}
	
void set_app_is_goint_to_bg(bool to_bg)
{
	APP_IS_GOING_TO_BG = to_bg;
}
	
void set_cb_shown(bool shown)
{
	CB_SHOWN = shown;
}
	
void set_revmob_shown(bool shown)
{
	RM_SHOWN = shown;
}
	
void set_ph_shown(bool shown)
{
	PH_SHOWN = shown;
}
	
bool is_interestial_shown()
{
	return CB_SHOWN || RM_SHOWN || PH_SHOWN;
}
	
bool is_paused()
{
	return PAUSED;
}
	
void game_do_resume();
void game_do_pause();
	
void set_paused(bool paused)
{
	if (paused == PAUSED)
	{
		return;
	}
	PAUSED = paused;
	
	if (PAUSED)
	{
		game_do_pause();
	}
	else
	{
		game_do_resume();
	}
}
	
int get_banner_height()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int W = (int)size.width;
	int H = (int)size.height;
	
	int m = W > H ? W : H;
	int _m = W < H ? W : H;
	
	// TODO : make this function better and take into account device orietation,
	// now all values will be ok only for portrait orientiaon
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (m == 480)
	{
		return 50;
	}
	if (m == 960 || m == 1136)
	{
		return 100;
	}
	if (m == 2048)
	{
		return 66 * 2;
	}
	return 66;
#endif	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (480 == _m)
	{
		return 75;
	}
	if (m < 720)
	{
		return 50;
	}
	if (m > 1400)
	{
		return 90 * 2;
	}
	else
	{
		return 90;
	}

#endif	
	return 50;
}
	
void share_game_score_facebook(const char * message)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_share_facebook(message);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_share_facebook_score(message);
#endif
}
	
void share_game_score_twitter (const char * message)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_share_twitter(message);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_share_twitter(message);
#endif
}
	
void share_image_facebook(cocos2d::CCImage * image, const char * message)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_share_image_facebook(image, message);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_share_image_facebook(image, message);
#endif
}
	
void save_image_to_photos(cocos2d::CCImage * image)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_save_image_to_photos(image);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_save_image_to_photos(image);
#endif
}
	
void send_email(const email_info& info)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_share_email(info);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_share_email(info);
#endif
}
	
bool can_send_email()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_can_send_email();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_can_send_email();
#endif
}
	
void init(const init_info& _info)
{
	// a bit dirty, all strings in _info must be compile time constant strings !!!
	info = _info;
	if (info.leaderboards.size() > 0)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ios_gc_init();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		mac_gc_init();
#endif
	}
	
	if (info.inapps.size() > 0)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ios_inapps_init();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		mac_inapps_init();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		android_inapps_init();
#endif
	}
}
	
void init_info::add_leaderboard(int game_id, const char * store_id)
{
	CCAssert(game_id == leaderboards.size(), "Leaderboard ids must be consequtive integers, e.g. 0, 1, 2, etc.");
	leaderboards.push_back(leaderboard());
	leaderboards.back().game_id = game_id;
	leaderboards.back().store_id = std::string(store_id);
}

void init_info::add_inapp(int game_id, const char * store_id, game_logic_purchase_callback logic)
{
	CCAssert(game_id == inapps.size(), "In-App ids must be consequtive integers, e.g. 0, 1, 2, etc.");
	CCAssert(logic,                    "In-App must be valid function pointer");	
	inapps.push_back(inapp());
	inapps.back().game_id = game_id;
	inapps.back().store_id = std::string(store_id);
	inapps.back().confiremd_by_store = false;
	inapps.back().purchased = false;
	inapps.back().logic_on_purchase = logic;
}

int get_total_inapps()
{
	return info.inapps.size();
}
	
const char * get_inapp_store_id(int index)
{
	if (index < 0 || index >= info.inapps.size())
	{
		CCLOG("In-App Purchases : get_inapp_store_id - incorrect index : %d", index);
		return 0;
	}
	return info.inapps[index].store_id.c_str();
}
	
int get_index_for_in_app(const char * store_id)
{
	std::string s(store_id);
	for (size_t i = 0; i < info.inapps.size(); ++i)
	{
		if (info.inapps[i].store_id == s)
		{
			return i;
		}
	}
	return -1;
}

	
	
static std::vector<ui_purchase_observer *> ui_purchase_observers;
	
	
static int get_observer_index(ui_purchase_observer * observer)
{
	for (size_t i = 0; i < ui_purchase_observers.size(); ++i)
	{
		if (observer == ui_purchase_observers[i])
		{
			return i;
		}
	}
	return -1;
}
	
void register_ui_purchase_observer(ui_purchase_observer * observer)
{
	int index = get_observer_index(observer);
	if (-1 != index)
	{
		return;
	}
	ui_purchase_observers.push_back(observer);
}
	
void unregister_ui_purchase_observer(ui_purchase_observer * observer)
{
	int index = get_observer_index(observer);
	if (-1 == index)
	{
		return;
	}
	if (index != ui_purchase_observers.size() - 1)
	{
		ui_purchase_observers[index] = ui_purchase_observers.back();
	}
	ui_purchase_observers.pop_back();
}
	
void on_inapp_confirmed_by_store(const char * store_id)
{
	int index = get_index_for_in_app(store_id);
	if (-1 == index)
	{
		CCLOG("In-App Purchases : ATTENTION : product %s exists in the application store, but NOT in the app!", store_id);
		return;
	}
	info.inapps[index].confiremd_by_store = true;
	CCLOG("In-App Purchases : product %s confirmed by application store", store_id);
}
	
bool buy_product(unsigned int game_id)
{
	if (game_id >= info.inapps.size())
	{
		CCLOG("In-App Purchases : buy_product - incorrect game_id : %d. Check you ids enum declaration and in-apps initialization code.", game_id);
		return false;
	}
	
	if (!info.inapps[game_id].confiremd_by_store)
	{
		CCLOG("In-App Purchases : buy_product - product %s is NOT confirmed by application store. Check you store account and initialization code.", info.inapps[game_id].store_id.c_str());
		return false;
	}
	
	if (info.inapps[game_id].purchased)
	{
		for (int i = 0; i < ui_purchase_observers.size(); ++i)
		{
			ui_purchase_observers[i]->ui_on_purchase(game_id, true);
		}
		return true;
	}
	
	const char * store_id = info.inapps[game_id].store_id.c_str();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return ios_buy_product(store_id);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	return mac_buy_product(store_id);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return android_buy_product(store_id);
#endif
	return false;
}
	
void on_purchase(const char * store_id, bool purchased)
{
	int index = get_index_for_in_app(store_id);
	if (-1 == index)
	{
		CCLOG("In-App Purchases : ATTENTION : product %s exists in the application store, but NOT in the app!", store_id);
		return;
	}
	
	if (purchased)
	{
		CCLOG("In-App Purchases : purchase succesful : %s", store_id);
	}
	else
	{
		CCLOG("In-App Purchases : purchase failed : %s", store_id);
	}
	
	if (purchased)
	{
		info.inapps[index].purchased = true;
		if (info.inapps[index].logic_on_purchase)
		{
			info.inapps[index].logic_on_purchase();
		}
	}
	
	for (int i = 0; i < ui_purchase_observers.size(); ++i)
	{
		ui_purchase_observers[i]->ui_on_purchase(index, purchased);
	}	
}

bool show_leaderboard(int leaderboars_id)
{
	const char * l = get_leaderboard_store_id(leaderboars_id);
	if (!l)
	{
		return false;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return ios_gc_show_leaderboard(l);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	return mac_gc_show_leaderboard(l);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return android_gc_show_leaderboard(l);
#endif
	return false;
}
	
bool submit_score(int leaderboars_id, unsigned int score)
{
	const char * l = get_leaderboard_store_id(leaderboars_id);
	if (!l)
	{
		return false;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_gc_submit_score(l, score);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	mac_gc_submit_score(l, score);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_gc_submit_score(l, score);
#endif
	return true;
}
	
const char * get_leaderboard_store_id(unsigned int leaderboard_id)
{
	for (size_t i = 0; i < info.leaderboards.size(); ++i)
	{
		if (info.leaderboards[i].game_id == leaderboard_id)
		{
			return info.leaderboards[i].store_id.c_str();
		}
	}
	return 0;
}
	
	
const char * get_app_store_url()
{
	return info.app_store_url.c_str();
}
	

void on_ads_removed()
{
	if (is_ads_removed())
	{
		return;
	}
	log_event("on_ads_removed");
	CCUserDefault::sharedUserDefault()->setBoolForKey("qiur3df1e3", true);
	CCUserDefault::sharedUserDefault()->flush();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_on_ads_remove();
#endif	
}

bool is_ads_removed()
{
	if (is_game_paid())
	{
		return true;
	}
	return CCUserDefault::sharedUserDefault()->getBoolForKey("qiur3df1e3", false);
}

void on_unlock_all()
{
	if (is_all_unlocked())
	{
		return;
	}
	log_event("on_unlock_all");
	CCUserDefault::sharedUserDefault()->setBoolForKey("qiur3df1e", true);
	CCUserDefault::sharedUserDefault()->flush();
}

void clear_unlock_all()
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("qiur3df1e", false);
	CCUserDefault::sharedUserDefault()->flush();
}

bool is_all_unlocked()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("qiur3df1e", false);											
}
	
void show_more_games()
{
	log_event("show_more_games");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
	ios_show_more_games();
#endif	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	mac_open_url("http://llcdraco.com");
#endif
}
	
void log_event(const char * event)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)		
	ios_log_event(event);
#endif		
}
	
	
void on_level_ended()
{
	log_event("level_ended");
	if (is_game_paid())
	{
		return;
	}	
	if (is_ads_removed())
	{
		return;
	}
	static int num_played = 0;
	static int k = 1;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int n = num_played % (25 * k);
	if (6 * k == n || 15 * k== n)
	{
		ios_show_cb();
	}
	else
	if (11 * k== n || 20 * k== n)
	{
		ios_show_ph();		
	}
	else
	if (24 * k== n)
	{
		ios_show_revmob();
	}
#endif
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	int n = num_played % 5;
	if (4 == n)
	{
		android_show_cb();
	}
#endif
	
	++num_played;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	
void show_video_ad(bool respect_ads_removed)
{
	if (respect_ads_removed && is_ads_removed())
	{
		return;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_play_adcolony();
#endif
}
	
void restore_purchases()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_restore_purchases();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	mac_restore_purchases();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_restore_purchases();
#endif
}
	
void on_tv_pressed()
{
	show_video_ad(false);
}
	
void on_gift_pressed()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_show_revmob();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	show_more_games();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_show_cb();
#endif
}
	
void on_launch()
{
	if (is_ads_removed())
	{
		return;
	}
	if (is_game_paid())
	{
		return;
	}
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_show_revmob();
	//ios_show_ph();
	//ios_show_cb();
#endif
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_show_cb();
#endif	
}
	
void open_url(const char * url)
{
	if (!url)
	{
		return;
	}
 navigator_invoke_invocation_t *invoke = NULL;
	//navigator_stop_events(0);
	    navigator_invoke_invocation_create(&invoke);

	    navigator_invoke_invocation_set_target(invoke, "sys.browser");
	    navigator_invoke_invocation_set_action(invoke, "bb.action.OPEN");

	    navigator_invoke_invocation_set_uri(invoke, url);

	    navigator_invoke_invocation_send(invoke);

	    navigator_invoke_invocation_destroy(invoke);

	//    if (paymentservice_purchase_request(digita

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_open_url(url);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	mac_open_url(url);
#endif
}
	
void on_rate_me_pressed()
{
	log_event("rate_me_pressed");
	open_url(get_app_store_url());
}
	
bool device_can_take_photo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return ios_device_can_take_photo();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return android_device_can_take_photo();
#endif
	
	return false;
}
	
const image_info& get_image_info()
{
	return _image_info;
}
	
bool get_image(image_observer * observer, const image_info& info)
{
	if (_image_observer)
	{
		return false;
	}
	if (ImageSourceUndefined == info.source)
	{
		return false;
	}
	if (ImageSourceCamera == info.source && !device_can_take_photo())
	{
		return false;
	}
	_image_info = info;
	_image_observer = observer;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	 
	ios_take_image();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_take_photo();
#endif
	return true;
}
	
void on_take_image(cocos2d::CCImage * photo)
{
	if (_image_observer)
	{
		_image_observer->on_get_image(photo, get_image_info().source);
	}
	else
	{
		if (photo)
		{
			delete photo;
		}
	}
	_image_observer = 0;
}
	
bool open_yes_no_dialog(unsigned int dialog_id, ui_dialog_observer * observer, const std::string& message, const std::string& title, const std::string& yes_text, const std::string& no_text)
{
	if (_dialog_observer)
	{
		return false;
	}
	_dialog_id = dialog_id;
	_dialog_observer = observer;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ios_start_yes_no_dialog(message, title, yes_text, no_text);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	mac_start_yes_no_dialog(message, title, yes_text, no_text);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	android_start_yes_no_dialog(message, title, yes_text, no_text);
#endif
	return true;
}
	
void set_yes_no_dialog_result(bool result)
{
	if (_dialog_observer)
	{
		_dialog_observer->ui_on_yes_no_dialog(_dialog_id, result);
	}
	_dialog_id = 0;
	_dialog_observer = 0;
}
	
} // namespace game_services
	
} // namespace sk
