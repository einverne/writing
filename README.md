writing Chinese

###已实现
 - 界面Layer和Scene
     - Splash 开场界面
     - WallScene 汉字砖墙界面
     - LianxiScene 练习界面
         - BackgroundLayer 背景层
         - TouchLayer 触摸响应层
         - TcharacterLayer 正字信息显示层
         - HcharacterLayer 手写汉字显示层
 - 弹出窗口类PopLayer,用户创建弹出窗口
    - static PopLayer\* create(const string hanzi,const char\* backgroundImage);
    - bool addButton(const char\* normalImage,const char* seletedImage ,const char\* title, int tag=0);
    - void setTitle(const char\* title, int fontsize = 20);
    - void setCallBackFunc(CCObject\* targer, SEL_CallFuncN callfun);
 - 和Lua交互实现，CLuaScriptReader类实现
     - 从Lua文件获取String 
     - 从Lua文件获取Table 
     - 调用Lua全局函数 
     - 传参数给Lua函数 
     - Lua函数调用static C++函数
     - **运行Lua代码并获取运行结果**
 - 字符编码转换，事实上后期因为所有字符编码都按照UTF-8保存和读取，所以不存在字符编码转换的问题，并且Android并不支持之前所写的UTF8TOGBK类，所以放弃使用。
 - 和Sqlite数据库交互类SqliteHelper
    - 用来创建一个db数据库 db为数据库的名字
	    - static void initDB(const char* db);
	- 用来判断表格是否存在
    	- static bool tableIsExist(string name);
	- 用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
	    - static void createTable(string sql,string name);
	- 用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
	    - static void deleteTable(string sql,string name);
	- 用来向表中插入一条数据
	    - static void insertData(string sql);
	- 用来向表中删除一条数据
	    - static void deleteData(string sql);
	- 用来向表中修改一条数据
	    - static void updateData(string sql);
	- 获取一个记录的条数
	    - static int getDataCount(string sql);
	- 读取一条记录的信息
	    - static void getDataInfo(string sql,CCObject* pSend);
	- 关闭打开的数据库
	    - static void closeDB();
 - 读取正字XML信息并解析保存到Character中 ReadXML类，借助tinyxml库实现
    - CReadXML(string xmlpath);       //传入xml路径解析
    - CReadXML(const char* xmlcontent);         //传入xml字符串并解析

###设计实现修改的地方

####界面设计和事件响应
在练习界面将布局和事件响应分开，分开四个Layer处理
似乎应该将田字格单独包装成为CCNode自定义类，单独在此类中处理田字格正字及手写字处理逻辑
同理手写字动画效果也应该在CCNode自定义类中实现。
