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
local StdHZ = nil

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

function setStdHZ(SZ)
	StdHZ = SZ
end
--#####元函数实现#####--
--所有操作索引从0开始
function GetBH( idx )
	local bh = WriteHZ.strokes[idx + 1]
	return bh
end

--注意与GetBH的区别，这里取得的是标准字的笔画
function GetPreBH(idx)
	local bh = StdHZ.strokes[idx + 1]
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

function GetBDByBH(bh,bdIdx)
	local bd = WZEnv.BD:new()
	local preIdx = 1
	local postIdx = #bh.ptSet
	if (bdIdx ~= 0) then
		preIdx = bh.InflectionPoint[bdIdx] + 1
	end
	if (bdIdx < #bh.InflectionPoint) then
	  postIdx = bh.InflectionPoint[bdIdx + 1] + 1
	end
	local bdPtSet = {}
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	return bd
end

function GetBDLen(bd)
	local len = 0
	for i = 1,#bd.ptSet - 1 do
		local curpt = bd.ptSet[i]
		local nextpt = bd.ptSet[i+1]
		local dis = math.sqrt( math.pow(curpt.x - nextpt.x,2) + math.pow(curpt.y - nextpt.y,2))
		len = len + dis
	end
	return len
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
	return  math.deg(math.acos(cosXY))
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


function GetAngel(spt,ept)
	local degree = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		degree = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))

	if (degree < 0) then
			degree = degree + 180
		end

	else
		if (ept.y < spt.y) then
			degree = -90
		else
			degree = 90
		end
	end
	return degree
end

