--单元字规则
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
--------------人的整字规则kkk
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
--中点切分
--------------中的部件
//##begin
//##:110003:----

local bh0 = GetBH(0)
if(IsShu(bh0,bl) ==false) then
	bflag = 0
end

//##:110018:----
local bh1 = GetBH(1)
if(IsHengZhe(bh1,bl) ==false) then
	bflag = 0
 end
bd0 = GetBDByBH(bh1,0)
if(BH2BHXiangJie(bh0,bd0,0,0) == false) then
	pflag = 0
 end

//##:110007:----
local bh2 = GetBH(2)
if(IsHeng(bh2,bl) ==false) then
	bflag = 0
end

//##:110003:----
local bh3 = GetBH(3)
if(IsShu(bh3,bl) ==false) then
	bflag = 0
 end
if( bflag ~= 0 and pflag ~= 0)then
	local tempbh = allBHTable[2]
	local bd1 =tempbh.bd[1]
	if(IsZhongDianQieFen(bd1,bh3) == false)then
		uflag = 0
		print"IsZhongDianQieFen"
	errorBHPoint=ReturntypeInfo()
	end
end
//##end
--中的整字
//##begin
	//^^中：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	errorBHPoint=ReturntypeInfo()
	end
//##end
--黄金分割
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
if( bflag ~= 0 and pflag ~= 0)then
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
--首点居正
--文的部件规则
//##begin
//##:110004:----
local bh0 = GetBH(0)
if(IsDian(bh0,bl) ==false) then
bflag = 0
 end

//##:110007:----
local bh1 = GetBH(1)
if(IsHeng(bh1,bl) ==false) then
bflag = 0
 end

//##:110009:----
local bh2 = GetBH(2)
if(IsPie(bh2,bl) ==false) then
bflag = 0
 end
if(BH2BHXiangJie(bh1,bh2,2,0)==false) then --横的线段和撇的起始点相接
pflag=0
end
//##:110014:----
local bh3 = GetBH(3)
if(IsNa(bh3,bl) ==false) then
bflag = 0
 end
if (BH2BHXiangJiao(bh2,bh3) == false) then
pflag = 0
end

//&&:240029
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	if(IsShouDianJuZheng(bh0,bh1)== false)then
		uflag = 0
		print"IsShouDianJuZheng"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end
--文的整字规则
//##begin
	//^^文：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
//##end
--竖直等分
--三的部件规则
//##begin
//##:110007:----
local bh0 = GetBH(0)
if(IsHeng(bh0,bl) ==false) then
	bflag = 0
 end

//##:110007:----
local bh1 = GetBH(1)
if(IsHeng(bh1,bl) ==false) then
	bflag = 0
 end
if(BH2BHUp(bh0,bh1) == false)then
	pflag = 0
end
//##:110007:----
local bh2 = GetBH(2)
if(IsHeng(bh2,bl) ==false) then
	bflag = 0
 end
if(BH2BHUp(bh1,bh2) == false)then
	pflag = 0
end
//&&:230071
if( bflag ~= 0 and pflag ~= 0)then
	local BHtable = {}
	local PTtable = {}
	BHtable[1]=bh0
	BHtable[2]=bh1
	BHtable[3]=bh2
	PTtable[1] = GetMidIndex(bh0)
	PTtable[2] = GetMidIndex(bh1)
	PTtable[3] = GetMidIndex(bh2)
	if(IsShuZhiDengFen(BHtable,PTtable) == false)then
		uflag = 0
		print"IsShuZhiDengFen"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end
--三的整字
//##begin
	//^^三：----
	if( uflag ~= 0)then
		if(IsAspectRatio(2) == false)then
			cflag = 0
			print"宽高比budui"
		end
		errorBHPoint=ReturntypeInfo()
	end
//##end
--水平等分
--山的部件
//##begin
//##:110003:----
local bh0 = GetBH(0)
if(IsShu(bh0,bl) ==false) then
	bflag = 0
 end

//##:110001:----
local bh1 = GetBH(1)
if(IsShuZhe(bh1,bl) ==false) then
	bflag = 0
 end
local bd0 = GetBDByBH(bh1,1)
if (BH2BHXiangJie(bd0,bh0,2,1) ==false) then
	pflag = 0
 end

//##:110003:----
local bh2 = GetBH(2)
if(IsShu(bh2,bl) ==false) then
	bflag = 0
 end
 local bh1 = GetBH(1)
if (BH2BHXiangJie(bh1,bh2,1,1) ==false) then
	pflag = 0
 end

