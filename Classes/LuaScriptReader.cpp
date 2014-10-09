// LuaScriptReader.cpp : Defines the entry point for the DLL application.
//

// #include "stdafx.h"
#include "LuaScriptReader.h"
#include "cocos2d.h"
USING_NS_CC;


char StandardZiInfo[10240] = "";		//用于接收标准字信息
char WriteZiInfo[1024*100] = "";		//手写字信息
char Hanzi[50] = "";
char RuleInfo[1024*100] = "";		//规则信息
char GlobalFunc[1024*100] = "";		//全局函数信息
char Rules[1024*100] = "";
// char* StandardZiInfo = new char[1024];
// char* WriteZiInfo = new char[1024*10];
// char* Hanzi = new char[50];
// char* RuleInfo = new char[1024*10];
// char* GlobalFunc = new char[1024*10];
// char* Rules = new char[1024*10];
// char* StandardZiInfo = "";
// char* WriteZiInfo = "";
// char* Hanzi = "";
// char* RuleInfo="";
// char* GlobalFunc="";
// char* Rules="";
int RunLevel;						//1:字 2：部件 3：笔画
int StepFlag;						//1:单步执行 0:整字执行
int UnitIndex;						//当前部件的索引
int StrokeIndex;					//当前笔画的索引

// This is the constructor of a class that has been exported.
// see LuaScriptReader.h for the class definition
CLuaScriptReader::CLuaScriptReader()
{
	StandardZiInfo[0] = '\0';		//用于接收标准字信息
	WriteZiInfo[0] = '\0';		//手写字信息
	Hanzi[0] = '\0';
	RuleInfo[0] = '\0';		//规则信息
	GlobalFunc[0] = '\0';		//全局函数信息
	Rules[0] = '\0';

	m_plua = NULL;
	return; 
}

CLuaScriptReader::~CLuaScriptReader(){
// 	delete [] StandardZiInfo;
// 	delete [] WriteZiInfo;
// 	delete [] Hanzi;
// 	delete [] RuleInfo;
// 	delete [] GlobalFunc;
// 	delete [] Rules;
}

int GetWriteInfoFromC(lua_State *plua){
	lua_pushstring(plua, WriteZiInfo);
	return 1;    
}

int GetStandardZiInfoFromC(lua_State *plua){
	lua_pushstring(plua, StandardZiInfo);
	return 1;    
}

int GetRuleInfoFromC(lua_State *plua){
	lua_pushstring(plua, RuleInfo);
	return 1;    
}

int GetRunTypeFromC(lua_State *plua){
	lua_pushinteger(plua,StepFlag);
	return 1;
}

int GetRunLevelFromC(lua_State *plua){
	lua_pushinteger(plua,RunLevel);
	return 1;
}

int GetUnitIndexFromC(lua_State*plua){
	lua_pushinteger(plua,UnitIndex);
	return 1;
}


int GetStrokeIndexFromC(lua_State*plua){
	lua_pushinteger(plua,StrokeIndex);
	return 1;
}

int GetGlobalFuncFromC(lua_State *plua){
	lua_pushstring(plua, GlobalFunc);
	return 1;    

}

int GetZiNameFromC(lua_State *plua){
	lua_pushstring(plua, Hanzi);
	return 1;
}
int GetRulesFromC(lua_State *plua){
	lua_pushstring(plua, Rules);
	return 1;
}

bool CLuaScriptReader::InitLuaScriptReader(){
	if( m_plua == NULL ){
		m_plua = lua_open();
		if( m_plua == NULL ) return false;
		luaL_openlibs(m_plua);
		lua_register(m_plua, "GetWriteInfoFromC", GetWriteInfoFromC);
		lua_register(m_plua, "GetStandardZiInfoFromC", GetStandardZiInfoFromC);
		lua_register(m_plua, "GetRuleInfoFromC", GetRuleInfoFromC);
		lua_register(m_plua, "GetRunTypeFromC", GetRunTypeFromC);
		lua_register(m_plua,"GetUnitIndexFromC",GetUnitIndexFromC);
		lua_register(m_plua,"GetRunLevelFromC",GetRunLevelFromC);
		lua_register(m_plua,"GetStrokeIndexFromC",GetStrokeIndexFromC);
		lua_register(m_plua,"GetGlobalFuncFromC",GetGlobalFuncFromC);
		lua_register(m_plua,"GetZiNameFromC",GetZiNameFromC);
		lua_register(m_plua,"GetRulesFromC",GetRulesFromC);

	}
	return true;
}


void CLuaScriptReader::ExitLuaScriptReader(){
	if( m_plua != NULL ){
		lua_close(m_plua);
		//hrf
		m_plua = NULL;
	}
}



bool CLuaScriptReader::RunScriptBuffer(const char *buff,char *name){
	int error = 0;
	char callname[256] = "";

	if( m_plua == NULL || buff == NULL ) return false;
	if( *name == 0 ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}
// 	CCLog("RunScriptBuffer buff:%s name:%s",buff,name);
	error = luaL_loadbuffer(m_plua, buff, strlen(buff),callname) || lua_pcall(m_plua, 0, 0, 0);
	if (error){
		fprintf(stderr, "%s", lua_tostring(m_plua, -1));
		CCLog("luaL_loadbuffer2 error : %s",lua_tostring(m_plua,-1));
		lua_pop(m_plua, 1);
	}else{
		CCLog("luaL_loadbuffer2 success");
	}
	return true;
}


