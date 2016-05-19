----------下的部件规则
//##begin
//##:110007:----
local bh0 = GetBH(0)
if(IsHeng(bh0,bl) == false) then
	bflag = 0
end

//##:110003:----
local bh1 = GetBH(1)
if(IsShu(bh1,bl) == false) then
	bflag = 0
end
if(BH2BHXiangJie(bh0,bh1,2,0) == false) then
	pflag = 0
end
//##:110004:----
local bh2 = GetBH(2)
if(IsDian(bh2,1) == false) then
	print"diancuolediancuole"
	bflag = 0
end
if (BH2BHXiangJie(bh1,bh2,2,0) == false) then
	pflag = 0
end

//&&:230058
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	if(IsZhongDianQieFen(bh0,bh1) == false)then
		uflag = 0
		print"IsZhongDianQieFen"
	end
	if(IsTuChuZhuBi(nil,bh1) == false)then
		uflag = 0
		print"IsTuChuZhuBi"
	end
	if(IsGoldenSection(bh1,bh2,0)== false)then
		uflag = 0
		print"IsGoldenSection"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end

----------下的整字规则
//##begin
	//^^下：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
	//##end

----------火的部件规则
//##begin
//##:110004:----
local bh0 = GetBH(0)
if(IsDian(bh0,bl) ==false) then --第一个笔画点
bflag=0
end

//##:110009:----
local bh1 = GetBH(1)
if(IsPie(bh1,bl) ==false) then --第二笔是撇
bflag=0
end

//##:110009:----
local bh2 = GetBH(2)
if(IsPie(bh2,bl) ==false) then --第三笔是撇
bflag=0
end


//##:110014:----
local bh3 = GetBH(3)
if(IsNa(bh3,bl) ==false) then --第四笔是捺
bflag=0
end
if(BH2BHXiangJie(bh2,bh3,2,0)==false) then --第三笔的撇的线段2 和 捺的起点0 相接
pflag=0
end
//&&:240034
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	if(IsZhongXinPingWen(bh2,bh3) == false)then
		uflag = 0
		print"IsZhongXinPingWen"
	end
	local BHtable1 = {}
	local PTtable1 = {}
	BHtable1[1]=bh0
	BHtable1[2]=bh1
	PTtable1[1]=GetMidIndex(bh0)
	PTtable1[2]=GetMidIndex(bh1)
	if(IsShuiPingPingQi(BHtable1,PTtable1) == false)then
		uflag = 0
		print"shuipingpingqi1"
	end

	local BHtable2 = {}
	local PTtable2 = {}
	BHtable2[1]=bh2
	BHtable2[2]=bh3
	PTtable2[1]=GetMidIndex(bh2)
	PTtable2[2]=GetMidIndex(bh3)
	if(IsShuiPingPingQi(BHtable2,PTtable2) == false)then
		uflag = 0
		print"shuipingpingqi2"
	end

	if(IsGoldenSection(bh2,bh3,0)== false)then
		uflag = 0
		print"IsGoldenSection"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end

--------------火的整字规则
//##begin
	//^^火：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
	//##end
--水平平齐
--------------人的部件规则
//##begin
	//##:110009:----
	local bh0 = GetBH(0)
	if(IsPie(bh0,bl) == false) then
		bflag = 0
	end
	//##:110014:----
	local bh1 = GetBH(1)
	if(IsNa(bh1,bl) ==false) then
		bflag = 0
	end
	if(BH2BHXiangJie(bh0,bh1,2,0) == false) then
		pflag = 0
	end
	//&&:220086
	if( bflag ~= 0 and pflag ~= 0)then
	local BHtable1 = {}
	local PTtable1 = {}
		BHtable1[1]=bh0
		BHtable1[2]=bh1
		PTtable1[1]=GetEndIndex(bh0)
		PTtable1[2]=GetEndIndex(bh1)
		if(IsShuiPingPingQi(BHtable1,PTtable1) == false)then
			uflag = 0
			print"shuipingpingqi1"
		end
	errorBHPoint=ReturntypeInfo()
	end
	//##end
--------------人的整字规则
//##begin
	//^^人：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	errorBHPoint=ReturntypeInfo()
	end
//##end
