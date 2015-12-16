--对原有api的改进.使得其可以可以调用stroke规则、unit规则和character规则，并进行相关返回.
local math = math
local string = string
local tonumber = tonumber
local tostring = tostring
local table = table
local JSON = require("JSON")

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

function trim2(s)
	return (string.gsub(s,"@",""))
end

--#########################			辅助函数			#######################################

--######################### PassParametersToAPI ########################################
local strokeLevel = nil
local writeHZ = nil
local PointTableStrings={}
local strUnitRule = nil
local strCharacterRule = nil
local RunAPI = {}

function RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	--初始化手写字
	local WZ = require("WriteZiInfo")
	writeHZ = WZ.WriteHZ:new()
	writeHZ:initialize(WriteZi)

	local bhNum = writeHZ.strokeNum

	--将所有手写点集存按笔画存在表中
	PointTableStrings={}
	for i=1,bhNum do
	PointTableStrings[#PointTableStrings+1]=writeHZ.strokeStrings[i]
	end

	local str = trim2(WriteZi)
	baseFuncs = require("BaseLib")
	baseFuncs.setWriteZiInfo(writeHZ)
	baseFuncs.setWZEnv(WZ)
	baseFuncs.GetPoints(str)
	baseFuncs.initStrokeStrs(PointTableStrings)

	strokeLevel = Level
	--部件
	local ZiRuleList = self:parseUnitRule(UnitRule)
	--整字
	local CharacterRule = self:parseZiRule(CharacterRule)
	--将部件和整字组装
	local NewZiRuleArr = self:contractRule(ZiRuleList,CharacterRule)

	baseFuncs.setbhNum(bhNum)
	print"============"
	print (bhNum)
	baseFuncs.infostr= {}
	local result3=self:RunZiRule(bhNum,NewZiRuleArr)

	return result3
end



--######################### PassParametersToAPI ########################################


--#########################	对部件规则进行整理，加到ZiRuleList表 ########################################

function RunAPI:parseUnitRule(strUnitRule)
	local ZiRuleList = {}
	if(strokeLevel == 3 )then
		strUnitRule  = string.gsub(strUnitRule , "//##begin", "" )
		strUnitRule  = string.gsub(strUnitRule , "//##end", "" )
		strUnitRule  = string.gsub(strUnitRule , "//##", "//##--" )
		strUnitRule  = string.gsub(strUnitRule , "//&&", "--//&&--")
		strUnitRule  = trim(strUnitRule)

		--切割出部件中的规则
		local tmpZiRuleList = {}
		tmpZiRuleList = superSplit(strUnitRule ,"//##")
		table.remove(tmpZiRuleList,1)
		for i = 1,#tmpZiRuleList do
			local oneNode = {}
			oneNode.index = i-1
			oneNode.codes = tmpZiRuleList[i]
			ZiRuleList[#ZiRuleList+1] = oneNode
		end
	end
	return ZiRuleList
end

--#########################	对部件规则进行整理，加到ZiRuleList表 ########################################

--#########################	对整字规则进行整理，加到CharacterRule表 ########################################

--将整字规则存到CharacterRule里
function RunAPI:parseZiRule(strCharacterRule)

	local CharacterRule =  {}
	--对整字规则做基本处理
	strCharacterRule  = string.gsub(strCharacterRule , "//##begin", "" )
	strCharacterRule  = string.gsub(strCharacterRule , "//##end", "" )
	strCharacterRule  = string.gsub(strCharacterRule , "//^^", "//^^--")
	strCharacterRule  = trim(strCharacterRule)

	local tmpCharacterRuleList = {}
	tmpCharacterRuleList = superSplit(strCharacterRule ,"//^^")
	table.remove(tmpCharacterRuleList,1)

	for i = 1,#tmpCharacterRuleList do
		local oneNode = {}
		oneNode.index = i-1
		oneNode.codes = tmpCharacterRuleList[i]
		CharacterRule[#CharacterRule+1] = oneNode
	end
	return CharacterRule
end
--#########################	对整字规则进行整理，加到CharacterRule表 ########################################


--#########################	将整字规则和部件规则整合到一起 ########################################

function RunAPI:contractRule(ZiRuleList,CharacterRule)
--给各个笔画增加笔画数目判断语句
	local NewZiRuleArr = {}
	local str1 = "if(bhNum == "
	local str2 = ") then ".."\n"
	local str3 = "end".."\n"
	for i = 1,#ZiRuleList do
		local newRule = str1.. tostring (i) ..str2
		local newBH = ""

		if ( i > 1 ) then
			for j = i-2,0,-1 do
				local tempBH = "local bh"..tostring (j) .. " = GetPreBH(" ..tostring(j) .. ") "
				newBH = newBH.."\n"..tempBH
			end
		end
		newRule  = newRule  .. newBH .. ZiRuleList[i].codes.."\n"
		if (strokeLevel == 2 ) then
			local retInfo = "local retInfo = tostring(bflag) .. tostring(pflag)".."\n".."trace(retInfo)".."\n"
			newRule  =  newRule ..retInfo..str3
		elseif (strokeLevel == 3) then--检查为宽松规则，但是有bflag和pflag的信息--.. tostring(InfleRatio1)..tostring(InfleRatio2)

		local retInfo1 = "local retInfo = tostring(bflag) ..tostring(pflag) .. tostring(uflag).. tostring(cflag)".."\n"
		local endline = "GerResult(retInfo,errorBHPoint)".."\n"

			if (i == #ZiRuleList) then--最后一笔加载整字侧面
				for j = 1,#CharacterRule do
				newRule =newRule ..CharacterRule[j].codes.."\n"
				end
			end

			newRule  =  newRule ..retInfo1..endline..str3
		else
			newRule  = newRule ..str3
		end
		NewZiRuleArr[#NewZiRuleArr+1] = newRule
	end
	return NewZiRuleArr
end

--#########################	将整字规则和部件规则整合到一起 ########################################

--#########################	将baseFuncs加入到执行的字符串中 执行规则 得到结果转换成JSON ########################################
function RunAPI:RunZiRule(bhNum,NewZiRuleArr)
	local header = [[setmetatable(baseFuncs,{__index= _G})
	setfenv(1,baseFuncs)]] .."\n"
	local pre = header .."\n" .."local bhNum ="..tostring (bhNum) .."\n".."local bl = "..tostring(strokeLevel).."\n".."local bflag = 1".."\n".."local pflag = 1".."\n".."local uflag = 1".."\n".."local cflag = 1".."\n".."local bhflag = 1".."\n".."local errorBHPoint={}".."\n"
	local allzirule = table.concat(NewZiRuleArr)
	local result = pre.."\n"..allzirule
	print(result)
	f = loadstring(result)
	f()

	baseFuncs=require("BaseLib")
	local Pass2CStrList={}
	Pass2CStrList = baseFuncs.resultTable

	--[[for i=1,#Pass2CStrList do
	local temp=Pass2CStrList[i]
	print"========================================"
		for j=1,#temp do
			print(temp[j])
			print"+++++++++++++++++++++++++++++++++++++"
		end
	print(#temp)
	end--]]

	local ret = Pass2CStrList[1]
	ret = tonumber(ret)
	local bhinfo = 1

	local bhrightinfo = math.floor(ret/1000)
	local wzrightinfo = math.floor((ret-bhrightinfo*1000)/100)
	local unitrightinfo =math.floor((ret-bhrightinfo*1000-wzrightinfo*100)/10)
	local zirightinfo =math.floor(ret-bhrightinfo*1000-wzrightinfo*100-unitrightinfo*10)
	print"//////////////////////"
	print (bhrightinfo,wzrightinfo,unitrightinfo,zirightinfo,ret)
	local curerrorStrokePoint  = {}
	--local errortype = Pass2CStrList[2]
	local errorStrokePoint= Pass2CStrList[2]

	if(errorStrokePoint == nil)then
		errorStrokePoint = {}
		curerrorStrokePoint={}
	else
		--curerrorStrokePoint = errorStrokePoint[1]
		--print">>>>>>>>>>>>>>>>>>>>>>>>>>"
		--for k,v in pairs(curerrorStrokePoint) do
			--print(k,v )
		--end
	end

	--print(errorStrokePoint,#errortype)

	--为了保持有些侧面，没有笔画信息时，还能返回"errorstroke":[]
	--if(errorStrokePoint == nil and #errortype ~= 0)then
	--errorStrokePoint = {}
	--elseif(errorStrokePoint == nil and #errortype == 0)then
	--errorStrokePoint = nil
	--end
	if(bhrightinfo == 0 )then
		table.remove(InflectionPoint,#InflectionPoint)
		table.remove(baseFuncs.typeInfo,#baseFuncs.typeInfo)
		ret = "011"
		--table.insert(errortype,1,"B0001")
		errorStrokePoint={}
		local errorBHPoint={}
		errorBHPoint["null"]="null"
		local temp={}
		temp["errortype"]="B0001"
		temp["errorstroke"]=errorBHPoint
		errorStrokePoint[#errorStrokePoint+1]=temp
	elseif(wzrightinfo == 0)then
		table.remove(InflectionPoint,#InflectionPoint)
		table.remove(baseFuncs.typeInfo,#baseFuncs.typeInfo)
		ret = "011"
		--table.insert(errortype,1,"B0002")
		errorStrokePoint={}
		local errorBHPoint={}
		errorBHPoint["null"]="null"
		local temp={}
		temp["errortype"]="B0001"
		temp["errorstroke"]=errorBHPoint
		errorStrokePoint[#errorStrokePoint+1]=temp
	elseif(bhrightinfo == 1 and wzrightinfo == 1 )then
		ret = bhrightinfo*100 + unitrightinfo*10 + zirightinfo
		print"笔画正确"
	else
		print"笔画正确"
	end

	ret=tostring(ret)
	--ret lua table for JSON encode
	function error_json_string(errors,ret)
		local allerror={}
		if(errors == nil)then
			allerror = {}
		else
			for i = 1,#errorStrokePoint do
				allerror[#allerror+1] = errorStrokePoint[i]
			end
			return { error = allerror, ["ret"] = ret }
		end
	end

	local result2 = {}
	result2 = JSON:encode(error_json_string(errorStrokePoint, ret))
	return result2
end

return RunAPI
