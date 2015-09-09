易汉字App

练习汉字应用，分单元形式自由练习汉字或者评分书写练习汉字。评分通过接口调用 Lua 脚本实现。评价方式有难易区分，难模式在书写评判时要求更加严格。应用自带4个单元典型汉字，后续汉字规则待扩充。


汉字练习及评价应用
=======

##项目简介

本项目使用 `Cocos2d-x-2.2.6` 开发，目前已经完成 Win32，及Android部分的开发，正迁移至iOS。本应用以辅助练习汉字，及给汉字评价为重点。

##项目环境

Cocos2d-x-2.2.6 [link](http://cocos2d-x.org/download)  
建议使用 Python 2.7.x 新建项目

win32开发：

1. Visual Studio 2012 with help of visual assist

Android开发：

1. Eclipse with ADT & CDT
2. JDK
3. Android [SDK](http://developer.android.com/sdk/index.html)
4. Android [NDK](http://developer.android.com/tools/sdk/ndk/index.html) r9

iOS开发(待更新)

1. xcode

##项目配置

###win32项目

项目中可能遇到找不到Resources目录，也就是寻找不到资源的错误，此时在项目属性中做如下修改：

> 项目属性 - 配置属性 - 调试 - 工作目录 $(ProjectDir)..\Resources

###Android

安装Android开发环境，配置NDK环境变量（设置NDK\_ROOT的环境变量,指向文件目录中Android NDK解压的目录类似“D:\\Android\\android-ndk-r9\\”）。

导入 `proj.android` 项目到 Eclipse 中，导入之后可能会产生很多的错误。此时注意配置。

> 选中Project,右击"Properties",在左边栏中找到Java Build Path,在Source Tab中找到Link Source Browse找到 **$COCOS2DX-HOME/cocos2dx/platform/android/java/src/**

> 项目属性->C/C++ Build->"Builder Settings"->"Build command"->改为"${NDKROOT}\ndk-build.cmd"

> 项目属性->C/C++ Build->"Environment"->添加名为"NDK\_MODULE\_PATH",值为"~\\cocos2d-x-2.1.5;~\\cocos2d-x-2.1.5\\cocos2dx\\platform\\thirdparty\\android\\prebuilt\\"的变量,根据自己的实际所在地址填写.

> 将Resources中资源文件拷贝到assets文件夹中,并且适当修改Android.mk文件