//&&:230006
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	local tempbh = allBHTable[2]
	local bd1 =tempbh.BD[1]
	local ShuBHtable = {}
	local PTtable = {}

	ShuBHtable[1]=bd1
	ShuBHtable[2]=bh0
	ShuBHtable[3]=bh2
	PTtable[1] =  GetMidIndex(bd1)
	PTtable[2] =  GetMidIndex(bh0)
	PTtable[3] =  GetMidIndex(bh2)
	if(IsShuiPingDengFen(ShuBHtable,PTtable) == false)then
		uflag = 0
		print"IsShuiPingDengFen"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end
--山的整字
//##begin
	//^^山：----
	if( uflag ~= 0)then
		if(IsAspectRatio() == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
//##end
--重心平稳
--大的部件
//##begin
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
	print(pflag)
end
//&&:230082
if( bflag ~= 0 and pflag ~= 0)then
	if(IsZhongXinPingWen(bh1,bh2) == false)then
		uflag = 0
		print"zhongxinpingwen"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end
--大的整字
//##begin
//^^大：----
if( uflag ~= 0)then
	if(IsAspectRatio(2) == false)then
		cflag = 0
		print"宽高比budui"
	end
end
//##end
--竖直平齐
--片的部件
//##begin
//##:110009:----
local bh0 = GetBH(0)
if(IsPie(bh0,bl) ==false) then --第一笔是撇
bflag=0
end

//##:110003:----
local bh1 = GetBH(1)
if(IsShu(bh1,bl) ==false) then --第二笔是竖
bflag=0
end

//##:110007:----
local bh2 = GetBH(2)
if(IsHeng(bh2,bl) ==false) then --第三笔是横
bflag=0
end
if(BH2BHXiangJie(bh1,bh2,1,2)==false) then --竖的终点1 和 横的线段2 相接
pflag=0
end

//##:110018:----
local bh3 = GetBH(3)
if(IsHengZhe(bh3,bl) ==false) then --第四笔是横折
bflag=0
end

//&&:240099
if( bflag ~= 0 and pflag ~= 0)then
	local tempbh = allBHTable[4]
	local bd1 =tempbh.bd[2]
	local BHtable = {}
	local PTtable = {}
	BHtable[1]=bh1
	BHtable[2]=bd1
	PTtable[1]=GetMidIndex(bh1)
	PTtable[2]=GetMidIndex(bd1)
	if(IsShuZhiPingQi(BHtable,PTtable) == false)then
		uflag = 0
		print"IsShuZhiPingQi"
	end
	errorBHPoint=ReturntypeInfo()
end

//##end
--片的整字
//##begin
//^^片----
if( uflag ~= 0)then
	if(IsAspectRatio(1) == false)then
		cflag = 0
		print"宽高比budui"
	end
end
//##end
--突出主笔
--干的部件规则
//##begin
//##:110007:----
local bh0 = GetBH(0)
if(IsHeng(bh0,bl) ==false) then
	bflag = 0
 end

//##:110007:----
local bh1 = GetBH(1)
if(IsHeng(bh1,bl) ==false) then
	bflag = 0
end
if(BH2BHUp(bh0,bh1) == false)then
	pflag = 0
end
//##:110003:----
local bh2 = GetBH(2)
if(IsShu(bh2,bl) ==false) then
	bflag = 0
 end
if (BH2BHXiangJiao(bh1,bh2) == false) then
	pflag = 0
end
//&&:230069
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	if(IsTuChuZhuBi(nil,bh2) == false)then
		uflag = 0
		print"IsTuChuZhuBi"
	end
end
//##end
--干的整字规则
//##begin
	//^^干：----
	if( uflag ~= 0)then
		if(IsAspectRatio(1) == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
//##end
--长度比例
--二的部件
//##begin
//##:110007:----
local bh0 = GetBH(0)
if(IsHeng(bh0,bl) ==false) then
	bflag = 0
 end

//##:110007:----
local bh1 = GetBH(1)
if(IsHeng(bh1,bl) ==false) then
	bflag = 0
end
if(BH2BHUp(bh0,bh1) == false)then
	pflag = 0
end
//&&:220053
print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
if( bflag ~= 0 and pflag ~= 0)then
	if(IsChangDuBiLi(bh1,bh0,1) == false)then
		uflag = 0
		print"IsChangDuBiLi"
	end
	errorBHPoint=ReturntypeInfo()
end
//##end
--二的整字
//##begin
	//^^二：----
	if( uflag ~= 0)then
		if(IsAspectRatio(2) == false)then
			cflag = 0
			print"宽高比budui"
		end
	end
//##end
