#pragma once

#ifndef  __SK_GAME_METADATA_H__
#define  __SK_GAME_METADATA_H__

#define DEFAULT_LEADERBOARD_GAME_ID 0

enum DIALOGS
{
	DIALOG_RESET_GAME_PROGRESS,
	DIALOG_CONFIRM_FACEBOOK
};

enum INAPPS
{
#ifndef SK_PAID
//	INAPP_REMOVE_ADS_GAME_ID,
#endif
	INAPP_UNLOCKALL_GAME_ID,
	INAPP_HINT_PACK_GAME_ID
};




#ifndef SK_PAID  // Free



#define APP_STORE_URL                               "https://itunes.apple.com/us/app/space-flip-free/id595616630?ls=1&mt=8"
#define DEFAULT_LEADERBOARD_STORE_ID                "grp.leader"
#define INAPP_UNLOCKALL_STORE_ID                    "com.llcdraco.flippmaniafree.unlockall"
#define INAPP_HINT_PACK_STORE_ID                    "com.llcdraco.flippmaniafree.hints0"
#define FLURRY_APP_KEY                              "8QZFBS8ZP2V76WC5G438"
#define CB_APP_ID                                   "50ff29a517ba471f5e000045"
#define CB_APP_SIGNATURE                            "a83b41241f019a2e0af76c647fc4c89b8165546f"
#define REVMOB_APP_ID                               "50ff2a42271f801100000014"
#define ADMOB_MEDIATION_ID                          "7249465349184cb0"
#define ADCOLONY_APPLICATION_ID                     "app60e374da1ff04c60914a7c"
#define ADCOLONY_ZONE                               "vzf128e42f9186469e981ea4"
#define PLAYHEAVEN_TOKEN                            "7777526414634f2b989340f6725fb9ec"
#define PLAYHEAVEN_SECRET                           "669de9a2c1c24a41911b89c4fdafdeb4"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // Free
#define APP_STORE_URL                               "https://itunes.apple.com/us/app/space-flip-free/id595616630?ls=1&mt=8"
#define DEFAULT_LEADERBOARD_STORE_ID                "grp.leader"
#define INAPP_UNLOCKALL_STORE_ID                    "com.llcdraco.flippmaniafree.unlockall"
#define INAPP_HINT_PACK_STORE_ID                    "com.llcdraco.flippmaniafree.hints0"
#define FLURRY_APP_KEY                              "8QZFBS8ZP2V76WC5G438"
#define CB_APP_ID                                   "50ff29a517ba471f5e000045"
#define CB_APP_SIGNATURE                            "a83b41241f019a2e0af76c647fc4c89b8165546f"
#define REVMOB_APP_ID                               "50ff2a42271f801100000014"
#define ADMOB_MEDIATION_ID                          "7249465349184cb0"
#define ADCOLONY_APPLICATION_ID                     "app60e374da1ff04c60914a7c"
#define ADCOLONY_ZONE                               "vzf128e42f9186469e981ea4"
#define PLAYHEAVEN_TOKEN                            "7777526414634f2b989340f6725fb9ec"
#define PLAYHEAVEN_SECRET                           "669de9a2c1c24a41911b89c4fdafdeb4"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // Free

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Free
#define APP_STORE_URL                               "https://play.google.com/store/apps/details?id=com.llcdraco.spaceflippaid&hl=en"
#define INAPP_UNLOCKALL_STORE_ID                    "unlockalllevels"
#define INAPP_HINT_PACK_STORE_ID                    "hintpack"
//#define ANDROID_BASE64_ENCODED_PUBLIC_KEY           "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAodtRdDBzUXMwWnylXI9h5KUWpYzpfFvOSl5XZiHmbWZNlBanGNnO1nKP4+qrkFCwBb5NXFhG45jD9ZHyqvk8lyUkwUZAlksI0j/Rx0yZXyIOHC0dZIhIPMTIJqcYP3LwYwsHsIKHOVZTk4JpIqBrVg/yebJZN2I6seugbC5eeRrMTHX4KJVN0omvUutZ8cDAg7N+kf43uc/0Lzw/AZvdKC/0HZ36lsu2ZKoytr3/BZL5h4isYwzy/42l/Ar7MV7fV+S0T3twv+4CK1zbO2XjEvCbpxAnTLH/qj41eB0sLpjFlY0G3FOJXayRBkQ8KJ6lHYpHE3CFQBmpVPOe5zQ6twIDAQAB";
#define ANDROID_BASE64_ENCODED_PUBLIC_KEY           "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhd0DngqzaC/q6VMss9+r3xhCyg+9nFLguZdPN6nu/7wlnG+FtTpU3OK9kpD4PAMUpHeStRdVLooQlS2oGyBDgCHlkiTxWBQx4KEFmB+GS47RsKhe0YSH/DkOE9iQ+8Uj705sMJSusBc5FOPd7fqquq3IpCr9Jq+Ed/+0fFWWEFfzWnAu0q9y/+zPlFpyNXKPiT6AbeSuRCWyNPz6CTJC4/7paYNC708OS18BBBWi/O/iC4jQ2/jq9PhbGhj+vLeXoA7nNs58E9AQ//skmCFJwwVJWTM6eKHEQZKntlUlyJLPRl0Gm+zhhJB9jrk3QwbiajZy3vNRS3J+KhuVsm3FwQIDAQAB"
#define ADMOB_MEDIATION_ID                          "invalid"
//"459038b9996a40ee"
#define DEFAULT_LEADERBOARD_STORE_ID                "8591"
#define CB_APP_ID                                   "51710a4516ba470b0b000001"
#define CB_APP_SIGNATURE                            "514599469d204bfdf03eb54ae9068c4f825e2c54"

