Cocos2d-x中实现的很实用的类

###MyToast Cocos2d-x中模拟Android Toast效果
代码在MyToast.h MyToast.cpp中。使用方法：
第一种方法，使用静态方法

    /**
    * static function to show Toast
    * @param node which node to call this function
    * @param msg	the message to show
    * @param dt		the time toast will show, second    can use TOAST_LONG and TOAST_SHORT or any other value you want
    * @return
    */
    static void showToast(CCNode* node,string msg,float dt);

调用：`MyToast::showToast(this,"This is a Test",TOAST_LONG);`

第二种方法，使用MyToast initToast函数

    MyToast* toast = MyToast::create();
    toast->initToast("This is a Test",TOAST_LONG);
    this->addChild(toast,2);

实现：http://blog.csdn.net/nat_myron/article/details/17279571

###Cocos2d-x中SQLite数据库使用



###Cocos2d-x中Lua调用


###Cocos2d-x中中文字符串显示