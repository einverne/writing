--#对原有api的改进.使得其可以适用于紧评判.
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
local WriteZiStr = GetWriteInfoFromC()	--外部接口1
local WZ = require("WriteZiInfo")
local writeHZ = WZ.WriteHZ:new()
writeHZ:initialize(WriteZiStr)
--#########################	处理手写字信息		 ########################################



--#########################	处理标准字信息		 ########################################
local StandardZiInfo = GetStandardZiInfoFromC()---接口2
local SZ = require("StandardZiInfo")
local stdHZ = SZ.StdHZ:new()
stdHZ:initialize(StandardZiInfo)

--#########################	处理标准字信息		 ########################################



--获得规则代码
local strZiRule = GetRulesFromC();---接口3 注意在外面要判断是取松规则还是紧规则
--接口4 注意根据松评判或者紧评判给出相应的strokelevel
local strokeLevel =  GetStrokeLevelFromC()


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
		newBH = "local bh"..tostring (i - 2) .. " = GetPreBH(" ..tostring(i - 2) .. ") "
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
ret = RunZiRule(bhNum)
Pass2CStr = baseFuncs.allInfoStr

--print(Pass2CStr)
return Pass2CStr

