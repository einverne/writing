--[[��ԭ��api�ĸĽ�.ʹ������Ե���stroke����unit�����character���򣬲�������ط���   ]]--

local math = math
local string = string
local tonumber = tonumber
local tostring = tostring
local table = table


Pass2CStr = ""
----########################			��������				#############################
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



-----------------------------------------------------------------------------------------
function ToStringEx(value)
    if type(value)=='table' then
       return TableToStr(value)
    elseif type(value)=='string' then
        return "\""..value.."\""
    else
       return tostring(value)
    end
end

function TableToStr(t)
    if t == nil then
		return " "
	end

    local retstr= "{"

    local i = 1
    for key,value in pairs(t) do
        local signal = ","
        if i==1 then
          signal = ""
        end

        if key == i then
            retstr = retstr..signal..ToStringEx(value)
        else
            if type(key)=='number' or type(key) == 'string' then
				if  ToStringEx(key)=="\"".."error".."\""  then
					retstr = retstr..signal..ToStringEx(key)..":"..ToStringEx2(value)
				else
					retstr = retstr..signal..ToStringEx(key)..":"..ToStringEx(value)
				end
            else
                if type(key)=='userdata' then
                    retstr = retstr..signal.."*s"..TableToStr(getmetatable(key)).."*e"..":"..ToStringEx(value)
                else
                    retstr = retstr..signal..key..":"..ToStringEx(value)
                end
            end
        end

        i = i+1
    end

     retstr = retstr.."}"
     return retstr
end


function ToStringEx2(value)
    if type(value)=='table' then
       return TableToStr2(value)
    elseif type(value)=='string' then
        return "\""..value.."\""
    else
       return tostring(value)
    end
end

function TableToStr2(t)
    if t == nil then
		return " "
	end

    local retstr= "["

    local i = 1
    for key,value in pairs(t) do
        local signal = ","
        if i==1 then
          signal = ""
        end

        if key == i then
            retstr = retstr..signal..ToStringEx(value)
        else
            if type(key)=='number' or type(key) == 'string' then
					retstr = retstr..signal..ToStringEx(key)..":"..ToStringEx(value)
            else
                if type(key)=='userdata' then
                    retstr = retstr..signal.."*s"..TableToStr(getmetatable(key)).."*e"..":"..ToStringEx(value)
                else
                    retstr = retstr..signal..key..":"..ToStringEx(value)
                end
            end
        end

        i = i+1
    end

     retstr = retstr.."]"
     return retstr
end
--#########################			��������			#######################################
local strokeLevel = nil
local writeHZ = nil
local PointTableStrings={}
local strUnitRule = nil
local strCharacterRule = nil
local RunAPI = {}


-------------------------------------------------------------------------------------------------
--######################### PassParametersToAPI ########################################
function RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	--��ʼ����д��
	local WZ = require("WriteZiInfo")
	writeHZ = WZ.WriteHZ:new()
	writeHZ:initialize(WriteZi)
	local bhNum = writeHZ.strokeNum

	--��������д�㼯�水�ʻ����ڱ���
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
	--����
	local ZiRuleList = self:parseUnitRule(UnitRule)
	--����
	local CharacterRule = self:parseZiRule(CharacterRule)
	--��������������װ
	local NewZiRuleArr = self:contractRule(ZiRuleList,CharacterRule)

	baseFuncs.setbhNum(bhNum)
	print"============"
	print (bhNum)
	baseFuncs.infostr= {}
	local result3=self:RunZiRule(bhNum,NewZiRuleArr)

	return result3
end

--#########################	�Բ���������������ӵ�ZiRuleList�� ########################################

