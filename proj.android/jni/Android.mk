LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
				   ../../Classes/sqlite3.c \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BackgroundLayer.cpp \
                   ../../Classes/Bujian.cpp \
                   ../../Classes/Character.cpp \
                   ../../Classes/HanziManage.cpp \
                   ../../Classes/HcharacterDrawnode.cpp \
                   ../../Classes/HcharacterLayer.cpp \
                   ../../Classes/HclcData.cpp \
                   ../../Classes/JudgeManager.cpp \
                   ../../Classes/LianxiScene.cpp \
                   ../../Classes/LuaScriptReader.cpp \
                   ../../Classes/MainScene.cpp \
                   ../../Classes/PopLayer.cpp \
                   ../../Classes/ReadXML.cpp \
                   ../../Classes/Splash.cpp \
                   ../../Classes/Stroke.cpp \
                   ../../Classes/StrokeDrawnode.cpp \
                   ../../Classes/StrokeNode.cpp \
                   ../../Classes/strokeFunc.cpp \
                   ../../Classes/SQLiteData.cpp \
                   ../../Classes/TcharacterDrawnode.cpp \
                   ../../Classes/TcharacterLayer.cpp \
                   ../../Classes/TouchLayer.cpp \
                   ../../Classes/WallScene.cpp \
                   ../../Classes/tinystr.cpp \
                   ../../Classes/tinyxml.cpp \
                   ../../Classes/tinyxmlerror.cpp \
                   ../../Classes/tinyxmlparser.cpp \
                   ../../Classes/MoveToRightPlaceInterval.cpp \
                   ../../Classes/SqliteHelper.cpp \
                   ../../Classes/CharacterEntity.cpp \
                   ../../Classes/RightZiAnimationAction.cpp \
                   ../../Classes/tools/DataTool.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes\

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += luajit_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,D:\graduation_project\cocos2d-x-2.1.5)
$(call import-add-path,D:\graduation_project\cocos2d-x-2.1.5\cocos2dx\platform\third_party\android\prebuilt\)
$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,scripting/lua/proj.android)