#else

#endif

#else // #ifdef SK_PAID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // Paid
#define APP_STORE_URL                               "http://itunes.apple.com/us/app/space-flip/id586197340?ls=1&mt=8"
#define DEFAULT_LEADERBOARD_STORE_ID                "leader"
#define INAPP_UNLOCKALL_STORE_ID                    "com.llcdraco.flippmania.unlockall"
#define INAPP_HINT_PACK_STORE_ID                    "com.llcdraco.flippmania.hints0"
#define FLURRY_APP_KEY                              "9R7T4YTS98XNSD96S7RK"
#define CB_APP_ID                                   "50ff289e16ba473141000053"
#define CB_APP_SIGNATURE                            "c89cc13a4017388caf28fcaed5ed33da2f51cca1"
#define REVMOB_APP_ID                               "50ff2a075b7ff30e0000003e"
#define ADCOLONY_APPLICATION_ID                     "app585115b3a3de47eb8cb910"
#define ADCOLONY_ZONE                               "vzf709bea8b99446cca342c7"
#define PLAYHEAVEN_TOKEN                            "636e3ec90dc74dffa7e3125d27c2764f"
#define PLAYHEAVEN_SECRET                           "5daf578f1b844c678d2decb5969bf5ed"


#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // Paid

#define APP_STORE_URL                               "http://itunes.apple.com/us/app/space-flip/id628533020?ls=1&mt=12"
#define INAPP_UNLOCKALL_STORE_ID                    "unlockalllevels"
#define INAPP_HINT_PACK_STORE_ID                    "hintpack"
#define DEFAULT_LEADERBOARD_STORE_ID                "com.dracollc.spaceflippaidmac.leader"
#define INAPP_UNLOCKALL_STORE_ID                    "com.llcdraco.spaceflipmacpaid.unlockall"
#define INAPP_HINT_PACK_STORE_ID                    "com.llcdraco.spaceflipmacpaid.hints0"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // Paid

#define APP_STORE_URL                               "https://play.google.com/store/apps/details?id=com.llcdraco.spaceflippaid&hl=en"
#define ANDROID_BASE64_ENCODED_PUBLIC_KEY           "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhd0DngqzaC/q6VMss9+r3xhCyg+9nFLguZdPN6nu/7wlnG+FtTpU3OK9kpD4PAMUpHeStRdVLooQlS2oGyBDgCHlkiTxWBQx4KEFmB+GS47RsKhe0YSH/DkOE9iQ+8Uj705sMJSusBc5FOPd7fqquq3IpCr9Jq+Ed/+0fFWWEFfzWnAu0q9y/+zPlFpyNXKPiT6AbeSuRCWyNPz6CTJC4/7paYNC708OS18BBBWi/O/iC4jQ2/jq9PhbGhj+vLeXoA7nNs58E9AQ//skmCFJwwVJWTM6eKHEQZKntlUlyJLPRl0Gm+zhhJB9jrk3QwbiajZy3vNRS3J+KhuVsm3FwQIDAQAB"
#define ADMOB_MEDIATION_ID                          "459038b9996a40ee"
#define DEFAULT_LEADERBOARD_STORE_ID                "8591"
#define CB_APP_ID                                   "51710a4516ba470b0b000001"
#define CB_APP_SIGNATURE                            "514599469d204bfdf03eb54ae9068c4f825e2c54"

#else
#error "SKGameMetadata does not support this platfrom, fix it"
#endif


#endif // #ifdef SK_PAID

#endif // __SK_GAME_METADATA_H__

