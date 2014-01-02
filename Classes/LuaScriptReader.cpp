// LuaScriptReader.cpp : Defines the entry point for the DLL application.
//

// #include "stdafx.h"
#include "LuaScriptReader.h"
#include "cocos2d.h"
USING_NS_CC;


char StandardZiInfo[1024] = "";		//用于接收标准字信息
char WriteZiInfo[1024*10] = "";		//手写字信息
char  Hanzi[50] = "";
char RuleInfo[1024*10] = "";		//规则信息
char GlobalFunc[1024*10] = "";		//全局函数信息
char Rules[1024*10] = "";
int RunLevel;						//1:字 2：部件 3：笔画
int StepFlag;						//1:单步执行 0:整字执行
int UnitIndex;						//当前部件的索引
int StrokeIndex;					//当前笔画的索引

// This is the constructor of a class that has been exported.
// see LuaScriptReader.h for the class definition
CLuaScriptReader::CLuaScriptReader()
{ 
	m_plua = NULL;
	return; 
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



bool CLuaScriptReader::RunScriptBuffer(char *buff,char *name){
	int error = 0;
	char callname[256] = "";

	if( m_plua == NULL || buff == NULL ) return false;
	if( *name == 0 ){
		strcpy(callname,"noname");
	}else{
		strcpy(callname,name);
	}
	error = luaL_loadbuffer(m_plua, buff, strlen(buff),callname) || lua_pcall(m_plua, 0, 0, 0);
	if (error){
		fprintf(stderr, "%s", lua_tostring(m_plua, -1));
		lua_pop(m_plua, 1);
	}else{
	}
	return true;
}


bool CLuaScriptReader::RunScriptBuffer(char *buff,char* ret_string,char *name){
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
		CCLog("%s",lua_tostring(m_plua,-1));
		lua_pop(m_plua, 1);
	}else{
 		sprintf(ret_string, "%s", lua_tostring(m_plua, -1));
		CCLog("%s",lua_tostring(m_plua,-1));
	}
	return true;
}

void CLuaScriptReader::SetGlobalFunc(const char * filename){
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
	return;
}

void CLuaScriptReader::SetRulesFunc(const char* filename){
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
	return;
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

	char line[1024] = "";
	char filebuff[MAX_LUA_FILE_LEN] = "";
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
	Print2File(filebuff,"mixed.txt");
	RunScriptBuffer(filebuff,callname);
	return true;
}




bool CLuaScriptReader::RunScriptFile(const char *filename,char* ret_string,char *name){
	if( m_plua == NULL || ret_string == NULL || filename == NULL ) return false;
	char line[1024] = "";
	char filebuff[MAX_LUA_FILE_LEN] = "";
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

	return true;
}



bool CLuaScriptReader::RunScriptFile(const char *filename,char *name){
	if( m_plua == NULL || filename == NULL ) return false;
	char line[1024] = "";	
	char filebuff[MAX_LUA_FILE_LEN] = "";
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

	return true;
}


bool CLuaScriptReader::GetSourceCode(char *code){
	strcpy(RuleInfo,code);
	return true;
}

bool CLuaScriptReader::GetWriteZiInfo(char* wz){
	strcpy(WriteZiInfo,wz);
	return true;
}
void CLuaScriptReader::GetZiName(string hanzi){
	strcpy(Hanzi,hanzi.c_str());
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
	FILE * pFile;

	pFile = fopen (filename, "wb" );
	fwrite (str, strlen(str), 1 , pFile );
	fclose (pFile);
	return 0;

}