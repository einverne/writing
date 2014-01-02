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

--读文件操作
function ReadFile(filename)
file = io.open(filename,"r")
local totalStr = ""
for line in file:lines() do
	totalStr = totalStr .. "\n"..line
end
file:close()
return totalStr
end
--#########################			辅助函数			#######################################


--#########################	处理手写字信息		 ########################################
local WriteZiStr = GetWriteInfoFromC()	--外部接口
--local WriteZiStr = [[28/61/28/61/29/61/30/61/32/61/34/61/36/60/41/60/46/60/52/60/58/60/64/60/70/60/74/60/77/60/79/59/80/59/80/58/80/58/@57/31/57/32/57/36/57/40/56/44/55/52/55/58/55/65/55/70/55/74/55/78/55/80/56/82/57/83/57/84/57/85/57/86/57/86/57/88/57/89/57/90/57/90/@]]
local WZ = require("WriteZiInfo")
local writeHZ = WZ.WriteHZ:new()
writeHZ:initialize(WriteZiStr)
--#########################	处理手写字信息		 ########################################


--只需要字级别的代码即可
--#########################	装载代码		 ########################################
--local ZiName = GetZiNameFromC()				--外部接口
--local filepath = "ZiList\\"..ZiName.."\\rules.txt"
--local RuleStr = ReadFile(filepath)
--local strZiRule = RuleStr
local strZiRule = GetRulesFromC();

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
		newBH = "bh"..tostring (i - 2) .. " = GetBH(" ..tostring(i - 2) .. ");"
	end
	newRule  = newRule  .. newBH .. ZiRuleList[i].codes.."\n"..str3
	NewZiRuleArr[#NewZiRuleArr+1] = newRule
end


baseFuncs = require("BaseLib")
baseFuncs.setWriteZiInfo(writeHZ)
baseFuncs.setWZEnv(WZ)

function RunZiRule(bhNum)
	local header = [[setmetatable(baseFuncs,{__index= _G})
	setfenv(1,baseFuncs)]] .."\n"
	local pre = header .."\n" .."local bhNum ="..tostring (bhNum) .."\n"
	local allzirule = table.concat(NewZiRuleArr)
	local result = pre.."\n"..allzirule
	f = loadstring(result)
	f()
end

local bhNum = writeHZ.strokeNum
RunZiRule(bhNum)
Pass2CStr = baseFuncs.allInfoStr
return Pass2CStr


