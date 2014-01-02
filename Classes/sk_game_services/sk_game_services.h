#pragma once

#ifndef _SK_GAME_SERVICES__
#define _SK_GAME_SERVICES__

#include <string>
#include <vector>

namespace cocos2d
{
	class CCImage;
}

namespace sk
{
	
namespace game_services
{

	typedef void (*game_logic_purchase_callback)();
	
	class ui_purchase_observer
	{
	public:
		virtual void ui_on_purchase(unsigned int game_id, bool purchased) = 0;
	};
	
	
	class ui_dialog_observer
	{
	public:
		virtual void ui_on_yes_no_dialog(unsigned int dialog_id, bool result) = 0;
	};
	
	enum supported_orientations
	{
		SO_UNDEFINED,
		SO_PORTRAIT_ONLY,
		SO_LANDSCAPE_ONLY
		// do not support both at the moment
	};
    
    enum mac_mouse_handlers
    {
        MAC_MOUSE_HANDLER_COCOS_STANDART,
        MAC_MOUSE_HANDLER_DRAGON
    };
	
	enum dialog_type
	{
		DIALOG_TYPE_YES_NO
	};
	
	struct leaderboard
	{
		unsigned int game_id;
		std::string store_id;
	};
	
	struct inapp
	{
		unsigned int game_id;
		std::string store_id;
		bool        confiremd_by_store;
		bool        purchased;
		game_logic_purchase_callback logic_on_purchase; // this is only positive login for non-ui stuff
	};
	
	struct init_info
	{
		inline init_info()
		{
			
		}
		
		void add_leaderboard(int game_id, const char * store_id);
		void add_inapp(int game_id, const char * store_id, game_logic_purchase_callback logic);
		
		std::string app_store_url;		
		std::vector<leaderboard> leaderboards;
		std::vector<inapp>       inapps;
	};
	
	enum ImageSource
	{
		ImageSourceUndefined,
		ImageSourceCamera,
		ImageSourcePhotoLibrary
	};
	
	class image_observer
	{
	public:
		virtual void on_get_image(cocos2d::CCImage * photo, ImageSource source) = 0;
	};
	

	
	struct image_info
	{
		inline image_info()
		:
		source(ImageSourceUndefined),
		 width(-1),
		height(-1)
		{
			
		}
		ImageSource source;
		int width;
		int height;
	};
	// return true if started taking photo
	bool get_image(image_observer * observer, const image_info& info = image_info());
	bool device_can_take_photo();
	void on_take_image(cocos2d::CCImage * photo);
	
	const image_info& get_image_info();
	

	void init(const init_info& info);	
	void on_rate_me_pressed();
	void show_more_games();
	void log_event(const char * event);
	void on_level_ended();
	void show_video_ad(bool respect_ads_removed = true);
	void on_gift_pressed();
	void on_tv_pressed();
	
	bool is_paused();
	void set_paused(bool paused);
	
	bool app_is_going_to_bg();
	void set_app_is_goint_to_bg(bool to_bg);
	
	int get_banner_height();
	
	void share_game_score_facebook(const char * message);
	void share_game_score_twitter (const char * message);
	void share_image_facebook(cocos2d::CCImage * image, const char * message);
	void save_image_to_photos(cocos2d::CCImage * image);
	
	struct email_info
	{
		inline email_info()
		:
		photo(0)
		{
		}
		std::string to;
		std::string subject;
		std::string message;
		cocos2d::CCImage * photo;
	};
	
	void send_email(const email_info& info);
	bool can_send_email();
	
	bool show_leaderboard(int leaderboars_id);
	bool submit_score(int leaderboars_id, unsigned int score);
	const char * get_leaderboard_store_id(unsigned int leaderboard_id);
	
	void on_launch();
	
	bool is_ads_removed();
	void on_ads_removed();
	
	void on_unlock_all();
	bool is_all_unlocked();
	
	// for testing purposes only
	void clear_unlock_all();
	
	void restore_purchases();	
	int get_total_inapps();
	const char * get_inapp_store_id(int index);
	
	bool buy_product(unsigned int game_id);
	void on_purchase(const char * store_id, bool purchased);
	int get_index_for_in_app(const char * store_id);
	void on_inapp_confirmed_by_store(const char * store_id);
	void   register_ui_purchase_observer(ui_purchase_observer * observer);
	void unregister_ui_purchase_observer(ui_purchase_observer * observer);
	
	const char * game_title();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char * android_base64_encoded_public_key();
	const char * android_share_score_subtitle();
	const char * android_share_score_description();
	const char * android_share_score_picture_url();
#endif
	
	const char * get_app_store_url();
	const char * get_flurry_app_key();
	const char * get_cb_app_id();	
	const char * get_cb_app_signature();
	const char * get_revmob_app_id();
	const char * get_adcolony_app_id();
	const char * get_adcolony_zone();
	const char * get_playheaven_token();
	const char * get_playheaven_secret();
	
	void set_cb_shown(bool);
	void set_revmob_shown(bool);
	void set_ph_shown(bool);
	
	bool is_interestial_shown();	
	bool disable_all_inerestials();
	bool disable_banner_ads();
#ifndef SK_PAID
	const char * get_admob_mediation_id();
#endif
	
	supported_orientations get_supported_orientation();
	bool disable_retina_ipad();
	bool is_game_paid();
	void open_url(const char * url);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	int windowed_width();
	int windowed_height();
	int desktop_width();
	int desktop_height();
	int desktop_prefered_windowed_height();
	const char * desktop_title();	
#endif
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	bool mac_gamecenter_supported();
    mac_mouse_handlers get_mac_mouse_handler();
#endif
	
	
	// all dialogs are modal
	bool open_yes_no_dialog(unsigned int dialog_id, ui_dialog_observer * observer, const std::string& message, const std::string& title = "", const std::string& yes_text = "Yes", const std::string& no_text = "No");
	//bool open_yes_no_dialog(unsigned int dialog_id, ui_dialog_observer * observer, const std::string& message, const std::string& title, const std::string& yes_text, const std::string& no_text)

	void set_yes_no_dialog_result(bool result);
		
	
	
} // namespace game_servies
	
} // namespace sk



#endif // _SK_GAME_SERVICES__
