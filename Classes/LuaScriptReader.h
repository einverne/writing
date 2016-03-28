#ifndef __LuaScriptReader_H__
#define __LuaScriptReader_H__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LUASCRIPTREADER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LUASCRIPTREADER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#include <string>
using namespace std;

extern "C"{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#define MAX_LUA_FILE_LEN 1024*200

class CLuaScriptReader{
public:
	CLuaScriptReader();
	~CLuaScriptReader();

	/**
	* 初始化lua及lua的底层库
	* @return
	*/
	bool InitLuaScriptReader();

	/**
	* 释放lua
	* @return
	*/
	void ExitLuaScriptReader();

	/**
	* 运行buff中的lua代码,不带返回值 luaL_loadbuffer
	* @param buff
	* @param name
	* @return
	*/
	bool RunScriptBuffer(const char *buff,const char *name);

	/**
	* 运行buff中lua代码，带返回值
	* @param buff
	* @param ret_string
	* @param name
	* @return
	*/
	bool RunScriptBuffer(const char *buff,char* ret_string,const char* name);

	/**
	* 运行buff中lua文件
	* @param filename
	* @param name
	* @return
	*/
	bool RunScriptFile(const char *filename,const char *name);

	/**
	* 运行buff中lua文件,带返回值
	* @param filename
	* @param ret_string
	* @param name
	* @return
	*/
	bool RunScriptFile(const char *filename,char* ret_string,const char *name);

	bool GetSourceCode(char *sourcecode);

	/**
	* pass hanzi string to Lua
	* @param hanzi
	* @return
	*/
	void setZiName(string hanzi);

	/**
	* pass level string to lua
	* @param level
	* @return
	*/void setLevel(string level);

	/**
	* copy wz to WriteZiInfo
	* @param wz
	* @return
	*/
	bool setWriteZiInfo(const char *wz);

	/**
	*
	* @param stdinfo
	* @return
	*/
	bool setStandardZiInfo(string stdinfo);
//	bool GetRunType(int level, int step);
//	bool GetUnitIndex(int idx);
//	bool GetStrokeIndex(int idx);

	/**
	*
	* @param globalfunc
	* @return
	*/
// 	bool setGlobalFunc(char *globalfunc);

	/**
	* read file and copy string to GlobalFunc
	* @param filename
	* @return
	*/
//	void SetGlobalFunc(const char * filename);

	
	/**
	* 
	* @param funcs
	* @return
	*/void setGlobalFunc(string funcs);
	
	/**
	* read file and copy string to Rules
	* @param filename
	* @return
	*/
//	void SetRulesFunc(const char* filename);
	
	/**
	 * pass lua to lus judge engine
	 * @param rules
	 */
	void setRulesFunc(string rules);

	/**
	* pass unit lua to lua engine
	* @param urules
	* @return
	*/
	void setUnitRule(string urules);

	/**
	* zi rule to lua engine
	* @param zirule
	* @return
	*/
	void setZiRule(string zirule);

	/**
	* 
	* @param filename
	* @param name
	* @return
	*/
	bool RunMixedFile(const char *filename,const char *name);
    
    
    int setLuaPath(const char* path);

protected:
    lua_State *m_plua;
    string sourcecode;
    
};


#endif
