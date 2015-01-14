#对原有api的改进.使得其可以适用于紧评判.
local math = math
strokeRuleNode = {index = 0, codes = ""}
unitRuleNode = {index = 0, zistrokeRules = {}}

Pass2CStr = ""
--########################			辅助函数				#############################
function string:split(sep,sign)
	local sep, fields = sep or "\t", {}
	local pattern = string.format("([^"..sign.."]+)", sep)
	self:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end


function superSplit(szFullString, szSeparator)
	local nFindStartIndex = 1
	local nSplitIndex = 1
	local nSplitArray = {}
	while true do
		local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)
		if not nFindLastIndex then
		 nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))
		 break
		end
		nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)
		nFindStartIndex = nFindLastIndex + string.len(szSeparator)
		nSplitIndex = nSplitIndex + 1
	end
	return nSplitArray
end


function trim(s)
	return (string.gsub(s,"^%s*(.-)%s*$","%1"))
end

--#########################			辅助函数			#######################################



--#########################	处理手写字信息		 ########################################
--local WriteZiStr = GetWriteInfoFromC()	--外部接口1
local WriteZiStr = [[90/140/91/147/91/154/91/161/91/167/91/174/91/181/91/187/91/194/91/201/91/207/91/214/91/221/91/227/91/234/91/241/91/247/91/254/91/261/91/267/@116/155/116/152/116/149/131/143/140/143/155/140/176/140/203/140/233/140/265/140/289/140/310/140/334/140/346/140/361/140/367/140/373/140/376/140/382/140/388/137/391/137/394/137/403/137/406/137/409/137/406/137/400/140/394/140/385/143/376/143/364/146/355/155/349/158/337/158/331/164/325/167/319/167/310/170/298/176/292/179/286/182/274/191/268/194/262/194/256/194/254/200/248/200/245/200/242/200/239/200/239/203/236/203/233/203/233/206/230/206/230/206/@]]
local WZ = require("WriteZiInfo")
local writeHZ = WZ.WriteHZ:new()
writeHZ:initialize(WriteZiStr)
--#########################	处理手写字信息		 ########################################



--#########################	处理标准字信息		 ########################################
--local StandardZiInfo = GetStandardZiInfoFromC()---接口2
--local SZ = require("StandardZiInfo")
--local stdHZ = SZ.StdHZ:new()
--stdHZ:initialize(StandardZiInfo)

--#########################	处理标准字信息		 ########################################



--获得规则代码
--local strZiRule = GetRulesFromC();---接口3 注意在外面要判断是取松规则还是紧规则
local strZiRule =[[//##begin
//##:110009:----
local bh0 = GetBH(0)
if(IsShu(bh0,bl) ==false) then
trace("0")
 else
  trace("1")
 end

//##:110023:----
local bh1 = GetBH(1)
if(IsHengZhe(bh1,bl) ==false) then
bflag = 0
 else
  bflag = 1
 end

//##:110014:----
local bh2 = GetBH(2)
if(IsNa(bh2,bl) ==false) then
trace("0")
 else
  trace("1")
 end
//##end
]]
--接口4 注意根据松评判或者紧评判给出相应的strokelevel
--local strokeLevel =  GetStrokeLevelFromC()
local strokeLevel =  '2'
--装载字级别规则 	ZiRuleList = {{index = 1 , codes = "..."},{index = 2 , codes = "..."},{index = 3 , codes = "..."},...}
strZiRule  = string.gsub(strZiRule , "//##begin", "" )
strZiRule  = string.gsub(strZiRule , "//##end", "" )
strZiRule  = string.gsub(strZiRule , "//##", "//##--" )
strZiRule  = trim(strZiRule)


local ZiRuleList = {}
local tmpZiRuleList = {}
tmpZiRuleList = superSplit(strZiRule ,"//##")
table.remove(tmpZiRuleList,1)
for i = 1,#tmpZiRuleList do
	local oneNode = {}
	oneNode.index = i-1
	oneNode.codes = tmpZiRuleList[i]
	ZiRuleList[#ZiRuleList+1] = oneNode
end


--给各个笔画增加笔画数目判断语句
local NewZiRuleArr = {}
local str1 = "if(bhNum == "
local str2 = ") then ".."\n"
local str3 = "end".."\n"
for i = 1,#ZiRuleList do
	local newRule = str1.. tostring (i) ..str2
	local newBH = ""
	if ( i > 1 ) then
		newBH = "local bh"..tostring (i - 2)
	end
	newRule  = newRule  .. newBH .. ZiRuleList[i].codes.."\n"
	if (strokeLevel == '2') then
		local retInfo = "local retInfo = tostring(bflag) .. tostring(pflag)".."\n".."trace(retInfo)".."\n"
		newRule  =  newRule ..retInfo..str3
	else
		newRule =  newRule..str3
	end
	NewZiRuleArr[#NewZiRuleArr+1] = newRule
end


baseFuncs = require("BaseLib")
baseFuncs.setWriteZiInfo(writeHZ)
baseFuncs.setWZEnv(WZ)
--设置标准字结构体
baseFuncs.setStdHZ(stdHZ)



function RunZiRule(bhNum)
	local header = [[setmetatable(baseFuncs,{__index= _G})
	setfenv(1,baseFuncs)]] .."\n"
	local pre = header .."\n" .."local bhNum ="..tostring (bhNum) .."\n".."local bl = "..tostring(strokeLevel).."\n".."local bflag = 1".."\n".."local pflag = 1".."\n"
	local allzirule = table.concat(NewZiRuleArr)
	local result = pre.."\n"..allzirule
	f = loadstring(result)
	f()
end

local bhNum = writeHZ.strokeNum
RunZiRule(bhNum)
Pass2CStr = baseFuncs.allInfoStr
print(Pass2CStr)
return Pass2CStr


