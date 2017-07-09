//  HclcData.cpp
//  CppLua
//  Created by Himi on 13-4-17.

#include "HclcData.h"
#include "CCLuaEngine.h"

bool LuaData::_isFirst;
LuaData* LuaData::_shared;

LuaData* LuaData::sharedHD(){
    if(!_isFirst){
	_shared = new LuaData();
    }
    return _shared;
}

const char* LuaData::getLuaVarString(const char* luaFileName,const char* varName){

    lua_State* ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();

    std::string filefullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFileName);
    const char* pfilefullpath = filefullpath.c_str();
    int isOpen = luaL_dofile(ls, pfilefullpath);
    if(isOpen!=0){
	CCLOG("Open Lua Error: %i", isOpen);
	CCLOG("Error: %s", lua_tostring(ls,-1));
	return NULL;
    }

    lua_settop(ls, 0);
    lua_getglobal(ls, varName);

    //Returns 1 if the value at the given index is a string or a number (which is always convertible to a string), and 0 otherwise.
    int statesCode = lua_isstring(ls, 1);
    if(statesCode!=1){
	CCLOG("Open Lua Error: %i", statesCode);
        return NULL;
    }

    const char* str = lua_tostring(ls, 1);
    lua_pop(ls, 1);

    return str;
}

const char* LuaData::getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName){
 
    lua_State* ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();

    std::string filefullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFileName);
    const char* pfilefullpath = filefullpath.c_str();
    int isOpen = luaL_dofile(ls, pfilefullpath);
    if(isOpen!=0){
        CCLOG("Open Lua Error: %i", isOpen);
        return NULL;
    }
 
    lua_getglobal(ls, varName);
 
    int statesCode = lua_istable(ls, -1);
    if(statesCode!=1){
        CCLOG("Open Lua Error: %i", statesCode);
        return NULL;
    }
 
    lua_pushstring(ls, keyName);
    lua_gettable(ls, -2);
    const char* valueString = lua_tostring(ls, -1);

    lua_pop(ls, -1);
 
    return valueString;
}


string LuaData::getLuaVarTable(const char* luaFileName,const char* varName){
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();

    std::string filefullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFileName);
    const char* pfilefullpath = filefullpath.c_str();
    int isOpen = luaL_dofile(ls, pfilefullpath);
    if(isOpen!=0){
        CCLOG("Open Lua Error: %i", isOpen);
        return NULL;
    }
 
    lua_getglobal(ls, varName);
 
    int it = lua_gettop(ls);
    lua_pushnil(ls);
 
    string result="";
 
    while(lua_next(ls, it))
    {
        std::string key = lua_tostring(ls, -2);
        std::string value = lua_tostring(ls, -1);
 
        result=result+key+":"+value+"\t";
 
        lua_pop(ls, 1);
    }
    lua_pop(ls, 1);
 
    return result;
}


const char* LuaData::callLuaFunction(const char* luaFileName,const char* functionName){
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
 
    std::string filefullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFileName);
    const char* pfilefullpath = filefullpath.c_str();
    int isOpen = luaL_dofile(ls, pfilefullpath);
    if(isOpen!=0){
        CCLOG("Open Lua Error: %i", isOpen);
        return NULL;
    }
 
	//luaopen_base(ls);	//print
    lua_getglobal(ls, functionName);
 
    lua_pushstring(ls, "einverne");
    lua_pushnumber(ls, 23);
    lua_pushboolean(ls, true);

//     lua_call(ls, 3, 1);
	/*
     lua_call
     ��һ������:�����Ĳ�������
     �ڶ�������:��������ֵ����
     */
	lua_pcall(ls, 3, 1,0);
    const char* iResult = lua_tostring(ls, -1);

    return iResult;
}


void  LuaData::callCppFunction(const char* luaFileName){
 
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
 
    /*
     Lua���õ�C++�ĺ��������Ǿ�̬��
     */
    lua_register(ls, "cppFunction", cppFunction);
 
    std::string filefullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFileName);
    const char* pfilefullpath = filefullpath.c_str();
    int isOpen = luaL_dofile(ls, pfilefullpath);
    if(isOpen!=0){
        CCLOG("Open Lua Error: %i", isOpen);
        return;
    }
}
 
int LuaData::cppFunction(lua_State* ls){
    int luaNum = (int)lua_tonumber(ls, 1);
    int luaStr = (int)lua_tostring(ls, 2);
    //CCLOG("Lua����cpp����ʱ���������������� %i  %s",luaNum,luaStr);
 
    /*
     ����Lua��ֵ
     */
    lua_pushnumber(ls, 321);
    lua_pushstring(ls, "Himi");
 
    /*
     ����Luaֵ����
     */
    return 2;
}
 
const char* LuaData::getFileFullPath(const char* fileName){
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
}

LuaData::~LuaData(){
 
    CC_SAFE_DELETE(_shared);
    _shared=NULL;
}