APP_STL := gnustl_shared
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1
APP_CFLAGS += -Wno-error=format-security
APP_PLATFORM := android-19