--获得拐点，暂未用到
function GetTurningPtNum(bh,BDNum)
	local height = 512
	local threshold_len = height / 28
	local threshold_count = 7

	local turning_ind = {}

	local len = GetBDLen(bh)
	if (len < threshold_len or #bh.ptSet <threshold_count  ) then
		return 0
	end

	local index_begin = 1
	local index_candidate = 4
	local index_end = 7

	while (index_end < #bh.ptSet - 1) do
		local it0 = bh.ptSet[1]
		local it1 = bh.ptSet[1]
		local it2 = bh.ptSet[1]
		local it0_index,it1_index,it2_index
		for i = 1,index_begin do
			it0 = bh.ptSet[i]
			it0_index = i
		end


		for i = 1, index_candidate do
			it1 = bh.ptSet[i]
			it1_index = i
		end


		for i = 1,index_end  do
			it2 = bh.ptSet[i]
			it2_index = i
		end


		local pt1 = it0
		local pt2 = it1
		local pt3 = it2

		local ang1 = GetAngel(pt1,pt2)
		local ang2 = GetAngel(pt2,pt3)
		local dis = math.abs(ang1 - ang2)

		if (dis > 180) then
			dis = 360 - dis
		end
		if (dis > 55) then
			if (#bh.ptSet - 1 - index_candidate < 4) then
				turning_ind[#turning_ind + 1] = index_candidate
				index_begin = index_candidate
				index_candidate = index_begin + 3
				index_end = index_candidate + 3
			else
				it1_index = it1_index + 1
				it2_index = it2_index + 1
				it1 = bh.ptSet[it1_index]
				it2 = bh.ptSet[it2_index]
				local a1 = GetAngel(it0,it1)
				local a2 = GetAngel(it1,it2)
				local dis1 = math.abs(a2 - a1)
				if(dis1 > 180) then
					dis1 = 360 - dis1
				end
				it1_index = it1_index + 1
				it2_index = it2_index + 1
				it1 =  bh.ptSet[it1_index]
				it2 = bh.ptSet[it2_index]
				a1 = GetAngel(it0,it1)
				a2 = GetAngel(it1,it2)
				local dis2 = math.abs(a2-a1)
				if(dis2 > 180) then
					dis2 = 360 - dis2
				end
				local f = math.max(dis,dis1,dis2)
				if (f == dis) then
					turning_ind[#turning_ind + 1] = index_candidate
					index_begin = index_candidate
					index_candidate = index_candidate + 3
					index_end = index_candidate + 3
				end

				if (f == dis1) then
					turning_ind[#turning_ind + 1] = index_candidate + 1
					index_begin = index_candidate + 1
					index_candidate = index_candidate + 4
					index_end = index_candidate + 3
				end


				if (f == dis2)then
					turning_ind[#turning_ind + 1] = index_candidate + 2
					index_begin = index_candidate + 2
					index_candidate = index_candidate + 5
					index_end = index_candidate + 3
				end

				it1_index = it1_index - 2
				it2_index = it2_index - 2
				it1 = bh.ptSet[it1_index]
				it2 = bh.ptSet[it2_index]
			end
		else
		index_candidate =  index_candidate + 1
		index_end = index_end + 1
	end
end


---------------------------------去除抖动-----------------------------------------------
--如果选出的拐点数目为0，直接返回，此时CInflectionPts是空的
	if(#turning_ind <= 0) then
		return 0
	end
	--如果拐点数目比标准少，将拐点数组赋值，返回
	if (#turning_ind < BDNum - 1) then
		bh.InflectionPoint = turning_ind
		return #turning_ind;
	end


	--如果拐点数目大于或者等于标准值
	if (#turning_ind >= BDNum-1) then
		--如果多出一个拐点，伪拐点一定是在第一个或者最后一个，分别计算其距离首位点的距离
		local pos1 = turning_ind[1]
		local pos2 = turning_ind[#turning_ind]
		local spt = bh.ptSet[1]
		local ept = bh.ptSet[#bh.ptSet]
		local dis1 = math.sqrt( math.pow(bh.ptSet[pos1].x - spt.x,2) + math.pow(bh.ptSet[pos1].y - spt.y,2))
		local dis2 = math.sqrt( math.pow(bh.ptSet[pos2].x - ept.x,2) + math.pow(bh.ptSet[pos2].y - ept.y,2))
		--1.如果第一个拐点是抖动点
		if( dis1 < 30 ) then
			n_prePos = turning_ind[1]
			n_postPos = #bh.ptSet
			--把抖笔段从笔画中删除
			for i = 1, n_prePos-1 do
				table.remove(bh.ptSet,1)
			end
			--从拐点数组中删除该拐点
			table.remove(turning_ind,1)
			--移动拐点索引数组中索引的位置
			if ( #turning_ind > 0 ) then
				for i = 1,#turning_ind do
					 turning_ind[i] = turning_ind[i] - n_prePos + 1
				end
			end
		end
		--2.如果最后一个拐点是抖动点
		if (dis2 < 30) then
			n_prePos = 1
			n_postPos = turning_ind[#turning_ind]
			--从拐点表格中删除该拐点
			table.remove(turning_ind,#turning_ind)
			--删除抖笔段
			for i = #bh.ptSet,n_postPos+1,-1 do
				table.remove(bh.ptSet,i)
			end
		end

	--计算拐点之间的距离
	if (#turning_ind > 1) then
		for i = 1, #turning_ind - 1  do
			print (turning_ind[i])
			print (turning_ind[i + 1])
			local firstIndex = turning_ind[i]
			local nextIndex = turning_ind[i + 1]
			local pt1 = bh.ptSet[firstIndex]
			local pt2 = bh.ptSet[nextIndex]
			local distance = GetDistance(pt1,pt2)
			print (distance)
		end
	end
		bh.InflectionPoint = turning_ind
		print ("拐点个数为...")
		print (#turning_ind)
		print ("test over")
		return #turning_ind
	end
end




--[[
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
]]--


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

	local pt = WZEnv.POINT:new()
	local index = 1
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x > bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			index = i
			end
	end
	return pt,index
end

--获得最右面的点 bh/bd
function GetRightMostPoint ( bh )
	local pt = WZEnv.POINT:new()
	local index = 1
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x < bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			index = i
			end
	end
	return pt,index
end

--获得最上方的点
function GetTopMostPoint ( bh )
	local pt = WZEnv.POINT:new()
	local index = 1
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end

	for i =1,#bh.ptSet do
		if ( pt.y > bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			index = i
			end
	end
	return pt,index
end

--获得最下方的点 bd/bd
function GetBottomMostPoint ( bh )
	local pt = WZEnv.POINT:new()
	local index = 1
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.y < bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			index =  i
			end
	end
	return pt,index
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


--获得笔画到直线 line ax + by + c = 0距离最远的点
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
	return bh.ptSet[index],index
end

--------
function GetFarDis2Line(bh,line)
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


--返回bh的ptSet中，索引为preIdx和postIdx之间的点集组成的笔段
function GetTempBD(bh,preIdx,postIdx)
	local bd = WZEnv.BD:new()
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	return bd
end


--传入的参数为当前笔画的索引.笔画索引均是从0开始.
function IsPosRight(idx)
	local wbh = WriteHZ.strokes[idx + 1]
	local sbh = StdHZ.strokes[idx + 1]

	local wspt = wbh.ptSet[1]
	local wept = wbh.ptSet[#wbh.ptSet]

	local sspt = sbh.ptSet[1]
	local sept = sbh.ptSet[#sbh.ptSet]

	local xMin = sspt.x - 50
	local xMax = sspt.x + 50

	local yMin = sspt.y - 50
	local yMax = sspt.y + 50

	if(wspt.x < xMin or wspt.x > xMax or wspt.y < yMin or wspt.y > yMax) then
		return false
	else
		return true
	end
end



function PointSame(pt1,pt2)
	local IsSame = false
	if (pt1.x == pt2.x and pt1.y == pt2.y) then
		IsSame = true
	end
	return IsSame
end

--[[
function SmallXiangJiao(pt11,pt12,pt21,pt22)
	local flag = false
	if (PointSame(pt11,pt21) or PointSame(pt11,pt22) or PointSame(pt12,pt21) or PointSame(pt12,pt22))then
		flag = true
	else
		local k1,c1
		local k2,c2
		if(pt11.x == pt12.x) then
			k1 = 0
			c1 = pt11.y
			print("k1 = 0 as defined ")
		else
			print("k1 = 0 after calculated")
			k1 = (pt12.y - pt11.y)/(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
		end
		if (pt21.x == pt22.x) then
			k2 = 0
			c2 = pt21.y
			print("k2 = 0 as defined")
		else
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			print("k2 = 0 as calculated")
		end
		if (k1 == k2) then		--如果两线平行
			print(pt11.x,pt11.y,pt12.x,pt12.y)
			print(pt21.x,pt21.y,pt22.x,pt22.y)
			print("para and k value is "..k1)
			return false
		else
			local vp = {}
			vp.x = (c1 - c2) /(k2 - k1)
			vp.y = (vp.x*k1) + c1
			local maxX1 = pt11.x
			local minX1 = pt12.x
			if(pt11.x < pt12.x) then
				maxX1 = pt12.x
				minX1 = pt11.x
			end
			local maxY1 = pt11.y
			local minY1 = pt12.y
			if (pt11.y < pt12.y) then
				maxY1 = pt12.y
				minY1 = pt11.y
			end

			local maxX2 = pt21.x
			local minX2 = pt22.x
			if (pt21.x < pt22.x) then
				maxX2 = pt22.x
				minX2 = pt21.x
			end

			local maxY2 = pt21.y
			local minY2 = pt22.y
			if (pt21.y < pt22.y) then
				maxY2 = pt22.y
				minY2 = pt21.y
			end

			print(vp.x,vp.y)
		--	print(minX1,maxY1,minX2,maxY2)
			if (vp.x >= minX1  and vp.x <= maxX1
			and vp.x >= minX2 and vp.x <= maxX2
			and vp.y >= minY1 and vp.y <= maxY1
			and vp.y >= minY2 and vp.y <= maxY2) then
				flag = true
			end
		end
	end
	return flag
end

]]--



function SmallXiangJiao(pt11,pt12,pt21,pt22)
	local flag1 = 0
	local flag2 = 0
	if (PointSame(pt11,pt21) or PointSame(pt11,pt22) or PointSame(pt12,pt21) or PointSame(pt12,pt22))then
		return true
	else
		if(pt11.x ==  pt12.x ) then
			flag1 = 1
		end

		if (pt21.x == pt22.x) then
			flag2 = 1
		end

		local vp = {}  --vp是两个直线段的交点
		local k1,c1
		local k2,c2
		if (flag1 == 1 and flag2 == 1)  then --如果两个直线段都平行于y轴
			return false
		elseif(flag1 == 1 ) then --如果直线段1平行于y轴
			vp.x = pt11.x
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.y = k2*(vp.x) + c2
		elseif(flag2 == 1) then --如果直线段2平行于y轴
			vp.x = pt21.x
			k1 = (pt12.y - pt11.y) /(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			vp.y = k1*(vp.x) + c1
		else  --如果两个直线段均不平行于y轴
			k1 = (pt12.y - pt11.y)/(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.x = (c1 - c2) /(k2 - k1)
			vp.y = (vp.x*k1) + c1
		end
			local maxX1 = pt11.x
			local minX1 = pt12.x
			if(pt11.x < pt12.x) then
				maxX1 = pt12.x
				minX1 = pt11.x
			end
			local maxY1 = pt11.y
			local minY1 = pt12.y
			if (pt11.y < pt12.y) then
				maxY1 = pt12.y
				minY1 = pt11.y
			end

			local maxX2 = pt21.x
			local minX2 = pt22.x
			if (pt21.x < pt22.x) then
				maxX2 = pt22.x
				minX2 = pt21.x
			end

			local maxY2 = pt21.y
			local minY2 = pt22.y
			if (pt21.y < pt22.y) then
				maxY2 = pt22.y
				minY2 = pt21.y
			end

			if (vp.x >= minX1  and vp.x <= maxX1
			and vp.x >= minX2 and vp.x <= maxX2
			and vp.y >= minY1 and vp.y <= maxY1
			and vp.y >= minY2 and vp.y <= maxY2) then
				return true
			else
				return false
			end
	end
end




function BH2BHXiangJiao(bh1,bh2)
	local flag = false
	for i = 1, #bh2.ptSet - 1 do
		for j = 1, #bh1.ptSet - 1 do
			flag = SmallXiangJiao(bh1.ptSet[j],bh1.ptSet[j+1],bh2.ptSet[i],bh2.ptSet[i+1])
		end
		if (flag == true) then
			break
		end
	end
	return flag
end


function Judge2Dots(pt1,pt2)
	local disThreshold =  50
	local distance = GetDistance(pt1,pt2)
	if (distance < disThreshold) then
		return true
	else
		return false
	end
end


function resample(bh)
	local newbh = {}
	newbh.ptSet = {}
	local n = 20
	local I = GetBDLen(bh) / (n - 1)
	local D = 0
	local dstpoList = {}
	dstpoList[1] = bh.ptSet[1]
	local i = 2
	while(true) do
			if( i <= #bh.ptSet) then
				local p1 = bh.ptSet[i - 1]
				local p2 = bh.ptSet[i]
				local dist = GetDistance(p1,p2)
				if ((D + dist) >= I) then
					local qx = p1.x + ((I - D) / dist) * (p2.x - p1.x);
					local qy = p1.y + ((I - D) / dist) * (p2.y - p1.y);
					local newp = {}
					newp.x = qx
					newp.y = qy
					table.insert(bh.ptSet, i,newp)
					dstpoList[#dstpoList + 1] = newp
					D = 0
				else
					D = D + dist
				end
					i = i + 1
			else
				break
			end
	end

	if (#dstpoList == n - 1 ) then
		dstpoList[#dstpoList + 1] = bh.ptSet[#bh.ptSet]
	end
	newbh.ptSet = dstpoList
	return newbh
end

function JudgeDotLine(pt,bd)
	local newbh = resample(bd)
	local tempDis =  512
	local disThreshold = 50
	for i =  1, #bd.ptSet do
		local curDis = GetDistance(pt,bd.ptSet[i])
		if (curDis < tempDis ) then
			tempDis =  curDis
		end
	end
	if(tempDis < disThreshold) then
		return true
	else
		return false
	end
end



--[[
function JudgeDotLine(pt,bd)
	local tempDis =  512
	local disThreshold = 50
	for i =  1, #bd.ptSet do
		local curDis = GetDistance(pt,bd.ptSet[i])
		if (curDis < tempDis ) then
			tempDis =  curDis
		end
	end
	if(tempDis < disThreshold) then
		return true
	end

	tempDis = 512
	for i = 1 ,# bd.ptSet - 1 do
		local startpt = bd.ptSet[i]
		local endpt = bd.ptSet[i + 1]
		local line = GetLine(startpt,endpt)
		local curDis = Cal_Point2LineDis(pt,line[1],line[2],line[3])
		if (curDis < tempDis) then
			tempDis = curDis
		end
	end
	if(tempDis < disThreshold) then
		return true
	end

	return false

end

]]--
--对于单笔段来说，只需要传入笔画即可
--	0 startpoint 1 endpoint 2 line
function BH2BHXiangJie(bd1,bd2,type1,type2)
	local flag = 0
	if (type1 == 0) then
		if(type2 == 0) then
			flag = Judge2Dots(bd1.ptSet[1],bd2.ptSet[1])
				elseif (type2 == 1) then
					flag = Judge2Dots(bd1.ptSet[1],bd2.ptSet[#bd2.ptSet])
						else
							flag = JudgeDotLine(bd1.ptSet[1],bd2)
						end

	elseif (type1 == 1) then
		if(type2 == 0) then
			flag = Judge2Dots(bd1.ptSet[#bd1.ptSet],bd2.ptSet[1])
				elseif (type2 == 1) then
					flag = Judge2Dots(bd1.ptSet[#bd1.ptSet],bd2.ptSet[#bd2.ptSet])
						else
							flag = JudgeDotLine(bd1.ptSet[#bd1.ptSet],bd2)
						end
	else --type1 == 2
		if(type2 == 0) then
			flag = JudgeDotLine(bd2.ptSet[1],bd1)
				elseif (type2 == 1) then
					flag = Judge2Dots(bd2.ptSet[#bd2.ptSet],bd1)
				end
	end

	return flag
end




function  IsShu(bh,bl)
	if(bl ~= 1 and bl~=2 ) then return end
	local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
	local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离
	local loose_angel_max_threshold = 45    --松评判时首尾点连线倾斜的最大角度
	local tight_angel_max_threshold = 30    --紧评判时首尾点连线倾斜的最大角度

	local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
	local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


	local startpt,startindex = GetTopMostPoint(bh)
	local endpt,endindex = GetBottomMostPoint(bh)
	if (endindex < startindex) then
		return false
	end

	 local finalbh = GetTempBD(bh,startindex,endindex)
	 local jitter1 = GetTempBD(bh,1,startindex)
	 local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

	 local len_jitter1 = GetBDLen(jitter1)
	 local len_jitter2 = GetBDLen(jitter2)
	 local len_bh = GetBDLen(finalbh)

	 local line = GetLine(startpt,endpt)
	 local dis = GetFarDis2Line(finalbh,line)

	 local angel = 90
	 if (endpt.y ~= startpt.y) then
		local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
		angel = math.deg(math.atan(slope))
	 end
	angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
			return false
     end
     if (dis > loose_dis_max_threshold) then
			return false
     end
     if (angel > loose_angel_max_threshold) then
			return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis > tight_dis_max_threshold) then
         return false
     end
     if (angel > tight_angel_max_threshold) then
         return false
     end
 end
 return true
end

function  IsHeng(bh,bl)
if (bl~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 45    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 30     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local startpt,startindex = GetLeftMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

 if (startindex > endindex) then
 return false
 end
 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

 local len_jitter1 = GetBDLen(jitter1)
 local len_jitter2 = GetBDLen(jitter2)
 local len_bh = GetBDLen(finalbh)

 local line = GetLine(startpt,endpt)
 local dis = GetFarDis2Line(finalbh,line)


 local angel = 90
 if (endpt.x ~= startpt.x) then
 local slope = (endpt.y - startpt.y) / (endpt.x - startpt.x)
 angel = math.deg ( math.atan(slope))
 end
 angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end
     if (dis > loose_dis_max_threshold) then
         return false
     end
     if (angel > loose_angel_max_threshold) then
         return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis > tight_dis_max_threshold) then
         return false
     end
     if (angel > tight_angel_max_threshold) then
         return false
     end
 end

 return true
end

function  IsShuZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离
local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 130  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =60    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 120  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =70    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineDown(turning_pt,spt_ept_line) == false) then
	return false
end
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.y ~= startpt.y) then
local slope0 = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.x ~= turning_pt.x) then
local slope1 = (endpt.y - turning_pt.y) / (endpt.x - turning_pt.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then

        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end

return true

end

function  IsShuWanGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离
local loose_angel_max_threshold = 45    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 30     --紧评判时首尾点连线倾斜的最大角度
local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_1_loose_angel_max_threshold = 130  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 60    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 120  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 70    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetTopMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
if (startindex > endindex) then
return false
end

local vpt0 = {}
vpt0.x = 0
vpt0.y = 512

local vpt1 = {}
vpt1.x = 512
vpt1.y = 512


local turning_index0 = 1
local minDis = 512
for i = 1,#bh.ptSet do
    local curpt = bh.ptSet[i]
    local tempDis = GetDistance(curpt,vpt0)
    if (tempDis < minDis ) then
        turning_index_0 = i
        minDis  = tempDis
    end
end

local turning_index_1 = 1
minDis = 512
for i = 1,#bh.ptSet do
    local curpt = bh.ptSet[i]
    local tempDis = GetDistance(curpt,vpt1)
    if (tempDis < minDis ) then
        turning_index_1 = i
        minDis  = tempDis
    end
end


bh.InflectionPoint[#bh.InflectionPoint + 1] =  turning_index0
bh.InflectionPoint[#bh.InflectionPoint + 1] =  turning_index1



local turning_pt_0 = bh.ptSet[turning_index_0]
local turning_pt_1 = bh.ptSet[turning_index_1]

local bd0 = GetTempBD(bh,startindex,turning_index_0)
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)



local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)



local angel0 = 90
if (turning_pt_0.y ~= startpt.y) then
local slope0 = (turning_pt_0.x -  startpt.x)/(turning_pt_0.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (turning_pt_1.x ~= turning_pt_0.x) then
local slope1 = (turning_pt_1.y - turning_pt_0.y) / (turning_pt_1.x - turning_pt_0.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
	if(len_bd2 < 5) then
		return false
	end
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end

return true

end

function  IsDian(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local loose_dis_max_threshold = 30      --松评判时笔迹到笔画连线的最大距离
local tight_dis_max_threshold = 15       --紧评判时笔迹到笔画连线的最大距离

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

if (endindex < startindex) then
    return false
end

if (startpt.x >= endpt.x ) then
    return false
end


local finalbh = GetTempBD(bh,startindex,endindex)
local jitter1 = GetTempBD(bh,1,startindex)
local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

local len_jitter1 = GetBDLen(jitter1)
local len_jitter2 = GetBDLen(jitter2)
local len_bh = GetBDLen(finalbh)

local line = GetLine(startpt,endpt)
local dis = GetFarDis2Line(finalbh,line)

if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end
     if(dis > loose_dis_max_threshold) then
         return false
     end
     for i = startindex, endindex, 5 do
        if (i + 5 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+5]
            if (curPt.x > nextPt.x) then
                  return false
            end

        if (curPt.y > nextPt.y)then
            return false
        end
        end
    end
end


if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end

     if(dis > tight_dis_max_threshold) then
         return false
     end

    for i = startindex, endindex, 2 do
        if (i + 2 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+2]
            if (curPt.x > nextPt.x) then
                return false
        end

        if (curPt.y > nextPt.y)then
                return false
        end
        end
    end
end
return true
end


function  IsHengGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd1_len_threshold = 2                       --笔段1的最小长度

local bd0_1_loose_angel_max_threshold = 90  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 10    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 20    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 10    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 6  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetRightMostPoint(bh)


if (startindex > endindex) then
return false
end

if (turning_pt.y > endpt.y) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)
if(len_bd1 < bd1_len_threshold ) then
return false
end

local angel0 = 90
if (turning_pt.x ~= startpt.x) then
local slope0 = (turning_pt.y -  startpt.y)/(turning_pt.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then        trace("loose_ratio_wrong")
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold ) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end

return true


end


function  IsHengZhe2(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local bd0_loose_angel_max_threshold = 30     --松评判时bd0首尾点连线倾斜的最大角度
local bd0_tight_angel_max_threshold = 15     --紧评判时bd0首尾点连线倾斜的最大角度


local bd1_loose_angel_max_threshold = 60    --松评判时bd1首尾点连线倾斜的最大角度
local bd1_tight_angel_max_threshold = 30    --紧评判时bd1首尾点连线倾斜的最大角度


local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 90  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =30    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 90  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 3    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/3  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 2  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/2  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineUp(turning_pt,spt_ept_line) == false) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.x ~= startpt.x) then
local slope0 = (turning_pt.y -  startpt.y)/(turning_pt.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.y ~= turning_pt.y) then
local slope1 = (endpt.x - turning_pt.x) / (endpt.y - turning_pt.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold or angel1 > bd1_loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold or angel1 > bd1_tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end

return true



end



function  IsXieGou(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_angel_max_threshold = 80        --spt与tpt连线偏离竖直方向的角度
local loose_angel_min_threshold = 10
local tight_angel_max_threshold = 60
local tight_angel_min_threshold = 20

local startpt,startindex = GetTopMostPoint(bh)
local endpt= GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetBottomMostPoint(bh)
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index


local angel = 90
if (turning_pt.y ~= startpt.y) then
     local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
     angel = math.deg(math.atan(slope))
end
angel = math.abs(angel)


local bd1 = GetTempBD(bh,turning_index,endindex)
local len_bd1 = GetBDLen(bd1)
if (len_bd1 < 5) then
    return false
end

if (endindex < startindex) then
    return false
end

if (startpt.x >= endpt.x ) then
    return false
end

if (turning_pt.x < startpt.x) then
    return false
end


if (bl == 1) then
    if (angel > loose_angel_max_threshold or angel < loose_angel_min_threshold) then
            return false
    end


    for i = startindex, turning_index, 5 do
        if (i + 5 < turning_index ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+5]
            if (curPt.x > nextPt.x) then
              return false
            end

        if (curPt.y > nextPt.y)then
            return false
        end
     end
    end
end


if (bl == 2) then
    if (angel > tight_angel_max_threshold or angel < tight_angel_min_threshold) then
            return false
    end

    for i = startindex, turning_index, 2 do
        if (i + 2 < turning_index ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+2]
            if (curPt.x < nextPt.x) then
                return false
        end

        if (curPt.y > nextPt.y)then
                return false
            end
        end
    end
end
return true
end


function  IsPie(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_jitter_max_threshold = 1/5
local tight_jitter_max_threshold = 1/10

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetLeftMostPoint(bh)

if (endindex < startindex) then
    return false
end

if (startpt.x <= endpt.x ) then
    return false
end


 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

 local len_jitter1 = GetBDLen(jitter1)
 local len_jitter2 = GetBDLen(jitter2)
 local len_bh = GetBDLen(finalbh)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end

    for i = startindex, endindex, 20 do
        if (i + 20 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+20]
            if (curPt.x < nextPt.x) then
                return false
			end

        if (curPt.y > nextPt.y)then
            return false
        end
        end
    end
end


if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end

    for i = startindex, endindex, 10 do
        if (i + 10 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+10]
            if (curPt.x < nextPt.x) then
                return false
        end

        if (curPt.y > nextPt.y)then
                return false
        end
        end
    end
end
return true

end


function  IsTi(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local loose_dis_max_threshold = 40      --松评判时笔迹到笔画连线的最大距离
local tight_dis_max_threshold = 20       --紧评判时笔迹到笔画连线的最大距离

local startpt,startindex = GetBottomMostPoint(bh)
local endpt,endindex = GetTopMostPoint(bh)

if (endindex < startindex) then
    return false
end

if (startpt.x >= endpt.x ) then
    return false
end


local finalbh = GetTempBD(bh,startindex,endindex)
local jitter1 = GetTempBD(bh,1,startindex)
local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

local len_jitter1 = GetBDLen(jitter1)
local len_jitter2 = GetBDLen(jitter2)
local len_bh = GetBDLen(finalbh)

local line = GetLine(startpt,endpt)
local dis = GetFarDis2Line(finalbh,line)

if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end
     if(dis > loose_dis_max_threshold) then
         return false
     end
     for i = startindex, endindex, 5 do
        if (i + 5 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+5]
            if (curPt.x > nextPt.x) then
                return false
        end

        if (curPt.y < nextPt.y)then
            return false
        end
        end
    end
end


if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end

     if(dis > tight_dis_max_threshold) then
         return false
     end

    for i = startindex, endindex, 2 do
        if (i + 2 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+2]
            if (curPt.x < nextPt.x) then
              return false
        end

        if (curPt.y < nextPt.y)then
            return false
        end
        end
    end
end
return true

end


function  IsZuoDian(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_dis_max_threshold =40      --松评判时笔迹到笔画连线的最大距离
local tight_dis_max_threshold = 20       --紧评判时笔迹到笔画连线的最大距离
local loose_angel_max_threshold = 80    --首尾点连线偏离竖直方向的角度
local tight_angel_max_threshold = 60



local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (endindex < startindex) then
    return false
end

if (startpt.x <= endpt.x ) then
    return false
end

local line = GetLine(startpt,endpt)
local dis = GetFarDis2Line(bh,line)



local angel = 90
if ( endpt.y ~= startpt.y ) then
    local slope = (endpt.x -  startpt.x)/(endpt.y- startpt.y)
    angel = math.deg(math.atan(slope))
end
angel = math.abs(angel)
trace(angel)


if (bl == 1) then
     if(dis > loose_dis_max_threshold) then
         return false
     end
     if(angel > loose_angel_max_threshold)then
        return false
     end
end


if (bl == 2) then

     if(dis > tight_dis_max_threshold) then
         return false
     end
     if(angel > tight_angel_max_threshold) then
         return false
     end

end
return true
end


function  IsWoGou(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_angel_max_threshold = 70        --spt与tpt连线偏离竖直方向的角度
local loose_angel_min_threshold = 10
local tight_angel_max_threshold = 60
local tight_angel_min_threshold = 20

local startpt,startindex = GetTopMostPoint(bh)
local endpt= GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetBottomMostPoint(bh)
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index


local angel = 90
if (turning_pt.y ~= startpt.y) then
     local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
     angel = math.deg(math.atan(slope))
end
angel = math.abs(angel)

local bd1 = GetTempBD(bh,turning_index,endindex)
local len_bd1 = GetBDLen(bd1)
if (len_bd1 < 5) then
    return false
end

if (endindex < startindex) then
    return false
end

if (startpt.x >= endpt.x ) then
    return false
end

if (turning_pt.x < startpt.x) then
    return false
end


if (bl == 1) then
    if (angel > loose_angel_max_threshold or angel < loose_angel_min_threshold) then
            return false
    end


    for i = startindex, turning_index, 20 do
        if (i + 20 < turning_index ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+20]
            if (curPt.x > nextPt.x) then
              return false
            end

     end
    end
end


if (bl == 2) then
    if (angel > tight_angel_max_threshold or angel < tight_angel_min_threshold) then
            return false
    end

    for i = startindex, turning_index, 10 do
        if (i + 10 < turning_index ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+10]
            if (curPt.x < nextPt.x) then
                return false
        end
     end
   end
end
return true
end


function  IsWanGou(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local bd0_loose_dis_max_threshold = 150      --松评判时笔迹到拐点0连线的最大距离
local bd0_tight_dis_max_threshold = 80
local bd1_loose_dis_max_threshold = 100
local bd1_tight_dis_max_threshold = 50 
local loose_dis_min_threshold = 1
local loose_angel_max_threshold = 45    --松评判时首与拐点0连线倾斜的最大角度
local loose_jitter_max_threshold = 1/5    --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local tight_dis_max_threshold = 30      --紧评判时笔迹到首尾点连线的最大距离
local tight_dis_min_threshold = 2

local tight_angel_max_threshold = 30    --紧评判时首尾点连线倾斜的最大角度
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 85      --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 10      --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 70      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 15      --紧评判时两笔段夹角的最小值
local bd1_0_lenratio_threshold = 1/2            --笔段1和笔段2的长度比例


local startpt,startindex = GetTopMostPoint(bh)
local endpt =  GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetBottomMostPoint(bh)
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

if (endindex < startindex) then
return false
end

if (turning_pt.x < endpt.x) then
return false
end

 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local len_jitter1 = GetBDLen(jitter1)
 local len_bh = GetBDLen(finalbh)

 local line = GetLine(startpt,endpt)
 local dis = GetFarDis2Line(finalbh,line)

 local bd0 = GetTempBD(bh,startindex,turning_index)
 local bd1 = GetTempBD(bh,turning_index,endindex)
 local line0 = GetLine(startpt,turning_pt)
 local dis0 = GetFarDis2Line(bd0,line0)
 local line1 = GetLine(turning_pt,endpt)
 local dis1 =  GetFarDis2Line(bd1,line1)
 local len_bd0 = GetBDLen(bd0)
 local len_bd1 = GetBDLen(bd1)
 if (len_bd1 < 5) then
 return false
 end



local bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)
 local angel = 90
 if (turning_pt.y ~= startpt.y) then
 local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
 angel = math.deg(math.atan(slope))
 end
 angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold ) then
         return false
     end
     if (dis0 > bd0_loose_dis_max_threshold or dis1 > bd1_loose_dis_max_threshold) then
         return false
     end

     if(dis0 < loose_dis_min_threshold) then
         return false
     end

     if (angel > loose_angel_max_threshold) then
         return false
     end
     if(bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold) then
         return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis0 > bd0_tight_dis_max_threshold or dis1 > bd1_tight_dis_max_threshold) then
         return false
     end

     if(dis0 < tight_dis_min_threshold) then
         return false
     end

     if (angel > tight_angel_max_threshold) then
         return false
     end
      if(bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold) then
         return false
     end
     if(len_bd1 /len_bd0 > bd1_0_lenratio_threshold) then
         return false
     end
 end

return true

end



function  IsNa(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

if (endindex < startindex) then
    return false
end

if (startpt.x >= endpt.x ) then
    return false
end


local finalbh = GetTempBD(bh,startindex,endindex)
local jitter1 = GetTempBD(bh,1,startindex)
local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

local len_jitter1 = GetBDLen(jitter1)
local len_jitter2 = GetBDLen(jitter2)
local len_bh = GetBDLen(finalbh)

if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end

    for i = startindex, endindex, 20 do
        if (i + 20 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+20]
            if (curPt.x > nextPt.x) then
              return false
            end

        if (curPt.y > nextPt.y)then
            return false
        end
        end
    end
end


if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end

    for i = startindex, endindex, 10 do
        if (i + 10 < endindex ) then
            local curPt = bh.ptSet[i]
            local nextPt = bh.ptSet[i+10]
            if (curPt.x > nextPt.x) then
                return false
        end

        if (curPt.y > nextPt.y)then
                return false
        end
        end
    end
end
return true

end


function  IsPieZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local bd0_loose_angel_max_threshold = 80    --松评判时笔段0偏移竖直方向最大的角度
local bd0_loose_angel_min_threshold = 10    --松评判时笔段0偏移竖直方向最小的角度 [bd0_angel_min,bd0_ngel_max]

local bd0_tight_angel_max_threshold = 70     --紧评判时bd0偏离水平方向的最大角度
local bd0_tight_angel_min_threshold = 20     --紧评判时bd0偏离水平方向的最小角度

local bd1_loose_angel_max_threshold = 45     --松评判时笔段1偏移水平方向最大的角度
local bd1_tight_angel_max_threshold = 10     --紧评判时bd1偏离水平方向的最大的角度

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离
local loose_angel_max_threshold = 60    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 45    --紧评判时首尾点连线倾斜的最大角度

local bd0_1_loose_angel_max_threshold = 85  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =30    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 70  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =40    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 3    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/3  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 2  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/2  --紧评判时两笔段长度比例最小值

local startpt = GetStartPoint(bh)
local endpt = GetEndPoint(bh)

if (startpt.y > endpt.y ) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (turning_pt.x > startpt.x or turning_pt.x > endpt.x or turning_pt.y < startpt.y ) then
return false
end
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index



if (endpt.y - turning_pt.y > 3) then
return false
end

local bd0 = GetTempBD(bh,1,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local len_bd0 = GetBDLen(bd0)

local bd1 = GetTempBD(bh,turning_index,#bh.ptSet)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local angel0 = 90
if (turning_pt.y ~= startpt.y) then
local slope0 = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.x ~= turning_pt.x) then
local slope1 = (endpt.y - turning_pt.y) / (endpt.x - turning_pt.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold  or angel1 > bd1_loose_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_loose_angel_min_threshold  ) then
        return false
    end

    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold  or angel1 > bd1_tight_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_tight_angel_min_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true

end


function  IsPieTi(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local bd0_loose_angel_max_threshold = 100    --松评判时笔段0偏移竖直方向最大的角度
local bd0_loose_angel_min_threshold = 10    --松评判时笔段0偏移竖直方向最小的角度 [bd0_angel_min,bd0_ngel_max]

local bd0_tight_angel_max_threshold = 80     --紧评判时bd0偏离水平方向的最大角度
local bd0_tight_angel_min_threshold = 10     --紧评判时bd0偏离水平方向的最小角度


local bd1_loose_angel_min_threshold = 3     --松评判时笔段1偏移水平方向最大的角度
local bd1_tight_angel_min_threshold = 5     --紧评判时bd1偏离水平方向的最小的角度

local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离
local loose_angel_max_threshold = 60    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 45    --紧评判时首尾点连线倾斜的最大角度


local bd0_1_loose_angel_max_threshold = 85  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 10    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 20    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local startpt = GetStartPoint(bh)
local endpt = GetEndPoint(bh)

if (startpt.y > endpt.y ) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (turning_pt.x > startpt.x or turning_pt.x > endpt.x or turning_pt.y < startpt.y ) then
return false
end

if (endpt.y - turning_pt.y > 3) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
local bd0 = GetTempBD(bh,1,turning_index)
local line0 = GetLine(startpt,turning_pt)
local len_bd0 = GetBDLen(bd0)

local bd1 = GetTempBD(bh,turning_index,#bh.ptSet)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local angel0 = 90
if (turning_pt.y ~= startpt.y) then
local slope0 = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.x ~= turning_pt.x) then
local slope1 = (endpt.y - turning_pt.y) / (endpt.x - turning_pt.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if ( dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold) then
        return false
    end

    if (angel0 < bd0_loose_angel_min_threshold or angel1 < bd1_loose_angel_min_threshold ) then
        return false
    end

    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_tight_angel_min_threshold or angel1 < bd1_tight_angel_min_threshold ) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
 end
return true
end


function  IsPieDian(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度


local bd0_1_loose_angel_max_threshold = 150   --紧评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 60    --紧评判时两笔段夹角的最小值

local bd0_1_tight_angel_max_threshold = 120   --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 45    --紧评判时两笔段夹角的最小值

local bd0_1_lenratio_max_threshold = 3    --紧评判时两笔段长度比例最大值
local bd0_1_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)
local spt_ept_line = GetLine(startpt,endpt)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if(turning_pt.x > startpt.x or turning_pt.x > endpt.x) then
    return false
end

if (startindex > endindex) then
    return false
end
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local len_bd0 = GetBDLen(bd0)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)
if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true

end


function  IsHengZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_1_loose_angel_max_threshold = 120  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =50    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 100  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineUp(turning_pt,spt_ept_line) == false) then
return false
end


bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.x ~= startpt.x) then
local slope0 = (turning_pt.y -  startpt.y)/(turning_pt.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.y ~= turning_pt.y) then
local slope1 = (endpt.x - turning_pt.x) / (endpt.y - turning_pt.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true
end


function  IsHengZheWan(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 120      --松评判时两笔段夹角的最大值(bd0-1和bd1-2阈值相同)
local bd0_1_loose_angel_min_threshold =50        --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 100      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60        --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5     --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5   --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3     --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3   --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet

if (startindex > endindex) then
return false
end

local vpt = {}
vpt.x = endpt.x
vpt.y = startpt.y
local turning_index_0 = 1
local temp = 90
for i = 1, #bh.ptSet do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == endpt.y) then
	angel = 90
    else
	angel = Cal_Angle (cpt,endpt,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_0  = i
    end
end

local turning_pt_0  = bh.ptSet[turning_index_0 ]
local bd0 = GetTempBD(bh,startindex,turning_index_0 )
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


vpt.x = startpt.x
vpt.y = endpt.y
temp = 90
local turning_index_1 = 1
for i = 1,#bh.ptSet do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == startpt.y) then
        angel = 90
    else
        angel = Cal_Angle(cpt,startpt,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_1= i
    end
end

local turning_pt_1 = bh.ptSet[turning_index_1]
local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)


bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

if(startpt.x > turning_pt_0.x or turning_pt_0.y > turning_pt_1.y or turning_pt_1.x > endpt.x ) then
return false
end

local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local angel2 = 90
if (endpt.x ~= turning_pt_1.x) then
local slope2 = (endpt.y -  turning_pt_1.y)/(endpt.x - turning_pt_1.x)
angel2 = math.deg(math.atan(slope2))
end
angel2 = math.abs(angel2)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)
local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold ) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (len_bd1 / len_bd2 > bd0_1_loose_lenratio_max_threshold  or len_bd1/len_bd2 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd0_1_loose_angel_max_threshold or bd1_bd2_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold ) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (len_bd1 / len_bd2 > bd0_1_tight_lenratio_max_threshold  or len_bd1/len_bd2 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd0_1_tight_angel_max_threshold or bd1_bd2_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true
end


function  IsHengZheZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 120      --松评判时两笔段夹角的最大值(bd0-1和bd1-2阈值相同)
local bd0_1_loose_angel_min_threshold = 50        --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 100      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60        --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5     --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5   --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3     --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3   --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet

if (startindex > endindex) then
return false
end

local vpt = {}
vpt.x = endpt.x
vpt.y = startpt.y
local turning_index_0 = 1
local temp = 90
for i = 1, #bh.ptSet do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == endpt.y) then
	angel = 90
    else
	angel = Cal_Angle (cpt,endpt,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_0  = i
    end
end

local turning_pt_0  = bh.ptSet[turning_index_0 ]
local bd0 = GetTempBD(bh,startindex,turning_index_0 )
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


vpt.x = startpt.x
vpt.y = endpt.y
temp = 90
local turning_index_1 = 1
for i = 1,#bh.ptSet do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == startpt.y) then
        angel = 90
    else
        angel = Cal_Angle(cpt,startpt,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_1= i
    end
end

local turning_pt_1 = bh.ptSet[turning_index_1]
local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

if(startpt.x > turning_pt_0.x or turning_pt_0.y > turning_pt_1.y or turning_pt_1.x > endpt.x ) then
return false
end

local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local angel2 = 90
if (endpt.x ~= turning_pt_1.x) then
local slope2 = (endpt.y -  turning_pt_1.y)/(endpt.x - turning_pt_1.x)
angel2 = math.deg(math.atan(slope2))
end
angel2 = math.abs(angel2)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)
local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold or angel2 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (len_bd1 / len_bd2 > bd0_1_loose_lenratio_max_threshold  or len_bd1/len_bd2 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd0_1_loose_angel_max_threshold or bd1_bd2_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold or angel2 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (len_bd1 / len_bd2 > bd0_1_tight_lenratio_max_threshold  or len_bd1/len_bd2 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd0_1_tight_angel_max_threshold or bd1_bd2_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true
end


function  IsHengZheTi(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_1_loose_angel_max_threshold = 120  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =50    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 100  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60    --紧评判时两笔段夹角的最小值

local bd1_2_loose_angel_max_threshold = 80  --松评判时两笔段夹角的最大值
local bd1_2_loose_angel_min_threshold = 10    --松评判时两笔段夹角的最小值
local bd1_2_tight_angel_max_threshold = 70  --紧评判时两笔段夹角的最大值
local bd1_2_tight_angel_min_threshold = 20    --紧评判时两笔段夹角的最小值

local bd0_1_loose_lenratio_max_threshold = 2    --松评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_max_threshold = 2  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/5  --紧评判时两笔段长度比例最小值


local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)


local spt_tpt1_line = GetLine(startpt,turning_pt_1)
local spt_tpt1_bd = GetTempBD(bh,1,turning_index_1)
local turning_pt_0,turning_index_0 = GetFarthestPt2Line(spt_tpt1_bd,spt_tpt1_line)

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1
local bd0 = GetTempBD(bh,startindex,turning_index_0)
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)



local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)


if ( endpt.x < turning_pt_1.x ) then
return false
end



local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle(turning_pt_0,turning_pt_1,endpt)





if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold)  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

    if (bd1_bd2_angel > bd1_2_loose_angel_max_threshold or bd1_bd2_angel < bd1_2_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end

return true
end



function  IsHengZheWanGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 100      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 45    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 30     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 120      --松评判时两笔段夹角的最大值(bd0-1和bd1-2阈值相同)
local bd0_1_loose_angel_min_threshold = 30        --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 100      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 40        --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 10     --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/10   --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 5     --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/5   --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet

if (startindex > endindex) then
return false
end

--先找到最后一个拐点
local tempdis = 512
local turning_index_2 = 1
local edget = {}
edget.x = 512
edget.y = 512

for i = 1, #bh.ptSet do
    local curpt = bh.ptSet[i]
    local curdis =  GetDistance(curpt,edget)
    if (curdis < tempdis) then
         tempdis = curdis
         turning_index_2 = i
    end
end
local turning_pt_2 = bh.ptSet[turning_index_2]

local bd3 = GetTempBD(bh,turning_index_2,endindex)
local line3 = GetLine(turning_pt_2,endpt)
local dis3 = GetFarDis2Line(bd3,line3)
local len_bd3 = GetBDLen(bd3)

local vpt = {}
vpt.x = endpt.x
vpt.y = startpt.y
local turning_index_0 = 1
local temp = 90
for i = 1, turning_index_2 do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == turning_pt_2.y) then
	angel = 90
    else
	angel = Cal_Angle (cpt,turning_pt_2,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_0  = i
    end
end


local turning_pt_0  = bh.ptSet[turning_index_0 ]
local bd0 = GetTempBD(bh,startindex,turning_index_0 )
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local len_bd0 = GetBDLen(bd0)




vpt.x = startpt.x
vpt.y = endpt.y
temp = 90
local turning_index_1 = 1
for i = 1,turning_index_2 do
    local cpt = bh.ptSet[i]
    local angel = 0
    if (cpt.y == startpt.y) then
        angel = 90
    else
        angel = Cal_Angle(cpt,startpt,vpt)
    end
    if (angel < temp) then
        temp = angel
        turning_index_1= i
    end
end



local turning_pt_1 = bh.ptSet[turning_index_1]
local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)



local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
local line2 = GetLine(turning_pt_1,turning_pt_2)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)

if(startpt.x > turning_pt_0.x or turning_pt_0.y > turning_pt_1.y or turning_pt_1.x > endpt.x ) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_2

local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local angel2 = 90
if (endpt.x ~= turning_pt_1.x) then
local slope2 = (endpt.y -  turning_pt_1.y)/(endpt.x - turning_pt_1.x)
angel2 = math.deg(math.atan(slope2))
end
angel2 = math.abs(angel2)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)
local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold ) then
       return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd0_1_loose_angel_max_threshold or bd1_bd2_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
	if (len_bd3 < 3) then
		return false
	end

    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
		  return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold ) then
		  return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
		  return false
    end
	if (len_bd1 / len_bd2 > bd0_1_tight_lenratio_max_threshold  or len_bd1/len_bd2 <bd0_1_tight_lenratio_min_threshold ) then
		  return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then

		  return false
    end
    if (bd1_bd2_angel > bd0_1_tight_angel_max_threshold or bd1_bd2_angel < bd0_1_tight_angel_min_threshold ) then
		  return false
    end

end
return true

end


function  IsShuWan(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_1_loose_angel_max_threshold = 130  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =60    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 120  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =70    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineDown(turning_pt,spt_ept_line) == false) then
return false
end
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.y ~= startpt.y) then
local slope0 = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.x ~= turning_pt.x) then
local slope1 = (endpt.y - turning_pt.y) / (endpt.x - turning_pt.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)
if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
end
return true
end


function  IsShuZheZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 130  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =45    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 120  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =70    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local bd1_2_loose_angel_max_threshold = 130  --松评判时两笔段夹角的最大值
local bd1_2_loose_angel_min_threshold = 60    --松评判时两笔段夹角的最小值
local bd1_2_tight_angel_max_threshold = 120  --紧评判时两笔段夹角的最大值
local bd1_2_tight_angel_min_threshold = 70    --紧评判时两笔段夹角的最小值


local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (startpt.y > endpt.y ) then
return false
end

local vpt0 = {}
vpt0.x = 512
vpt0.y = 0


local vpt1 = {}
vpt1.x = 0
vpt1.y = 512


local turning_index0 = 1
local minDis = 512
for i = startindex,#bh.ptSet do
    local curpt = bh.ptSet[i]
    local tempDis = GetDistance(curpt,vpt0)
    if (tempDis < minDis ) then
        turning_index_0 = i
        minDis  = tempDis
    end
end

local turning_index_1 = 1
minDis = 512
for i = startindex,#bh.ptSet do
    local curpt = bh.ptSet[i]
    local tempDis = GetDistance(curpt,vpt1)
    if (tempDis < minDis ) then
        turning_index_1 = i
        minDis  = tempDis
    end
end


local turning_pt_0 = bh.ptSet[turning_index_0]
local turning_pt_1 = bh.ptSet[turning_index_1]


local bd0 = GetTempBD(bh,startindex,turning_index_0)
local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local bd2 = GetTempBD(bh,turning_index_1,endindex)

local line0 = GetLine(startpt,turning_pt_0)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local line2 = GetLine(turning_pt_1,endpt)



local dis0 = GetFarDis2Line(bd0,line0)
local dis1 = GetFarDis2Line(bd1,line1)
local dis2 = GetFarDis2Line(bd2,line2)

local len_bd0 = GetBDLen(bd0)
local len_bd1 = GetBDLen(bd1)
local len_bd2 = GetBDLen(bd2)
if (turning_pt_0.x > endpt.x or turning_pt_0.y < startpt.y ) then
return false
end


local angel0 = 90
if (turning_pt_0.y ~= startpt.y) then
local slope0 = (turning_pt_0.x -  startpt.x)/(turning_pt_0.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.x ~= turning_pt_0.x) then
local slope1 = (turning_pt_1.y - turning_pt_0.y) / (turning_pt_1.x - turning_pt_0.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local angel2 = 90
if (turning_pt_1.y ~= endpt.y) then
local slope2 = (endpt.x -  turning_pt_1.x)/(endpt.y - turning_pt_1.y)
angel0 = math.deg(math.atan(slope2))
end
angel2 = math.abs(angel2)

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold or angel2 > loose_angel_max_threshold) then
        return false
    end



    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

   if (bd1_bd2_angel > bd1_2_loose_angel_max_threshold or bd1_bd2_angel < bd1_2_loose_angel_min_threshold ) then
        return false
    end

end
if (bl == 2) then
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold  or angel1 > bd1_tight_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_tight_angel_min_threshold ) then
        return false
    end


    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

   if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end
return true
end


function  IsShuZheZheGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local bd0_loose_angel_max_threshold = 30    --松评判时笔段0偏移竖直方向最大的角度
local bd0_tight_angel_max_threshold = 15     --紧评判时bd0偏离水平方向的最大角度

local bd1_loose_angel_max_threshold = 40     --松评判时笔段1偏移水平方向最大的角度
local bd1_tight_angel_max_threshold = 20     --紧评判时bd1偏离水平方向的最大的角度

local bd2_loose_angel_max_threshold = 30    --bd2偏离水平方向的最大角度
local bd2_tight_angel_max_threshold = 15

local loose_dis_max_threshold = 40          --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20

local bd0_1_loose_angel_max_threshold = 120  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =45   --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 90  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =60   --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3    --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local bd1_2_loose_angel_max_threshold = 120    --松评判时两笔段夹角的最大值
local bd1_2_loose_angel_min_threshold = 30    --松评判时两笔段夹角的最小值
local bd1_2_tight_angel_max_threshold = 90    --紧评判时两笔段夹角的最大值
local bd1_2_tight_angel_min_threshold = 40    --紧评判时两笔段夹角的最小值


local startpt,startindex = GetTopMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt_2,turning_index_2 = GetBottomMostPoint(bh)
local turning_pt_0,turning_index_0 = GetLeftMostPoint(bh)
local turning_pt_1,turning_index_1 = GetRightMostPoint(bh)

local bd0 = GetTempBD(bh,startindex,turning_index_0)
local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
local bd3 = GetTempBD(bh,turning_index_2,endindex)


local line0 = GetLine(startpt,turning_pt_0)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local line2 = GetLine(turning_pt_1,turning_pt_2)
local line3 = GetLine(turning_pt_2,endpt)

local dis0 = GetFarDis2Line(bd0,line0)
local dis1 = GetFarDis2Line(bd1,line1)
local dis2 = GetFarDis2Line(bd2,line2)
local dis3 = GetFarDis2Line(bd3,line3)

local len_bd0 = GetBDLen(bd0)
local len_bd1 = GetBDLen(bd1)
local len_bd2 = GetBDLen(bd2)
local len_bd3 = GetBDLen(bd3)

local angel0 = 90
if (turning_pt_0.y ~= startpt.y) then
local slope0 = (turning_pt_0.x -  startpt.x)/(turning_pt_0.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.x ~= turning_pt_0.x) then
local slope1 = (turning_pt_1.y - turning_pt_0.y) / (turning_pt_1.x - turning_pt_0.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)

local angel2 = 90
if (turning_pt_2.y ~= turning_pt_1.y) then
local slope2 = (turning_pt_0.x -  startpt.x)/(turning_pt_0.y - startpt.y)
angel2 = math.deg(math.atan(slope2))
end
angel2 = math.abs(angel0)

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_2




if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold or dis3 > loose_dis_max_threshold ) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold  or angel1 > bd1_loose_angel_max_threshold or angel2 > bd2_loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

   if (bd1_bd2_angel > bd1_2_loose_angel_max_threshold or bd1_bd2_angel < bd1_2_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold or dis3 > tight_dis_max_threshold ) then
        return false
    end

    if (angel0 > bd0_tight_angel_max_threshold  or angel1 > bd1_tight_angel_max_threshold or angel2 > bd2_tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

    if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end
return true
end


function  IsShuZhePie(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local bd0_loose_angel_max_threshold = 80    --松评判时笔段0偏移竖直方向最大的角度
local bd0_loose_angel_min_threshold = 10    --松评判时笔段0偏移竖直方向最小的角度 [bd0_angel_min,bd0_ngel_max]

local bd0_tight_angel_max_threshold = 60     --紧评判时bd0偏离水平方向的最大角度
local bd0_tight_angel_min_threshold = 10     --紧评判时bd0偏离水平方向的最小角度

local bd1_loose_angel_max_threshold = 40     --松评判时笔段1偏移水平方向最大的角度
local bd1_tight_angel_max_threshold = 20     --紧评判时bd1偏离水平方向的最大的角度

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15    --紧评判时首尾点连线倾斜的最大角度

local bd0_1_loose_angel_max_threshold = 85  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold =30    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 70  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold =40    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 5    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/5  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 3  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/3  --紧评判时两笔段长度比例最小值

local bd1_2_loose_angel_max_threshold = 80  --松评判时两笔段夹角的最大值
local bd1_2_loose_angel_min_threshold = 20    --松评判时两笔段夹角的最小值
local bd1_2_tight_angel_max_threshold = 70  --紧评判时两笔段夹角的最大值
local bd1_2_tight_angel_min_threshold = 30    --紧评判时两笔段夹角的最小值

local startpt = GetStartPoint(bh)
local startindex = 1
local endpt,endindex = GetBottomMostPoint(bh)

if (startpt.y > endpt.y ) then
return false
end

local turning_pt_1,turning_index_1 = GetRightMostPoint(bh)
local bd2 = GetTempBD(bh,turning_index_1, endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)


local turning_pt_0,turning_index_0 = GetLeftMostPoint(bh)
if (turning_pt_0.x > startpt.x or turning_pt_0.x > endpt.x or turning_pt_0.y < startpt.y ) then
    return false
end

if (turning_pt_1.x < endpt.x) then
    return false
end


local bd0 = GetTempBD(bh,1,turning_index_0)
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local len_bd0 = GetBDLen(bd0)



local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local angel0 = 90
if (turning_pt_0.y ~= startpt.y) then
local slope0 = (turning_pt_0.x -  startpt.x)/(turning_pt_0.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)

local angel1 = 90
if (turning_pt_1.x ~= turning_pt_0.x) then
local slope1 = (turning_pt_1.y - turning_pt_0.y) / (turning_pt_1.x - turning_pt_0.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle (turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold  or angel1 > bd1_loose_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_loose_angel_min_threshold ) then
        return false
    end


    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end

   if (bd1_bd2_angel > bd1_2_loose_angel_max_threshold or bd1_bd2_angel < bd1_2_loose_angel_min_threshold ) then
        return false
    end

end

if (bl == 2) then
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold  or angel1 > bd1_tight_angel_max_threshold ) then
        return false
    end

    if (angel0 < bd0_tight_angel_min_threshold ) then
        return false
    end


    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

   if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end
return true
end

function  IsShuTi(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_dis_max_threshold = 40     --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 30    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 15    --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 85      --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 10      --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 70      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 15      --紧评判时两笔段夹角的最小值
local bd1_0_lenratio_threshold = 1/2            --笔段1和笔段2的长度比例


local startpt,startindex = GetTopMostPoint(bh)
local endpt =  GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetBottomMostPoint(bh)

if (endindex < startindex) then
return false
end

if (turning_pt.x > endpt.x) then
return false
end


 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local len_jitter1 = GetBDLen(jitter1)
 local len_bh = GetBDLen(finalbh)

 local line = GetLine(startpt,endpt)
 local dis = GetFarDis2Line(finalbh,line)

 local bd0 = GetTempBD(bh,startindex,turning_index)
 local bd1 = GetTempBD(bh,turning_index,endindex)
 local line0 = GetLine(startpt,turning_pt)
 local dis0 = GetFarDis2Line(bd0,line0)
 local line1 = GetLine(turning_pt,endpt)
 local dis1 =  GetFarDis2Line(bd1,line1)


bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
 local len_bd0 = GetBDLen(bd0)
 local len_bd1 = GetBDLen(bd1)
 if (len_bd1 < 5) then
 return false
 end



local bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)
 local angel = 90
 if (turning_pt.y ~= startpt.y) then
 local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
 angel = math.deg(math.atan(slope))
 end
 angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold ) then
         return false
     end
     if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
         return false
     end
     if (angel > loose_angel_max_threshold) then
         return false
     end
     if(bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold) then
         return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
         return false
     end
     if (angel > tight_angel_max_threshold) then
         return false
     end
      if(bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold) then
         return false
     end
     if(len_bd1 /len_bd0 > bd1_0_lenratio_threshold) then
         return false
     end
 end
 return true
end


function  IsShuGou(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end
local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 45    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 30    --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_tight_angel_max_threshold = 70      --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 15      --紧评判时两笔段夹角的最小值
local bd1_0_lenratio_threshold = 1/2            --笔段1和笔段2的长度比例


local startpt,startindex = GetTopMostPoint(bh)
local endpt =  GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt,turning_index = GetBottomMostPoint(bh)

if (endindex < startindex) then
return false
end

if (turning_pt.x < endpt.x) then
return false
end


 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local len_jitter1 = GetBDLen(jitter1)
 local len_bh = GetBDLen(finalbh)

 local line = GetLine(startpt,endpt)
 local dis = GetFarDis2Line(finalbh,line)

 local bd0 = GetTempBD(bh,startindex,turning_index)
 local bd1 = GetTempBD(bh,turning_index,endindex)
 local line0 = GetLine(startpt,turning_pt)
 local dis0 = GetFarDis2Line(bd0,line0)
 local line1 = GetLine(turning_pt,endpt)
 local dis1 =  GetFarDis2Line(bd1,line1)
 bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index


 local len_bd0 = GetBDLen(bd0)
 local len_bd1 = GetBDLen(bd1)
 if (len_bd1 < 5) then
 return false
 end



local bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)
 local angel = 90
 if (turning_pt.y ~= startpt.y) then
 local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
 angel = math.deg(math.atan(slope))
 end
 angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold ) then
         return false
     end
     if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
         return false
     end
     if (angel > loose_angel_max_threshold) then
         return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
         return false
     end
     if (angel > tight_angel_max_threshold) then
         return false
     end
      if(bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold) then
         return false
     end
     if(len_bd1 /len_bd0 > bd1_0_lenratio_threshold) then
         return false
     end
 end
 return true
end


function  IsHengZheGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 80
local tight_dis_max_threshold = 50
local loose_angel_max_threshold = 45
local tight_angel_max_threshold = 30

local loose_jitter_max_threshold = 1/5
local tight_jitter_max_threshold = 1/10

local bd0_1_loose_angel_max_threshold = 120
local bd0_1_loose_angel_min_threshold = 30
local bd0_1_tight_angel_max_threshold = 100
local bd0_1_tight_angel_min_threshold = 45

local bd1_2_loose_angel_max_threshold = 80
local bd1_2_loose_angel_min_threshold = 10
local bd1_2_tight_angel_max_threshold = 70
local bd1_2_tight_angel_min_threshold = 20

local bd0_1_loose_lenratio_max_threshold = 3
local bd0_1_tight_lenratio_max_threshold = 2
local bd0_1_tight_lenratio_min_threshold = 1/3


local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

local spt_tpt1_line = GetLine(startpt,turning_pt_1)
local spt_tpt1_bd = GetTempBD(bh,1,turning_index_1)
local turning_pt_0,turning_index_0 = GetFarthestPt2Line(spt_tpt1_bd,spt_tpt1_line)

local bd0 = GetTempBD(bh,startindex,turning_index_0)
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)

local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)

local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)

if ( endpt.x > turning_pt_1.x ) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle(turning_pt_0,turning_pt_1,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold)  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > loose_angel_max_threshold or angel1 > loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > tight_angel_max_threshold or angel1 > tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end
return true
end


function  IsHengPie(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local bd0_loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local bd0_tight_dis_max_threshold = 50

local bd0_loose_angel_max_threshold = 60     --松评判时bd0首尾点连线倾斜的最大角度
local bd0_tight_angel_max_threshold = 45     --紧评判时bd0首尾点连线倾斜的最大角度


local bd1_loose_angel_max_threshold = 80    --松评判时bd1首尾点连线倾斜的最大角度
local bd1_loose_angel_min_threshold = 10    --松评判时bd1首尾点连线倾斜的最小角度
local bd1_tight_angel_max_threshold = 70    --紧评判时bd1首尾点连线倾斜的最大角度
local bd1_tight_angel_min_threshold = 20    --紧评判时bd1首尾点连线倾斜的最小角度
local bd1_loose_dis_max_threshold = 100      --松评判时bd1路径上到bd1首尾点连线的最大距离
local bd1_loose_dis_min_threshold = 3       --松评判时bd1路径上到bd1首尾点连线的最小距离
local bd1_tight_dis_max_threshold = 50      --紧评判时bd1路径上到bd1首尾点连线的最大距离
local bd1_tight_dis_min_threshold = 5       --紧评判时bd1路径上到bd1首尾点连线的最小距离

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例

local bd0_1_loose_angel_max_threshold = 90  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 20    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 20    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 3    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/3  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 2  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/2  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetLeftMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineUp(turning_pt,spt_ept_line) == false) then
return false
end

if (turning_pt.x < endpt.x) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index
local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.x ~= startpt.x) then
local slope0 = (turning_pt.y -  startpt.y)/(turning_pt.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.y ~= turning_pt.y) then
local slope1 = (endpt.x - turning_pt.x) / (endpt.y - turning_pt.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then
        return false
    end
    if (dis0 > bd0_loose_dis_max_threshold or dis1 > bd1_loose_dis_max_threshold or dis1 <bd1_loose_dis_min_threshold) then
         return false
    end
    if (angel0 > bd0_loose_angel_max_threshold or angel1 > bd1_loose_angel_max_threshold or angel1 < bd1_loose_angel_min_threshold) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > bd1_tight_dis_max_threshold or dis1 < bd1_tight_dis_min_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold or angel1 > bd1_tight_angel_max_threshold or angel1 < bd1_tight_angel_min_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
end

return true
end


function  IsXieShu(bh,bl)
if(bl ~= 1 and bl~=2 ) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local loose_angel_max_threshold = 60    --松评判时首尾点连线倾斜的最大角度
local tight_angel_max_threshold = 30    --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetBottomMostPoint(bh)

if (endindex < startindex) then
return false
end

 local finalbh = GetTempBD(bh,startindex,endindex)
 local jitter1 = GetTempBD(bh,1,startindex)
 local jitter2 = GetTempBD(bh,endindex,#bh.ptSet)

 local len_jitter1 = GetBDLen(jitter1)
 local len_jitter2 = GetBDLen(jitter2)
 local len_bh = GetBDLen(finalbh)

 local line = GetLine(startpt,endpt)
 local dis = GetFarDis2Line(finalbh,line)

 local angel = 90
 if (endpt.y ~= startpt.y) then
 local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
 angel = math.deg(math.atan(slope))
 end
 angel = math.abs(angel)

 if (bl == 1) then
     if (len_jitter1/len_bh > loose_jitter_max_threshold or len_jitter2/len_bh > loose_jitter_max_threshold) then
         return false
     end
     if (dis > loose_dis_max_threshold) then
         return false
     end
     if (angel > loose_angel_max_threshold) then
         return false
     end
 end

 if (bl == 2) then
     if (len_jitter1/len_bh > tight_jitter_max_threshold or len_jitter2/len_bh > tight_jitter_max_threshold) then
         return false
     end
     if (dis > tight_dis_max_threshold) then
         return false
     end
     if (angel > tight_angel_max_threshold) then
         return false
     end
 end

 return true

end


function  IsXieShuZhe(bh,bl)
if (bl ~= 1 and bl~=2) then return end

local loose_dis_max_threshold = 40      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 20      --紧评判时笔迹到首尾点连线的最大距离

local bd1_loose_angel_max_threshold = 20    --松评判时首尾点连线倾斜的最大角度
local bd1_tight_angel_max_threshold = 10     --紧评判时首尾点连线倾斜的最大角度

local bd0_loose_angel_max_threshold = 60    --松评判时首尾点连线倾斜的最大角度
local bd0_tight_angel_max_threshold = 30     --紧评判时首尾点连线倾斜的最大角度

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_1_loose_angel_max_threshold = 90  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 30    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 45    --紧评判时两笔段夹角的最小值
local bd0_1_loose_lenratio_max_threshold = 3    --松评判时两笔段长度比例最大值
local bd0_1_loose_lenratio_min_threshold = 1/3  --松评判时两笔段长度比例最小值
local bd0_1_tight_lenratio_max_threshold = 2  --紧评判时两笔段长度比例最大值
local bd0_1_tight_lenratio_min_threshold = 1/2  --紧评判时两笔段长度比例最小值

local startpt,startindex = GetTopMostPoint(bh)
local endpt,endindex = GetRightMostPoint(bh)

if (startindex > endindex) then
return false
end

local spt_ept_line = GetLine(startpt,endpt)
local finalbh = GetTempBD(bh,startindex,endindex)
local turning_pt,turning_index = GetFarthestPt2Line(bh,spt_ept_line)
if (Point2LineDown(turning_pt,spt_ept_line) == false) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

local bd0 = GetTempBD(bh,startindex,turning_index)
local line0 = GetLine(startpt,turning_pt)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index,endindex)
local line1 = GetLine(turning_pt,endpt)
local dis1 = GetFarDis2Line(bd1,line1)
local bd1_jitter = GetTempBD(bh,endindex,#bh.ptSet)
local len_bd1 = GetBDLen(bd1)
local len_bd1_jitter = GetBDLen(bd1_jitter)

local angel0 = 90
if (turning_pt.y ~= startpt.y) then
local slope0 = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (endpt.x ~= turning_pt.x) then
local slope1 = (endpt.y - turning_pt.y) / (endpt.x - turning_pt.x)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt,endpt)

if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold or len_bd1_jitter/len_bd1 > loose_jitter_max_threshold )  then

        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold or angel1 > bd1_loose_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_loose_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_loose_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold or len_bd1_jitter/len_bd1 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold or angel1 > bd1_tight_angel_max_threshold) then
        return false
    end
    if (len_bd0 / len_bd1 > bd0_1_tight_lenratio_max_threshold  or len_bd0/len_bd1 <bd0_1_tight_lenratio_min_threshold ) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end

end
return true

end

function  IsXieWanGou(bh,bl)
if (bl ~= 1 and bl~=2) then return end
local loose_dis_max_threshold = 80      --松评判时笔迹到首尾点连线的最大距离
local tight_dis_max_threshold = 50      --紧评判时笔迹到首尾点连线的最大距离

local loose_jitter_max_threshold = 1/5  --松评判时抖笔笔段长度与去除抖笔之后笔画长度的比例
local tight_jitter_max_threshold = 1/10 --紧评判时抖笔笔段长度与去除抖笔之后笔画长度的比例


local bd0_loose_angel_max_threshold = 80    --松评判时bd0首尾点连线的最大倾斜角度
local bd0_loose_angel_min_threshold = 3     --松评判时bd0首尾点连线的最小倾斜角度
local bd0_tight_angel_max_threshold = 60    --紧评判时bd1首尾点连线的最大倾斜角度
local bd0_tight_angel_min_threshold = 5    --紧评判时bd1首尾点连线的最小倾斜角度

local bd1_loose_angel_max_threshold = 50    --松评判时bd1偏离竖直方向的最大倾斜角度
local bd1_tight_angel_max_threshold = 40    --紧评判时bd1偏离竖直方向的最大倾斜角度


local bd0_1_loose_angel_max_threshold = 90  --松评判时两笔段夹角的最大值
local bd0_1_loose_angel_min_threshold = 30    --松评判时两笔段夹角的最小值
local bd0_1_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd0_1_tight_angel_min_threshold = 40    --紧评判时两笔段夹角的最小值

--local bd1_2_loose_angel_max_threshold = 80  --松评判时两笔段夹角的最大值
--local bd1_2_loose_angel_min_threshold = 10    --松评判时两笔段夹角的最小值
local bd1_2_tight_angel_max_threshold = 80  --紧评判时两笔段夹角的最大值
local bd1_2_tight_angel_min_threshold = 10    --紧评判时两笔段夹角的最小值


local startpt,startindex = GetLeftMostPoint(bh)
local endpt = GetEndPoint(bh)
local endindex = #bh.ptSet
local turning_pt_0,turning_index_0 = GetTopMostPoint(bh)


local vpt1 = {}
vpt1.x = 512
vpt1.y = 512

local turning_index_1 = 1
minDis = 512
for i = 1,#bh.ptSet do
    local curpt = bh.ptSet[i]
    local tempDis = GetDistance(curpt,vpt1)
    if (tempDis < minDis ) then
        turning_index_1 = i
        minDis  = tempDis
    end
end
turning_pt_1 = bh.ptSet[turning_index1]
local bd0 = GetTempBD(bh,startindex,turning_index_0)
local line0 = GetLine(startpt,turning_pt_0)
local dis0 = GetFarDis2Line(bd0,line0)
local bd0_jitter = GetTempBD(bh,1,startindex)
local len_bd0 = GetBDLen(bd0)
local len_bd0_jitter = GetBDLen(bd0_jitter)


local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
local line1 = GetLine(turning_pt_0,turning_pt_1)
local dis1 = GetFarDis2Line(bd1,line1)
local len_bd1 = GetBDLen(bd1)



local bd2 = GetTempBD(bh,turning_index_1,endindex)
local line2 = GetLine(turning_pt_1,endpt)
local dis2 = GetFarDis2Line(bd2,line2)
local len_bd2 = GetBDLen(bd2)


if ( endpt.x > turning_pt_1.x ) then
return false
end

bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1


local angel0 = 90
if (turning_pt_0.x ~= startpt.x) then
local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
angel0 = math.deg(math.atan(slope0))
end
angel0 = math.abs(angel0)


local angel1 = 90
if (turning_pt_1.y ~= turning_pt_0.y) then
local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
angel1 = math.deg ( math.atan(slope1))
end
angel1 = math.abs(angel1)


local bd0_bd1_angel = 0
bd0_bd1_angel = Cal_Angle (startpt,turning_pt_0,turning_pt_1)


local bd1_bd2_angel = 0
bd1_bd2_angel = Cal_Angle(turning_pt_0,turning_pt_1,endpt)


if (bl == 1) then
    if (len_bd0_jitter/len_bd0 > loose_jitter_max_threshold)  then
        return false
    end
    if (dis0 > loose_dis_max_threshold or dis1 > loose_dis_max_threshold or dis2 > loose_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_loose_angel_max_threshold or angel0 < bd0_loose_angel_min_threshold or angel1 > bd1_loose_angel_max_threshold) then
        return false
    end
    if (bd0_bd1_angel > bd0_1_loose_angel_max_threshold or bd0_bd1_angel < bd0_1_loose_angel_min_threshold ) then
        return false
    end
end

if (bl == 2) then
    if (len_bd0_jitter/len_bd0 > tight_jitter_max_threshold )  then
        return false
    end
    if (dis0 > tight_dis_max_threshold or dis1 > tight_dis_max_threshold or dis2 > tight_dis_max_threshold) then
        return false
    end
    if (angel0 > bd0_tight_angel_max_threshold or angel0 < bd0_tight_angel_min_threshold or angel1 > bd1_tight_angel_max_threshold) then
        return false
    end

    if (bd0_bd1_angel > bd0_1_tight_angel_max_threshold or bd0_bd1_angel < bd0_1_tight_angel_min_threshold ) then
        return false
    end
    if (bd1_bd2_angel > bd1_2_tight_angel_max_threshold or bd1_bd2_angel < bd1_2_tight_angel_min_threshold ) then
        return false
    end
end
return true
end