function RunAPI:parseUnitRule(strUnitRule)
	local ZiRuleList = {}
	local allZiRuleList={}
	strUnitRule  = trim(strUnitRule)
	ZiRuleListTable = superSplit(strUnitRule ,"//##begin")
	table.remove(ZiRuleListTable,1)

	for i = 1,#ZiRuleListTable do
			local oneUnitRule = ZiRuleListTable[i]
			--oneUnitRule  = string.gsub(oneUnitRule , "//##begin", "" )
			oneUnitRule  = string.gsub(oneUnitRule , "//##end", "" )
			oneUnitRule  = string.gsub(oneUnitRule , "//##", "//##--" )
			oneUnitRule  = string.gsub(oneUnitRule , "//&&", "--//&&--")
			oneUnitRule  = trim(oneUnitRule)
			local ZiRuleList = {}
			--�и�������еĹ���
			local tmpZiRuleList = {}
			tmpZiRuleList = superSplit(oneUnitRule ,"//##")
			table.remove(tmpZiRuleList,1)
			for i = 1,#tmpZiRuleList do
				local oneNode = {}
				oneNode.index = i-1
				oneNode.codes = tmpZiRuleList[i]
				ZiRuleList[#ZiRuleList+1] = oneNode
			end
			allZiRuleList[#allZiRuleList+1]=ZiRuleList
		end
	return allZiRuleList
end

--#########################	�Բ���������������ӵ�ZiRuleList�� ########################################

--#########################	�����ֹ�����������ӵ�CharacterRule�� ########################################

--�����ֹ���浽CharacterRule��
function RunAPI:parseZiRule(strCharacterRule)

	local CharacterRule =  {}
	--�����ֹ�������������
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
--#########################	�����ֹ�����������ӵ�CharacterRule�� ########################################


--#########################	�����ֹ���Ͳ����������ϵ�һ�� ########################################

function RunAPI:contractRule(ZiRuleList,CharacterRule)
--�������ʻ����ӱʻ���Ŀ�ж����
	local NewZiRuleArr = {}
	local str1 = "if(bhNum == "
	local str2 = ") then ".."\n"
	local str3 = "end".."\n"
	local newRule = ""
	local last = 0
	local temp = 0

	for i = 1,#ZiRuleList do
		local tempZirule = ZiRuleList[i]
		print"kkkkkkkkkkkkkkkkkkkkkk"
		print(#tempZirule)
		for k = 1,#tempZirule do
			newRule = str1.. tostring (k) ..str2
			if(i > 1) then
				newRule = str1.. tostring (last + k) ..str2
			end
			local newBH = ""
			--�õ���ǰ�ʻ�֮ǰ�����бʻ��㼯
			--if ( k > 0 ) then
			--local tempBH= ""
			--	for j = k-1,0,-1 do
			--		if(i > 1) then
			--			tempBH = "local bh"..tostring (j) .. " = GetBH(" ..tostring(last + j) .. ") "
			--		else
			--			tempBH = "local bh"..tostring (j) .. " = GetBH(" ..tostring(j) .. ") "
			--		end
			--		newBH = newBH.."\n"..tempBH
			--	end
			--end
			--------------------------------
			newRule  = newRule  .. newBH .. tempZirule[k].codes.."\n"
			local retInfo1 = "local retInfo = tostring(bflag) ..tostring(pflag) .. tostring(uflag).. tostring(cflag)".."\n"
			local endline = "GerResult(retInfo,errorBHPoint)".."\n"
			if (i == #ZiRuleList and k == #tempZirule) then--���һ�ʼ������ֲ���
				for n = 1,#CharacterRule do
					newRule =newRule ..CharacterRule[n].codes.."\n"
				end
			end
			newRule  =  newRule ..retInfo1..endline..str3
			NewZiRuleArr[#NewZiRuleArr+1] = newRule
			temp = k
		end
		last = last + temp
	end
	return NewZiRuleArr
end

--#########################	�����ֹ���Ͳ����������ϵ�һ�� ########################################


--#########################	��baseFuncs���뵽ִ�е��ַ����� ִ�й��� �õ����ת����JSON ########################################
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

	for i=1,#Pass2CStrList do
	local temp=Pass2CStrList[i]
	print"========================================"
	print (tonumber(temp))
		for j=1,#temp do
			print(temp[j])
			print"+++++++++++++++++++++++++++++++++++++"
		end
	print(#temp)
	end

	local ret = Pass2CStrList[1]
	ret = tonumber(ret)
	local bhinfo = 1

	local bhrightinfo = math.floor(ret/1000)
	local wzrightinfo = math.floor((ret-bhrightinfo*1000)/100)
	local unitrightinfo =math.floor((ret-bhrightinfo*1000-wzrightinfo*100)/10)
	local zirightinfo =math.floor(ret-bhrightinfo*1000-wzrightinfo*100-unitrightinfo*10)

	print"//////////////////////"
	print (bhrightinfo,wzrightinfo,unitrightinfo,zirightinfo,ret)
	------------------------------------------------------------------------------------

	local errorStrokePoint= Pass2CStrList[2]

	if(errorStrokePoint == nil)then
		errorStrokePoint = {}
	end

	--print(errorStrokePoint,#errortype)

	--Ϊ�˱�����Щ���棬û�бʻ���Ϣʱ�����ܷ���"errorstroke":[]
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
		temp["errortype"]="B0002"
		temp["errorstroke"]=errorBHPoint
		errorStrokePoint[#errorStrokePoint+1]=temp
	elseif(bhrightinfo == 1 and wzrightinfo == 1 )then
		ret = bhrightinfo*100 + unitrightinfo*10 + zirightinfo
		print"�ʻ���ȷ"
	end

	ret=tostring(ret)
	print (ret)


	--print (errorStrokePoint)
	--ret lua table for JSON encode
	function error_json_string(ret)
		local allerror={}

		local re_index=1
		local re_diff=0.0

		if #errorStrokePoint>1 then
			for i = 1,#errorStrokePoint do
				if(errorStrokePoint[i]["errordiff"]>re_diff) then
					re_index=i
					re_diff=errorStrokePoint[i]["errordiff"]
				end
			end
			allerror[#allerror+1] = errorStrokePoint[re_index]--ֻ����һ�ִ�������
		elseif #errorStrokePoint==1 then
			allerror[#allerror+1] = errorStrokePoint[1]--ֻ����һ�ִ�������
		end
		return { error = allerror, ["ret"] = ret }
	end


	local temptable = error_json_string(ret)

	local result2 = {}
	result2 = TableToStr(temptable)
	return result2


	--[[local myreturn="123"
	if #errorStrokePoint>=1 then
		myreturn = "ok"..errorStrokePoint[1]["errortype"]
	else
		myreturn = "abc"
	end
	return myreturn]]
end


-------------------------------------------------------------------------------------------------
local WriteZi = GetWriteInfoFromC()
local StandardZiInfo = GetStandardZiInfoFromC()
local Level = 3
local UnitRule = GetUnitRulesFromC()
local CharacterRule = GetZiRulesFromC()

local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
return result


