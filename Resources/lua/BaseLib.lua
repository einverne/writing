-----------------------------------------
--元函数库
--TODO返回字符串：最终返回一个字符串，给界面层使用
--TODO 原函数异常处理，包括类型判断，边界判断等等
-----------------------------------------

local print = print
local type = type
local loadstring = loadstring
local math = math
local pairs = pairs
local error = error
module("BaseLib")

local WriteHZ  = nil
local HZStruct = nil
local RunRule = nil
local WZEnv = nil

--代码运行结果返回信息
allInfoStr = ""
retInfoStr = ""

--#####运行环境设置函数#####--
function clearRetInfoStr()
	retInfoStr = ""
end

function setWriteZiInfo(WZ)
	WriteHZ = WZ
end

function setHZStruct(HS)
	HZStruct = HS
end

function setAllRule(RC)
	RunRule = RC
end


function setWZEnv(env)
	WZEnv = env
end


--#####元函数实现#####--
--所有操作索引从0开始
function GetBH( idx )
	local bh = WriteHZ.strokes[idx + 1]
	return bh
end

--参数为bd或bh都可
function GetStartPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[1].x
	point.y = bh.ptSet[1].y
	return point
end

function GetEndPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[#bh.ptSet].x
	point.y = bh.ptSet[#bh.ptSet].y
	return point
end

--获得中点
function	GetMidPoint ( bh )
 	local point = WZEnv.POINT:new()
	local len= #bh.ptSet
	local tmpIdx = math.floor ( len / 2 )
	point.x = bh.ptSet[tmpIdx].x
	point.y = bh.ptSet[tmpIdx].y
	return point
end


--获得点的横纵坐标
function GetPointX ( pt )
	return pt.x
end

function GetPointY ( pt )
	return pt.y
end



--获取某一笔段 笔段索引从0开始
function GetBDByBH(bh,bdIdx)
	local bd = WZEnv.BD:new()
	local preIdx = 1
	local postIdx = #bh.ptSet
	if (bdIdx ~= 0) then
		preIdx = bh.InflectionPoint[bdIdx ]
	end

	if (bdIdx < #bh.InflectionPoint) then
	   postIdx = bh.InflectionPoint[bdIdx + 1]
	end
	local bdPtSet = {}
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	return bd
end


--计算curIdx对应的角度
function Cal_Angle(prePt,curPt,postPt)
	local vecX = {}
	local vecY = {}
	vecX.x = prePt.x - curPt.x
	vecX.y = prePt.y - curPt.y
	vecY.x = postPt.x - curPt.x
	vecY.y = postPt.y - curPt.y
	disX = math.sqrt(vecX.x*vecX.x + vecX.y*vecX.y)
	disY = math.sqrt(vecY.x*vecY.x + vecY.y*vecY.y)
	cosXY = ( vecX.x*vecY.x + vecX.y*vecY.y )/(disX*disY)
	return math.acos(cosXY)
end


function sortingFun(a,b)
	if b.angle < a.angle then
			return false
	elseif b.angle > a.angle then
			return true
	else
			return false
	end
end


--获得笔画拐点的个数
function GetTurningPtNum(bh,BDNum)
	local n_step = 5			--lua索引从1开始
	if( #bh.ptSet <2*n_step+1) then
		return nil
	end
	local n_curIdx = n_step+1
	local n_preIdx = 1
	local n_postIdx = 2*n_step+1
	local angleArr = {}
	--计算点对应的角度
	for i = n_curIdx,(#bh.ptSet)-n_step do
		local ele = {}
		local angle = Cal_Angle(bh.ptSet[n_preIdx],bh.ptSet[i],bh.ptSet[n_postIdx])
		ele.pos = i
		ele.angle = angle
		table.insert(angleArr,ele)
		n_preIdx = n_preIdx + 1
		n_postIdx = n_postIdx + 1
	end
	--把角度按照从小到大排序
	table.sort(angleArr,sortingFun)
	local nCandidateNum = #angleArr

	local CInflectionPts = {}		--存储候选拐点的索引
	--选出了最多BDNum+1个拐点，存储在CInflectionPts中
	local f_angleThres = 12/18*3.14;
	for p = 1,nCandidateNum do
		local cFlag = false
		for q = 1,#CInflectionPts do
			if( math.abs(angleArr[p].pos - CInflectionPts[q]) < n_step) then
				cFlag = true
			end
		end

		if (cFlag == false) then
			if(angleArr[p].angle < f_angleThres) then
				CInflectionPts[#CInflectionPts+1] = angleArr[p].pos
			end
		end
	end

	--从CInflectionPts中继续选出最终拐点，
	--这样主要是为了屏蔽笔画尖端的抖动，减小拐点检测的误差
	local n_prePos
	local n_postPos
	table.sort(CInflectionPts)

--如果选出的拐点数目为0，直接返回，此时CInflectionPts是空的
	if(#CInflectionPts <= 0) then
		return 0
	end
	--如果拐点数目比标准少，将拐点数组赋值，返回
	if (#CInflectionPts < BDNum - 1) then
		bh.InflectionPoint = CInflectionPts
		return #CInflectionPts
	end

   --如果拐点数目大于或者等于标准值
	if (#CInflectionPts >= BDNum-1) then
		--如果多出一个拐点，伪拐点一定是在第一个或者最后一个，分别计算其距离首位点的距离
		local pos1 = CInflectionPts[1]
		local pos2 = CInflectionPts[#CInflectionPts]
		local spt = bh.ptSet[1]
		local ept = bh.ptSet[#bh.ptSet]
		local dis1 = math.sqrt( math.pow(bh.ptSet[pos1].x - spt.x,2) + math.pow(bh.ptSet[pos1].y - spt.y,2))
		local dis2 = math.sqrt( math.pow(bh.ptSet[pos2].x - ept.x,2) + math.pow(bh.ptSet[pos2].y - ept.y,2))
		--1.如果第一个拐点是抖动点
		if( dis1 < 5 ) then
			n_prePos = CInflectionPts[1]
			n_postPos = #bh.ptSet
			--把抖笔段从笔画中删除
			for i = 1, n_prePos-1 do
				table.remove(bh.ptSet,1)
			end

			--从拐点数组中删除该拐点
			table.remove(CInflectionPts,1)

			--移动拐点索引数组中索引的位置
			if ( #CInflectionPts > 0 ) then
				for i = 1,#CInflectionPts do
					 CInflectionPts[i] = CInflectionPts[i] - n_prePos + 1
				end
			end
		end
		--2.如果最后一个拐点是抖动点
		if (dis2 < 5) then
			n_prePos = 1
			n_postPos = CInflectionPts[#CInflectionPts]
			--从拐点表格中删除该拐点
			table.remove(CInflectionPts,#CInflectionPts)
			--删除抖笔段
			for i = #bh.ptSet,n_postPos+1,-1 do
				table.remove(bh.ptSet,i)
			end
		end
		bh.InflectionPoint = CInflectionPts
		return #CInflectionPts
	end
end


--获得笔画的拐点 索引从0开始
function GetTurningPt(bh,index)
	local ptIdx = bh.InflectionPoint[index + 1]
	return bh.ptSet[ptIdx]
end



 --获得两点间距离
 function GetDistance(pt1,pt2)
	local dis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
	return dis
 end



 function GetBDNum(bh)
	 local tnum = GetTurningPtNum(bh)
	 local bdnum = tnum + 1
	 return bdnum
 end


 function GetBHLen ( bh )

 end

--[[
--boolean number string table {GeoType} function
function trace(var)
	local varType = type(var)
	if(varType == "boolean") then
		print("boolean:")
		print(var)
	elseif(varType == "number") then
		print("number:")
		print(var)
	elseif(varType == "string") then
		print("string:")
		print(var)
	elseif(varType == "function") then
		print("function:")
		print(var)
	elseif(varType == "table") then
		if(var.GeoType == nil) then
			print("table:")
			print(var)
		elseif(var.GeoType == "KBH") then
			print("KBH:")
			print("start point: "..var.ptSet[1].x,var.ptSet[1].y)
			print("end point: "..var.ptSet[#var.ptSet].x,var.ptSet[#var.ptSet].y)
		elseif(var.GeoType == "KPOINT") then
			print("KPOINT:")
			print(var.x,var.y)
		elseif(var.GeoType == "KBD") then
			print("KBD:")
			print("start point: "..var.ptSet[1].x,var.ptSet[1].y)
			print("end point: "..var.ptSet[#var.ptSet].x,var.ptSet[#var.ptSet].y)
		elseif(var.GeoType == "KRECT") then
			print("KRECT:")
			print("left: "..var.Edges.left..", right: "..var.Edges.left..", top: "..var.Edges.top..", bottom: "..var.Edges.bottom)
		end
	end
end
]]--


function trace(var)
	retInfoStr = ""
	local varType = type(var)
	if(varType == "string") then
		retInfoStr = var .. "\r\n"
	end
	if(varType == "table") then
			if(var.GeoType == "KBH") then
			retInfoStr = "KBH:\r\n"
			retInfoStr = retInfoStr .. "start point: "..var.ptSet[1].x..","..var.ptSet[1].y
			retInfoStr = retInfoStr .. "\r\n"
			retInfoStr = retInfoStr .. "end point: "..var.ptSet[#var.ptSet].x..","..var.ptSet[#var.ptSet].y
			retInfoStr = retInfoStr .. "\r\n"
			end
	end
	if (varType == "number") then
		retInfoStr = var .. "\r\n"
	end
	allInfoStr = allInfoStr .. retInfoStr
end



--获得经过点pt1 pt2的直线方程ax+by+c = 0 返回数组linevar中依次是系数a b c
function GetLine(pt1,pt2)
	local linevar = {}
	linevar[1] = pt2.y - pt1.y
	linevar[2] = pt1.x - pt2.x
	linevar[3] = pt1.y*pt2.x - pt2.y*pt1.x
	return linevar
end

--判断点是否在直线下边
function Point2LineDown(pt,line)
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result < 0) then
		return true
	end
	return false
end

--判断点是否在直线的上边
function Point2LineUp(pt,line)
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result > 0) then
		return true
	end
	return false
end



--获得最左面的点 bh/bd
function GetLeftMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	if ( bh==nil ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x > bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--获得最右面的点 bh/bd
function GetRightMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	if ( bh==nil ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x < bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--获得最上方的点
function GetTopMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.y > bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--获得最下方的点 bd/bd
function GetBottomMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end

	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.y < bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--获得bh上离pt最近的点
function GetVPoint(bh,pt)
	local minPtIndex = 0
	local minDis = GetDistance(pt,bh.ptSet[1])
	for i = 2,#bh.ptSet do
		local tmpDis = GetDistance(pt,bh.ptSet[i])
		if (tmpDis < minDis) then
			minDis = tmpDis
			minPtIndex = i
		end
	end
	return bh.ptSet[minPtIndex]
end


--获得两个笔画的交点  如果有交点 返回该交点
--							  如果没有交点  返回nil
function GetJoint(bh1, bh2)
	local disThreshold = 3
	local minDis = 256
	local index1 = 1
	local index2 = 1
	for i = 1,#bh1.ptSet do
		for j = 1,#bh2.ptSet do
			local pt1 = bh1.ptSet[i]
			local pt2 = bh2.ptSet[j]
			local tmpDis = GetDistance(pt1,pt2)
			if (tmpDis < minDis) then
				index1 = i
				index2 = j
				minDis = tmpDis
			end
		end
	end
	if (minDis > disThreshold) then
		return nil
	end
	local pts = bh1.ptSet[index1]
	local pte = bh2.ptSet[index2]
	local jpt = {}
	jpt.x = (pts.x + pte.x) / 2
	jpt.y = (pte.x + pte.y) / 2
	return  jpt
end



--pt 到ax + by + c = 0的距离
function Cal_Point2LineDis( pt, a, b, c)
	local x = pt.x
	local y = pt.y
	local m = math.abs(a*x+b*y+c)
	local n = 	math.sqrt(a*a+b*b)
	local dis = m/n
	return dis
end

--判断pt在直线ax+by+c=0的方位
function Cal_Direction(pt, a, b,c)
	local x = pt.x
	local y = pt.y
	local dir = a*x+b*y+c
	return dir
end


--获得笔画到直线 line ax + by + c = 0距离最远的距离
function GetFarthestPt2Line(bh,line)
	local a,b,c = line[1],line[2],line[3]
	local maxDis = 0
	local index = 1
	for i = 1,#bh.ptSet do
		local pt = bh.ptSet[i]
		local dis = Cal_Point2LineDis(pt,a,b,c)
		if (dis > maxDis) then
			maxDis = dis
			index = i
		end
	end
	return maxDis
end



--获得笔画到直线line  ax + by + c = 0的
function GetBHTrend(bh,line)
	local a,b,c = line[1],line[2],line[3]
	local trendArray = {}
	trendArray[#trendArray + 1 ] = 0   --第一个tag初始化为0
	local preDis = 0
	--1 表示后面的
	for i = 2,#bh.ptSet do
		local curPt = bh.ptSet[i]
		local curDis = Cal_Point2LineDis(curPt,a,b,c)
		if(curDis > preDis) then
			if (trendArray[#trendArray] == 0 )then
				trendArray[#trendArray + 1] = 1
			end
		end
		if (curDis < preDis) then
			if(trendArray[#trendArray] == 1) then
				trendArray[#trendArray + 1] = 0
			end
		end
		preDis = curDis
	end
	return trendArray
end

