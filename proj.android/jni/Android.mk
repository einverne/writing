LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
    ../../Classes/CocosWidget/Button.cpp \
	../../Classes/CocosWidget/CheckBox.cpp \
	../../Classes/CocosWidget/ColorView.cpp \
	../../Classes/CocosWidget/ControlView.cpp \
	../../Classes/CocosWidget/ExpandableListView.cpp \
	../../Classes/CocosWidget/GradientView.cpp \
	../../Classes/CocosWidget/GridPageView.cpp \
	../../Classes/CocosWidget/GridView.cpp \
	../../Classes/CocosWidget/ImageView.cpp \
	../../Classes/CocosWidget/ImageViewScale9.cpp \
	../../Classes/CocosWidget/Label.cpp \
	../../Classes/CocosWidget/LabelAtlas.cpp \
	../../Classes/CocosWidget/LabelBMFont.cpp \
	../../Classes/CocosWidget/Layout.cpp \
	../../Classes/CocosWidget/ListView.cpp \
	../../Classes/CocosWidget/Lua_cocos2dx_widget.cpp \
	../../Classes/CocosWidget/PageView.cpp \
	../../Classes/CocosWidget/ProgressBar.cpp \
	../../Classes/CocosWidget/Scale9Sprite.cpp \
	../../Classes/CocosWidget/ScrollView.cpp \
	../../Classes/CocosWidget/Slider.cpp \
	../../Classes/CocosWidget/TableView.cpp \
	../../Classes/CocosWidget/TextArea.cpp \
	../../Classes/CocosWidget/TextRich.cpp \
	../../Classes/CocosWidget/ToggleView.cpp \
	../../Classes/CocosWidget/Widget.cpp \
	../../Classes/CocosWidget/WidgetProtocol.cpp \
	../../Classes/CocosWidget/WidgetWindow.cpp \
	../../Classes/CocosWidget/WitlsMacros.cpp \
	../../Classes/tools/DataTool.cpp \
   ../../Classes/sqlite3.c \
   ../../Classes/AppDelegate.cpp \
   ../../Classes/BackgroundLayer.cpp \
   ../../Classes/Bujian.cpp \
   ../../Classes/Character.cpp \
   ../../Classes/JudgeScene.cpp \
   ../../Classes/JudgeLayer.cpp \
   ../../Classes/HanziManage.cpp \
   ../../Classes/HcharacterDrawnode.cpp \
   ../../Classes/HcharacterLayer.cpp \
   ../../Classes/HclcData.cpp \
   ../../Classes/JudgeManager.cpp \
   ../../Classes/LianxiScene.cpp \
   ../../Classes/LuaScriptReader.cpp \
   ../../Classes/MainScene.cpp \
   ../../Classes/MyToast.cpp \
   ../../Classes/PopLayer.cpp \
   ../../Classes/PopCancelLayer.cpp \
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
   ../../Classes/WallSingleScene.cpp \
   ../../Classes/tinystr.cpp \
   ../../Classes/tinyxml.cpp \
   ../../Classes/tinyxmlerror.cpp \
   ../../Classes/tinyxmlparser.cpp \
   ../../Classes/SqliteHelper.cpp \
   ../../Classes/CharacterEntity.cpp \
   ../../Classes/CharacterExtend.cpp \
   ../../Classes/RightZiAnimationAction.cpp \
   ../../Classes/NewUnit.cpp \
   ../../Classes/ViewScene.cpp \
   ../../Classes/TagData.cpp \
   ../../Classes/MultiPointNode.cpp \
   ../../Classes/MoveToHorizontal.cpp \
   ../../Classes/MoveToVertical.cpp \
   ../../Classes/OnePointNode.cpp \
   ../../Classes/MoveToRightPlace.cpp \
   ../../Classes/DotLineNode.cpp \
   ../../Classes/UniformSpaceNode.cpp \
   ../../Classes/MoveToUniformSpace.cpp \
   ../../Classes/TwoLineNode.cpp \
   ../../Classes/DynamicDrawing.cpp \
   ../../Classes/BarycentreNode.cpp \
   ../../Classes/MoveBarycentre.cpp \
   ../../Classes/RectangleNode.cpp \
   ../../Classes/MoveRectangleNode.cpp \
   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,scripting/lua/proj.android)