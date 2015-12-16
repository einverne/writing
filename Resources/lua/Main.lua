function func1()
 local WriteZi =GetWriteInfoFromC()
	--local StandardZiInfo = GetStandardZiInfoFromC()
	local Level = 3
	local UnitRule = [[//##begin
	//##:110007:----
	local bl = 1
	local bh0 = GetBH(0)
	if(IsHeng(bh0,bl) == false) then
		bflag = 0
	end
	//##:110009:----
	local bh1 = GetBH(1)
	if(IsPie(bh1,bl) == false) then
		bflag = 0
	end
	if (BH2BHXiangJiao(bh0,bh1) == false) then
		pflag = 0
	end
	//##:110014:----
	local bh2 = GetBH(2)
	if(IsNa(bh2,bl) ==false) then
		bflag = 0
	end
	if(BH2BHXiangJie(bh1,bh2,2,0) == false) then
		pflag = 0
	end
	//&&:230082＿---
	--local inflection= ReadFromTxt()
	--二维数组，第二笔的第一个拐点，第三笔的第一个拐炿
	--
	if( tonumber(bflag) ~= 0 and tonumber(pflag) ~= 0)then
	print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
	print (InflectionPoint[2],InflectionPoint[3])
		if(IsShuiPingPingQi(bh1,InflectionPoint[2],bh2,InflectionPoint[3]) == false)then
			uflag = 0
			print"shuipingpingqi"
		end
		if(IsZhongDianQieFen(bh0,bh1) == false)then
			uflag = 0
			print"zhongdianqiefen"
		end

		if(IsGoldenSection(bh1,bh2,0) == false)then
			uflag = 0
			print"huangjinfenge"
		end
		--errorlist=ReturnErrorType()
		errorBHPoint=ReturntypeInfo()
	end
	//##end]]

	local CharacterRule = [[//##begin
	//^^大：----
	if(  tonumber(bflag) ~= 0 and tonumber(pflag) ~= 0 and tonumber(uflag) ~= 0)then
		if(IsAspectRatio() == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	return result

end

local ret=func1()
return ret