bool CLuaScriptReader::RunScriptBuffer(const char *buff,char* ret_string,char *name){
	int error = 0;
	char callname[256] = "";
	//*ret_string = 0;
	if( m_plua == NULL || buff == NULL || ret_string == NULL ) return false;
	if( name == NULL ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}
	error = luaL_loadbuffer(m_plua, buff, strlen(buff),callname) || lua_pcall(m_plua, 0, 1, 0);

	if (error){
		fprintf(stderr, "%s", lua_tostring(m_plua, -1));
		CCLog("luaL_loadbuffer3 %s",lua_tostring(m_plua,-1));
		lua_pop(m_plua, 1);
	}else{
 		sprintf(ret_string, "%s", lua_tostring(m_plua, -1));
		CCLog("luaL_loadbuffer3 %s",lua_tostring(m_plua,-1));
	}
	return true;
}

void CLuaScriptReader::SetGlobalFunc(const char * filename){
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FILE* fpFile = fopen(filename,"r");
	if (fpFile == NULL)
	{
		printf("cannot open file");
		return;
	}
	char line[1024] = "";
	while (fgets(line, 1024,fpFile) != NULL)
	{
		strcat(GlobalFunc,line);
	}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		unsigned long int size =0;
		unsigned char* temp = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
		CCString* ccStr = CCString::createWithData(temp,size);
		strcpy(GlobalFunc,ccStr->getCString());
// 		CCLog("GlobalFunc %s",GlobalFunc);
#endif
	return;
}

void CLuaScriptReader::SetGlobalFunc(string funcs){
	strcpy(GlobalFunc,funcs.c_str());
}

void CLuaScriptReader::SetRulesFunc(const char* filename){
	CCLog("SetRulesFunc(const char* filename)");
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FILE* fpFile = fopen(filename,"r");
	if (fpFile == NULL)
	{
		printf("cannot open file");
		return;
	}
	char line[1024] = "";
	while (fgets(line, 1024,fpFile) != NULL)
	{
		strcat(Rules,line);
	}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long int size = 0;
	unsigned char* temp = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(temp,size);
	strcpy(Rules,ccStr->getCString());
	CCLog("Rules %s",Rules);
#endif
	return;
}

void CLuaScriptReader::SetRulesFunc(CCString* rules){
	CCLog("SetRulesFunc(CCString* rules)");
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	strcpy(Rules,rules->getCString());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	strcpy(Rules,rules->getCString());
	CCLog("Rules %s",Rules);
#endif
}


bool CLuaScriptReader::RunMixedFile(const char *filename,char *name)
{
	int error = 0;
	char callname[256] = "";
	if (m_plua == NULL || filename == NULL ) return false;
	if( name == NULL ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	strcat(filebuff,"\n");
	strcat(filebuff,GlobalFunc);
	RunScriptBuffer(filebuff,callname);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	char* filebuff1 = (char*)ccStr->getCString();
	strcat(filebuff1,"\n");
	strcat(filebuff1,GlobalFunc);
	RunScriptBuffer(filebuff1,callname);
#endif
	return true;
}




bool CLuaScriptReader::RunScriptFile(const char *filename,char* ret_string,char *name){
	if( m_plua == NULL || ret_string == NULL || filename == NULL ) return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	RunScriptBuffer(filebuff,ret_string,name);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long int size = 0;
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	RunScriptBuffer(ccStr->getCString(),ret_string,name);
#endif
	return true;
}



bool CLuaScriptReader::RunScriptFile(const char *filename,char *name){
	if( m_plua == NULL || filename == NULL ) return false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char filebuff[MAX_LUA_FILE_LEN] = "";
	char line[1024] = "";
	FILE* fpFile = fopen(filename,"r");
	if( fpFile == NULL ) {
		printf("can not find file %s\n",filename);
		return false;
	}
	while( fgets(line, 1024, fpFile) != NULL) {
		strcat(filebuff,line);
	}
	fclose(fpFile);
	RunScriptBuffer(filebuff,name);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long int size = 0;
	unsigned char* filebuff = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&size);
	CCString* ccStr = CCString::createWithData(filebuff,size);
	RunScriptBuffer(ccStr->getCString(),name);
#endif
	return true;
}


bool CLuaScriptReader::GetSourceCode(char *code){
	strcpy(RuleInfo,code);
	return true;
}

bool CLuaScriptReader::SetWriteZiInfo(const char* wz){
	strcpy(WriteZiInfo,wz);
	return true;
}
void CLuaScriptReader::SetZiName(string hanzi){
	strcpy(Hanzi,hanzi.c_str());
	return;
}

bool CLuaScriptReader::GetStandardZiInfo(char *stdInfo){
	strcpy(StandardZiInfo,stdInfo);
	return true;
}

bool CLuaScriptReader::GetGlobalFunc(char *globalfunc){
	strcpy(GlobalFunc,globalfunc);
	return true;
}


bool CLuaScriptReader::GetRunType(int level, int step){
	RunLevel = level;
	StepFlag = step ;
	return true;
}

bool CLuaScriptReader::GetUnitIndex(int idx){
	UnitIndex = idx;
	return true;
}

bool CLuaScriptReader::GetStrokeIndex(int idx){
	StrokeIndex = idx;
	return true;
}

bool CLuaScriptReader::Print2File(char* str, char*filename)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	string finame(filename);
	string path = CCFileUtils::sharedFileUtils()->getWritablePath()+finame;
	FILE* file = fopen(path.c_str(),"w");
	if (file != NULL)
	{
		file = fopen(path.c_str(),"wb");
		fwrite(str,strlen(str),1,file);
	}else{
// 		CCLog("CLuaScriptReader::Print2File file NULL");
	}
	fclose(file);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FILE * pFile;

	pFile = fopen (filename, "wb" );
	fwrite (str, strlen(str), 1 , pFile );
	fclose (pFile);
#endif
	return 0;
}
