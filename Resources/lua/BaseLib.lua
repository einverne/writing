--友情提示 使用前请先将编码改为UTF-8
---------------------------------------------数据类-----------------------------------------------------------------------------------------------------------
_G.InflectionPoint = _G.InflectionPoint or {}
_G.allBHTable = _G.allBHTable or {}
local print = print
local type = type
local loadstring = loadstring
local table = table
local math = math
local pairs = pairs
local error = error
local string = string
local tonumber = tonumber
local InflectionPoint = InflectionPoint
local allBHTable = allBHTable
module("BaseLib")
local WriteHZ  = nil
local HZStruct = nil
local RunRule = nil
local WZEnv = nil
local StdHZ = nil
local BhNum = nil
local pointstr =nil
local PointStr={}
local errorType={}
local errorBH={}
local errorPoint={}
local pointstable={}
typeInfo ={}
allInfoStr = ""
retInfoStr = ""
infostr= {}
BasePointTableStrings={}
resultTable={}
--------------------------------------------数据访问---------------------------------------------------------------------------------------------------------------------------------------------
function setWriteZiInfo(WZ)
	WriteHZ = WZ
end

function setHZStruct(HS)
	HZStruct = HS
end

function setAllRule(RC)
	RunRule = RC
end

function setbhNum(bhNum)
	BhNum= bhNum
end

function setWZEnv(env)
	WZEnv = env
end

function setStdHZ(SZ)
	StdHZ = SZ
end

function GetPoints(str)
	pointstr=str
end

function ReturnErrorType()
	return errorType
end
function ReturnErrorPoint()
	return errorPoint
end

function ReturnErrorBH()
	return errorBH
end

function ReturntypeInfo()
	for i=1,#typeInfo do
		local te = typeInfo[i]
		for k,v in pairs(te) do
			print"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
			print(k,v)
		end
	end
	return typeInfo
end

function initStrokeStrs(PointTableStrings)
BasePointTableStrings={}
	for i=1,#PointTableStrings do
		BasePointTableStrings[#BasePointTableStrings+1] = PointTableStrings[i]
		print(PointTableStrings[i].."\n")
	end
end

function Write2Txt(point)
	local file=io.open("inflection.txt","a") --当前目录"2.txt"不需要存在
	file:write(point.."@")
	io.flush()
	io.close()
	file:close()
end

function GetBH( idx )
	local bh = WriteHZ.strokes[idx + 1]
	return bh
end

function GetPreBH(idx)
	local bh = WriteHZ.strokes[idx + 1]
	return bh
end

function Points2Num(Str)
	local strokeStrs  = {}
	strokeStrs  = Str:split(Str,"@")--返回分割后的table
	local allstr=""
   for i=1,#strokeStrs do
		local str=strokeStrs[i]
		allstr=allstr..str
	end
	print(allstr)
	local ptSet = {}
	for strx,stry in string.gmatch(allstr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSet[#ptSet+1] = pt
	end
	self.ptSet = ptSet
end

function GetStartIndex(bh)--取笔画起点索引
     return 1
end
function GetEndIndex(bh)--取笔画末点索引
     return #bh.ptSet
end
function GetMidIndex ( bh )--取笔画中点
 	local point = WZEnv.POINT:new()
	local len= #bh.ptSet
	local tmpIdx = math.floor ( len / 2 )
	point.x = bh.ptSet[tmpIdx].x
	point.y = bh.ptSet[tmpIdx].y
	print(len,tmpIdx)
	return tmpIdx
end

function GetStartPoint(bh)--取笔画起点
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[1].x
	point.y = bh.ptSet[1].y
	return point ,1
end

function GetEndPoint(bh)--取笔画末点
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[#bh.ptSet].x
	point.y = bh.ptSet[#bh.ptSet].y
	return point ,#bh.ptSet
end

function GetMidPoint ( bh )--取笔画中点
 	local point = WZEnv.POINT:new()
	local len= #bh.ptSet
	local tmpIdx = math.floor ( len / 2 )
	point.x = bh.ptSet[tmpIdx].x
	point.y = bh.ptSet[tmpIdx].y
	print(len,tmpIdx)
	return point,tmpIdx
end

function	GetMidIndex( bd )--取笔画中点索引
 	local point = WZEnv.POINT:new()
	local len= #bd.ptSet
	local tmpIdx = math.floor ( len / 2 )
	return tmpIdx
end

function GetPointX ( pt )--取点x值
	return pt.x
end

function GetPointY ( pt )--取点y值
	return pt.y
end

function GetBDByBH(bh,bdIdx)--通过笔画和笔段索引取笔段
	local bd = WZEnv.BD:new()
	local preIdx = 1
	local postIdx = #bh.ptSet
	print(bdIdx)
	print(postIdx)
	print(bdIdx)
	if (bdIdx ~= 0) then
		preIdx = bh.InflectionPoint[bdIdx] + 1
	end
	if (bdIdx < #bh.InflectionPoint) then
	  postIdx = bh.InflectionPoint[bdIdx + 1]
	end
	print(preIdx,postIdx)
	local bdPtSet = {}
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	print("getBD!")
	print(preIdx,postIdx)
	return bd
end

function GetTurningPtNum(bh,BDNum)--取笔画的拐点数
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

	if(#turning_ind <= 0) then
		return 0
	end

	if (#turning_ind < BDNum - 1) then
		bh.InflectionPoint = turning_ind
		return #turning_ind;
	end

	if (#turning_ind >= BDNum-1) then
		local pos1 = turning_ind[1]
		local pos2 = turning_ind[#turning_ind]
		local spt = bh.ptSet[1]
		local ept = bh.ptSet[#bh.ptSet]
		local dis1 = math.sqrt( math.pow(bh.ptSet[pos1].x - spt.x,2) + math.pow(bh.ptSet[pos1].y - spt.y,2))
		local dis2 = math.sqrt( math.pow(bh.ptSet[pos2].x - ept.x,2) + math.pow(bh.ptSet[pos2].y - ept.y,2))
		if( dis1 < 30 ) then
			n_prePos = turning_ind[1]
			n_postPos = #bh.ptSet
			for i = 1, n_prePos-1 do
				table.remove(bh.ptSet,1)
			end
			table.remove(turning_ind,1)
			if ( #turning_ind > 0 ) then
				for i = 1,#turning_ind do
					 turning_ind[i] = turning_ind[i] - n_prePos + 1
				end
			end
		end
		if (dis2 < 30) then
			n_prePos = 1
			n_postPos = turning_ind[#turning_ind]
			table.remove(turning_ind,#turning_ind)
			for i = #bh.ptSet,n_postPos+1,-1 do
				table.remove(bh.ptSet,i)
			end
		end

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
		print (#turning_ind)
		print ("test over")
		return #turning_ind
	end
end

function GetTurningPt(bh,index)--根据笔画和索引取拐点
	local ptIdx = bh.InflectionPoint[index + 1]
	return bh.ptSet[ptIdx]
end

function GetBDNum(bh)--取笔画的笔段数
	 local tnum = GetTurningPtNum(bh)
	 local bdnum = tnum + 1
	 return bdnum
 end

 function trace(var)
	retInfoStr = ""
	local varType = type(var)
	if(varType == "string") then
		retInfoStr = var .. "\r"
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

function trace2(var)
	retInfoStr = ""
	local varType = type(var)

	if(varType == "string") then
		retInfoStr = var
	end
	if(varType == "table") then
			retInfoStr = var
	end
	if (varType == "number") then
		retInfoStr = var
	end
	print"typettttttttttttttttttttttttttt"
	print(retInfoStr)
	infostr[#infostr+1] =retInfoStr
	for i=1,#infostr do
	print"(((((((((((((((((((((((((((((("
	local te = infostr[i]
		for j=1,#te do
		print(te[i])
		end
	end
end

function GetRightMostPoint ( bh )--取笔画最右点
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

function GetTopMostPoint ( bh )--取笔画最上点
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

function GetBottomMostPoint ( bh )--取笔画最下点
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

function GetVPoint(bh,pt)--取笔画上距离给定点最近的点
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

function GetJoint(bh1, bh2)--取两笔交点
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

function GetTempBD(bh,preIdx,postIdx)--根据索引取临时笔段
	local bd = WZEnv.BD:new()
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	return bd
end

function PointSame(pt1,pt2)--判断两点是否为相同的点
	local IsSame = false
	if (pt1.x == pt2.x and pt1.y == pt2.y) then
		IsSame = true
	end
	return IsSame
end

function resample(bh)--笔画重采样
	local newbh = {}
	newbh.ptSet = {}
	--local n = GetBDLen(bh)/I
	--local I = GetBDLen(bh) / (n - 1)
	local I = 0.5
	local n = GetBDLen(bh)/I
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
	--print"MMMMMMMMMMMMMMMMMM"
	--for i=1,#dstpoList do
	--print(dstpoList[i].x,dstpoList[i].y)
	--end
	--print(#dstpoList)
	newbh.ptSet = dstpoList
	return newbh
end

function GetInflection()--取拐点数
local temp = {}
print (#InflectionPoint)
	for i=1,#InflectionPoint do
		temp[i]=InflectionPoint[i]
		print"得到拐点数$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
		print (temp[i])
	end
	return temp
end

function GetBDLen(bd)--取笔段长度
	local len = 0
	for i = 1,#bd.ptSet - 1 do
		local curpt = bd.ptSet[i]
		local nextpt = bd.ptSet[i+1]
		local dis = math.sqrt( math.pow(curpt.x - nextpt.x,2) + math.pow(curpt.y - nextpt.y,2))
		len = len + dis
		--print(#bd.ptSet,i,len)

	end
	return len
end

function splitPoints(ptStr)
	local ptSet = {}
	for strx,stry in string.gmatch(ptStr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSet[#ptSet+1] = pt
	end
	return ptSet
end

function GetBHByPoint(bh)--确定当前笔画为字的第几笔
	local index=0
	local points ={}
	for i=1,#BasePointTableStrings do
	points=splitPoints(BasePointTableStrings[i])
	local startdot=bh.ptSet[1]
	local enddot=bh.ptSet[#bh.ptSet]
		if(startdot.x == points[1].x and enddot.x == points[#points].x)then
		index = i
		break
		end
	end
	--从笔画0开始计算
	return index-1
end

function Box()--包围盒计算 返回tempXmin,tempXmax,tempYmin,tempYmax
	local tempYmin =512
	local tempYmax =0
	local tempXmin =512
	local tempXmax =0

	local ptSets = {}
	for strx,stry in string.gmatch(pointstr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSets[#ptSets+1] = pt
	end
	for i= 1,#ptSets do

		if (ptSets[i].y > tempYmax) then
			tempYmax=ptSets[i].y
		end
		if (ptSets[i].y < tempYmin) then
			tempYmin=ptSets[i].y
		end
		if (ptSets[i].x> tempXmax) then
			tempXmax=ptSets[i].x
		end
		if (ptSets[i].x < tempXmin) then
			tempXmin=ptSets[i].x
		end
	end
	print"x小 x大 y小 y大"
	print(tempXmin,tempXmax,tempYmin,tempYmax)

	--[[local LeftUpPoint= WZEnv.POINT:new()
	LeftUpPoint.x = tempXmin
	LeftUpPoint.y = tempYmin
	local RightDownPoint= WZEnv.POINT:new()
	RightDownPoint.x = tempXmax
	RightDownPoint.y = tempYmax
	return LeftUpPoint,RightDownPoint--]]

	return tempXmin,tempXmax,tempYmin,tempYmax
end

function trim3(s)
	return (string.gsub(s,"@",""))
end

function BoxCenter(BHtable)--给定包含几个笔画的表 返回形成的包围盒中心点
--[[包围盒计算ing...
		local strokeStrs  = {}
		local tempYmin =512
		local tempYmax =0
		local tempXmin =512
		local tempXmax =0

		local ptSets = {}
		for strx,stry in string.gmatch(pointstr,"(%d+)/(%d+)") do
			local pt = {}
			pt.x = tonumber(strx)
			pt.y = tonumber(stry)
			ptSets[#ptSets+1] = pt
		end
		for i= 1,#ptSets do
			--print(ptSets[i].x,ptSets[i].y)
			if (ptSets[i].y > tempYmax) then
			tempYmax=ptSets[i].y
			end
			if (ptSets[i].y < tempYmin) then
			tempYmin=ptSets[i].y
			end
			if (ptSets[i].x> tempXmax) then
			tempXmax=ptSets[i].x
			end
			if (ptSets[i].x < tempXmin) then
			tempXmin=ptSets[i].x
			end
		end
		print"PPPPPPPPPPPPPPPPP"
		print"x小 x大 y小 y大"
		print(tempXmin,tempXmax,tempYmin,tempYmax)
		local Xdiff=tempXmax-tempXmin
		local Ydiff=tempYmax-tempYmin
		local trueX = tempXmin + Xdiff/2
		local StrokeAndPoint = {}
			local LastNum = 0
			for j = 1,#ShuBHtable,1 do
				local bh = ShuBHtable[j]
				local strokeNum = GetBHByPoint(bh)
				if(LastNum == 0 or LastNum ~= strokeNum)then
					local index = PTtable[j]
					StrokeAndPoint[""..strokeNum]=""..index/#bh.ptSet
					curpoint = ""..index/#bh.ptSet
					LastNum = strokeNum
				elseif(LastNum == strokeNum)then
					local index = PTtable[j]
					StrokeAndPoint[""..strokeNum]=curpoint.."/"..""..index/#bh.ptSet
					curpoint = curpoint.."/"..""..index/#bh.ptSet
				end
			end

			local strokeStrs  = {}
	strokeStrs  = Str:split(Str,"@")--返回分割后的table
	local allstr=""
   for i=1,#strokeStrs do
		local str=strokeStrs[i]
		allstr=allstr..str
	end
	print(allstr)]]
	local BHPTSets = ""
	for i = 1,#BHtable,1 do
		print"###################"
		--local tp = BHtable[1]
		--print (#tp.ptSet)
		--print (tp.ptSet[1].x)
		local tempbh= BHtable[i]
		for j = 1,#tempbh.ptSet,1 do
			--local str.x = tempbh[j].x
			--local str.y = tempbh[j].y
			--local str = str.x..str.y
		   BHPTSets = BHPTSets..tempbh.ptSet[j].x.."/"..tempbh.ptSet[j].y.."/"
		print"%%%%%%%%%%%%%%%%%%%%%%%%%%%%BIHUADIANJI"
		print (BHPTSets)
		end

	end
	print(BHPTSets)
	--local Tempstr = trim3(BHPTSets)
	--print"去掉@@@@@@@@@@@@@@@"
	--print(Tempstr)
	local tempYmin =512
	local tempYmax =0
	local tempXmin =512
	local tempXmax =0

	local ptSets = {}
	for strx,stry in string.gmatch(pointstr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSets[#ptSets+1] = pt
	end
	for i= 1,#ptSets do

		if (ptSets[i].y > tempYmax) then
			tempYmax=ptSets[i].y
		end
		if (ptSets[i].y < tempYmin) then
			tempYmin=ptSets[i].y
		end
		if (ptSets[i].x > tempXmax) then
			tempXmax=ptSets[i].x
		end
		if (ptSets[i].x < tempXmin) then
			tempXmin=ptSets[i].x
		end
	end
	print"x小 x大 y小 y大"
	print(tempXmin,tempXmax,tempYmin,tempYmax)

	local BoxCenter = WZEnv.POINT:new()

	BoxCenter.x = tempXmin + ( tempXmax - tempXmin )/2
	BoxCenter.y = tempYmin + ( tempYmax - tempYmin )/2
	print (BoxCenter.x,BoxCenter.y)

	return BoxCenter
end



function GerResult(ret,bhpoint)
--处理ret
resultTable[1]=ret
resultTable[2]=bhpoint
print"......................"
print(#resultTable[2])
end

--------------------------------------------几何计算函数------------------------------------------------------------------------------------------------------------------------------------
function Cal_Angle(prePt,curPt,postPt)--计算三点组成的角的角度
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

function GetAngel(spt,ept)--计算两点组成的角的角度
	local degree = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		degree = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))
	if (degree < 0) then
			degree = degree + 180
		end
	else
		if (ept.y < spt.y) then
			degree = -90
			degree = -90
		else
			degree = 90
		end
	end
	return degree
end

 function GetDistance(pt1,pt2)--计算两点距离
	local dis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
	return dis
 end


function GetLine(pt1,pt2)--计算通过两点的直线相关数据
	local linevar = {}
	linevar[1] = pt2.y - pt1.y
	linevar[2] = pt1.x - pt2.x
	linevar[3] = pt1.y*pt2.x - pt2.y*pt1.x
	print(pt1.x,pt1.y)
	print(pt2.x,pt2.y)
	print(linevar[1])
	print(linevar[2])
	print(linevar[3])
	return linevar
end

function Point2LineDown(pt,line)--点在直线下方
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result < 0) then
		return true
	end
	return false
end

function Point2LineUp(pt,line)--点在直线上方
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result > 0) then
		return true
	end
	return false
end

function Point2LineRight(pt,line)--点在直线右侧
	local a,b,c = line[1],line[2],line[3]
	--line: ax + by + c = 0
	local x = pt.x
	local y = pt.y
	if (a == 0) then
			return false
	end

	local result_x = (-c-b*y)/a
	if (result_x < x) then
		return true
	else
		return false
	end
end

function GetLeftMostPoint ( bh )--点在直线左侧
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

function Cal_Point2LineDis( pt, a, b, c)--计算点到直线距离
	local x = pt.x
	local y = pt.y
	local m = math.abs(a*x+b*y+c)
	local n = math.sqrt(a*a+b*b)
	--print(m,n)
	local dis = m/n
	return dis
end

function Cal_Direction(pt, a, b,c)
	local x = pt.x
	local y = pt.y
	local dir = a*x+b*y+c
	return dir
end


function GetFarthestPt2Line(bh,line)--计算笔画上距离某直线距离最远的点
	local a,b,c = line[1],line[2],line[3]
	--print(a,b,c)
	local maxDis = 0
	local index = 1
	for i = 1,#bh.ptSet do
		local pt = bh.ptSet[i]
		local dis = Cal_Point2LineDis(pt,a,b,c)
		--print(dis)
		if (dis > maxDis) then
			maxDis = dis
			index = i
		end
	end
	return bh.ptSet[index],index
end

function GetNearestPt2Line(line,bh)--计算笔画上距离某直线距离最近的点
	local a,b,c = line[1],line[2],line[3]
	--print(a,b,c)
	local maxDis = 512
	local index = 1
	for i = 1,#bh.ptSet do
		local pt = bh.ptSet[i]
		local dis = Cal_Point2LineDis(pt,a,b,c)
		--print(dis)
		if (dis < maxDis) then
			maxDis = dis
			index = i
		end
	end
	return bh.ptSet[index],index
end

function GetFarDis2Line(bh,line)--计算笔画与某直线的最远距离
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

function GetBHTrend(bh,line)
	local a,b,c = line[1],line[2],line[3]
	local trendArray = {}
	trendArray[#trendArray + 1 ] = 0
	local preDis = 0
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

function GetXAngel(spt,ept)--取两点连线与X轴夹角（绝对值）
	local angel = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		angel = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))
		angel = math.abs(angel)
	end
	return angel
end

function GetXAngel2(spt,ept)--取两点连线与X轴夹角
	local angel = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		angel = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))
	end
	return angel
end

function GetYAngel(spt,ept)--取两点连线与Y轴夹角（绝对值）
	local angel = 0
	if (math.abs(spt.y - ept.y)>0.0001 ) then
		angel = math.deg(math.atan( (ept.x- spt.x) / (ept.y - spt.y)))
		angel = math.abs(angel)
	end
	return angel
end

function GetYAngel2(spt,ept)--取两点连线与Y轴夹角
	local angel = 0
	if (math.abs(spt.y - ept.y)>0.0001 ) then
		angel = math.deg(math.atan( (ept.x- spt.x) / (ept.y - spt.y)))
	end
	return angel
end

function SmallXiangJiaoDian(pt11,pt12,pt21,pt22)--相交点标记
	local flag1 = 0
	local flag2 = 0
	local pt = WZEnv.POINT:new()
	pt.x, pt.y = 0 , 0
	local vp = {}
	if (PointSame(pt11,pt21) or PointSame(pt11,pt22)) then
		pt.x=pt11.x
		pt.y=pt11.y
			print("xiangjiaolapointsame!")
		return pt
	elseif (PointSame(pt12,pt21) or PointSame(pt12,pt22))then
		pt.x=pt12.x
		pt.y=pt12.y
		print("xiangjiaolapointsame!")
		return pt
	else
		if(pt11.x ==  pt12.x ) then
			flag1 = 1
		end

		if (pt21.x == pt22.x) then
			flag2 = 1
		end

		local k1,c1
		local k2,c2
		if (flag1 == 1 and flag2 == 1)  then
			return nil
		elseif(flag1 == 1 ) then
			vp.x = pt11.x
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.y = k2*(vp.x) + c2
		elseif(flag2 == 1) then
			vp.x = pt21.x
			k1 = (pt12.y - pt11.y) /(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			vp.y = k1*(vp.x) + c1

		else
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
			print("xiangjiaola!")
			pt.x=vp.x
			pt.y=vp.y
			return pt
		else
			return nil
		end
	end
end

function SmallXiangJiao(pt11,pt12,pt21,pt22)--相交点
	local flag1 = 0
	local flag2 = 0
	if (PointSame(pt11,pt21) or PointSame(pt11,pt22)) then
		return true
	elseif (PointSame(pt12,pt21) or PointSame(pt12,pt22))then
		return true
	else
		if(pt11.x ==  pt12.x ) then
			flag1 = 1
		end

		if (pt21.x == pt22.x) then
			flag2 = 1
		end

		local vp = {}
		local k1,c1
		local k2,c2
		if (flag1 == 1 and flag2 == 1)  then
			return false
		elseif(flag1 == 1 ) then
			vp.x = pt11.x
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.y = k2*(vp.x) + c2
		elseif(flag2 == 1) then
			vp.x = pt21.x
			k1 = (pt12.y - pt11.y) /(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			vp.y = k1*(vp.x) + c1

		else
			k1 = (pt12.y - pt11.y)/(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.x = (c1 - c2) /(k2 - k1)
			vp.y = (vp.x*k1) + c1
		end

			local maxX1 = pt11.x
			local minX1 = pt12.x
			--print(pt11.x,pt11.y)
			--print(pt12.x,pt12.y)
			--print(pt21.x,pt21.y)
			--print(pt22.x,pt22.y)
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
				print("xiangjiaola!")
				return true
			else
				return false
			end
	end
end

function Judge2Dots(pt1,pt2)--判断两点距离是否小于一定阈值
	local disThreshold =  70
	print("Judge2Dots")
	local distance = GetDistance(pt1,pt2)
	if (distance < disThreshold) then
		return true
	else
		return false
	end
end

function JudgeDotLine(pt,bd)--判断点到笔段的最小距离是否小于一定阈值
	--local newbh = resample(bd)
	local tempDis =  512
	local disThreshold = 30
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


----------------------------------------------------位置关系----------------------------------------------------------------------------------------------------------------------------------
function IsPosRight(idx)--与标准字位置进行比较（以后可能会弃用）
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

function BH2BHXiangJiaoDIAN(bh1,bh2)--取两笔相交点，返回的是bh1上面的点索引
	local flag = false
	local index=0
	local pt = WZEnv.POINT:new()
	for i = 1, #bh2.ptSet - 1 do
		for j = 1, #bh1.ptSet - 1 do
			flag = SmallXiangJiao(bh1.ptSet[j],bh1.ptSet[j+1],bh2.ptSet[i],bh2.ptSet[i+1])
			pt = SmallXiangJiaoDian(bh1.ptSet[j],bh1.ptSet[j+1],bh2.ptSet[i],bh2.ptSet[i+1])
			if (flag == true) then
			index = j
			break
			end
		end
		if (flag == true) then
			break
		end
	end
	if (flag == true) then
	print"xiaojiaoing"
	print(pt.x,pt.y)
	end
	if(flag == false)then
	print"weixiangjiao"
	end
	return pt,index
end

function BH2BHXiangJiao(bh1,bh2)--判断两笔是否相交
	local flag = false
	for i = 1, #bh2.ptSet - 1 do
		for j = 1, #bh1.ptSet - 1 do
			flag = SmallXiangJiao(bh1.ptSet[j],bh1.ptSet[j+1],bh2.ptSet[i],bh2.ptSet[i+1])
			if (flag == true) then
			print"shixiangjiao"
			break
			end
		end
		if (flag == true) then
					print"shixiangjiao"
			break
		end
	end
	--print(NodePoint.x)
	return flag
end

function BH2BHUp(bh1,bh2)--判断笔画1是否在笔画2上方（以后可能会弃用）
	local flag =false
	local startpt,startindex = GetStartPoint(bh1)
	local endpt,endindex = GetEndPoint(bh1)
	local line0 = GetLine(startpt,endpt)
	local npt,npt_index=GetNearestPt2Line(line0,bh2)
	for i = 1 ,#bh1.ptSet - 1 do
		if(npt.y <= bh1.ptSet[i].y) then
			flag = false
			break
		else
			flag = true
		end
	end
	if(BH2BHXiangJiao(bh1,bh2)== true) then
		flag = false
	end
	return flag
end

function BH2BHXiangJie(bd1,bd2,type1,type2)--判断两个笔段是否相接 相接类型为 0 startpoint 1 endpoint 2 line
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
					flag = JudgeDotLine(bd2.ptSet[#bd2.ptSet],bd1)
					print("======flag")
					print(flag)
				end
	end

	return flag
end

------参数：PBD是笔画或笔段，CBD是笔画或笔段，SorE是标识。
------功能：判断CBD的起点，在PBD某个点的左边还是右边。
------返回值：3表示左半区，4表示右半区
function DirectionLR(PBD,SorE,CBD)--SorE表示PBD的首点还是末点 1首点2末点
	local flag = 0
	local p1,p1Idx = GetStartPoint(CBD)

	local p3,p3Idx = GetStartPoint(PBD)
	local p4,p4Idx = GetEndPoint(PBD)
	local vector = {}
	vector[1] = p1.x - p3.x
	vector[2] = p1.x - p4.x
	vector[3] = p1.y - p3.y
	vector[4] = p1.y - p4.y
	if (SorE == 1)then
		if ( vector[1] > 0 )then
			flag = 4
			print"右半区"
			return flag
		end
		if ( vector[1] < 0 )then
			flag = 3
			print"左半区"
			return flag
		end
	end

	if (SorE == 2)then
		if ( vector[2] > 0 )then
			flag = 4
			print"右半区"
			return flag
		end
		if ( vector[2] < 0 )then
			flag = 3
			print"左半区"
			return flag
		end
	end



	return flag
end

------参数：PBD是笔画或笔段，CBD是笔画或笔段，SorE是标识。
------功能：判断CBD的起点，在PBD某个点的上边还是下边。
------返回值：1表示上半区，2表示下半区
function DirectionUD(PBD,SorE,CBD)--SorE表示PBD的首点还是末点 1首点 2末点
	local flag = 0
	local p1,p1Idx = GetStartPoint(CBD)

	local p3,p3Idx = GetStartPoint(PBD)
	local p4,p4Idx = GetEndPoint(PBD)
	local vector = {}
	vector[1] = p1.x - p3.x
	vector[2] = p1.x - p4.x
	vector[3] = p1.y - p3.y
	vector[4] = p1.y - p4.y
	if (SorE == 1)then
		if ( vector[3] > 0.0 )then
			flag = 2
			print"下半区"
			return flag
		end
		if ( vector[3] <= 0.0 )then
			flag = 1
			print"上半区"
			return flag
		end
	end

	if (SorE == 2)then
		if ( vector[4] > 0 )then
			flag = 2
			print"下半区"
			return flag
		end
		if ( vector[4] < 0 )then
			flag = 1
			print"上半区"
			return flag
		end
	end


	return flag
end

----------------------------------------------笔画评判--------------------------------------------------------------------------------------------------------------------------------------------
function  IsShu(bh,bl)--竖
print("shu is ok")
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local curve = 0
	local angel = 90

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	InflectionPoint.Shu= infle
	currentBH.inflection = InflectionPoint.Shu

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH
	print(#allBHTable.."!!!!!!!!!!!!!!!!!!!!")
	if (startpt.y >= endpt.y) then
		return false
		else
		local line = GetLine(startpt,endpt)
		local len1 = GetBDLen(bh)
		local dis = GetDistance(startpt,endpt)
		curve = len1 / dis

		if (endpt.y ~= startpt.y) then
			local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
			angel = math.deg(math.atan(slope))
		end
		angel = math.abs(angel)
	end

	if(angel > 22)then
		return false
	elseif(angel > 4.4 and angel <= 12.7 and curve> 1.12)then
		return false
	end

	if(bl == 1)then
		if(angel > 12.7 and angel <= 22)then
			return true
		elseif(angel <= 12.7 and curve <= 1.12)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel <= 12.7 and curve <= 1.12)then
			return true
		else
			return false
		end
	end
end

function  IsHeng(bh,bl)--横
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local curve_var = 0
	local angel_var = 0

	if(startpt.x >= endpt.x) then
			return false
	else
		local line = GetLine(startpt,endpt)
		local dis_var = GetBDLen(bh)
		local line_len = GetDistance(startpt,endpt)
		curve_var = dis_var / line_len
		local angel = 90
		if(startpt.x ~= endpt.x) then
			local slope = (endpt.y - startpt.y) / (endpt.x - startpt.x)
			angel = math.deg ( math.atan(slope))
		end
		angel_var = math.abs(angel)
	end
	bh.InflectionPoint[#bh.InflectionPoint + 1] = endindex

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	currentBH.inflection= infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH
	print(#allBHTable.."!!!!!!!!!!!!!!!!!!!!")


	if(angel_var > 50.3)  then
		return false
	end
	if(angel_var <= 9.86 and curve_var > 1.3) then
		return false
	end
	if(curve_var > 1.4) then
		return false
	end

	if (bl == 1 )then
		if(angel_var > 14.86 and angel_var <= 50.3)then
			return true
		elseif(angel_var <= 14.86 and curve_var <= 1.3)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel_var <= 14.86 and curve_var <= 1.1)then
			return true
		else
			return false
		end

	end
end

function IsHengZhe(bh,bl)--横折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)
	local bottom_pt,bottom_index = GetBottomMostPoint(bh)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

	local currentBH = {}

	local infle ={}
	infle[1] = startindex
	infle[2] = turning_index_0
	infle[3] = endindex
	currentBH.inflection = infle


	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH
	print(#allBHTable.."!!!!!!!!!!!!!!!!!!!!")


	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= endpt.y ) then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
		return false
	end

	local Dindex=endindex - bottom_index
	print("here")
	print(endindex,bottom_index,Dindex)
	if( Dindex > 4 )then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (endpt.y ~= turning_pt_0.y) then
		 angel1 = GetYAngel2(turning_pt_0,endpt);
	end
	print("here")
	print(angel0,angel1,wanqu1)
	if(angel0 > 21.6 )then
		return false
	elseif(angel0 <= 9.95 and angel1 >=30 and wanqu1 > 1.3)then
		return false
	end


	if(bl == 1)then
		if(angel0 > 9.95 and angel0 <= 21.6 and wanqu1 <= 1.3)then
			return true
		--elseif(angel0 <= 9.95 and angel1 > 13.7 and wanqu1 <= 1.3)then
			--return true
		elseif(angel0 <= 9.95 and angel1 <= 30 and wanqu1 <= 1.3)then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel0 <= 9.95 and angel1 <= 13.7 and angel1 > -30 and wanqu1 <= 1.08)then
			return true
		else
			return false
		end
	end
end

function IsHengZhe2(bh,bl)--横折2
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local bottom_pt,bottom_index = GetBottomMostPoint(bh)
	local line = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

	local currentBH = {}

	local infle ={}
	infle[1] = startindex
	infle[2] = turning_index_0
	infle[3] = endindex
	currentBH.inflection = infle


	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH
	print(#allBHTable.."!!!!!!!!!!!!!!!!!!!!")

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= endpt.y ) then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
		return false
	end

	local Dindex=endindex - bottom_index
	print("here")
	print(endindex,bottom_index,Dindex)
	if( Dindex > 4 )then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 0
	local angel1 = 0
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetYAngel2(turning_pt_0,endpt);
	end
	if(angel0 > 21.6)then
		return false
	elseif(angel0 > 12.7 and angel0 <= 21.6 and angel1 > 30)then
		return false
	end

	if(bl == 1)then
		if(angel0 > 12.7 and angel0 <= 21.6 and angel1 <= 30)then
			return true
		elseif(angel0 <= 12.7)then
			return true
		else
			return false
		end
	end
	print("===========")
	print(angel0,angel1,wanqu1)

	if(bl == 2 )then
		if(angel0 <= 12.7 and angel1 >-45 and angel1 <= 13.7 and wanqu1 <= 1.08 and wanqu0 <= 1.02)then
			return true
		elseif(angel0 <= 12.7 and angel1 > -45 and angel1 <= 13.7 and wanqu1 <= 1.06)then
			return true
		else
			return false
		end
	end
end

function IsHengZheXieGou(bh,bl)--横折斜钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

	local line0 = GetLine(startpt,turning_pt_1)
	local tmpBD = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(tmpBD,line0)
	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)

	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)

	local line1 = GetLine(turning_pt_0,turning_pt_1)
	local fpt,findex = GetFarthestPt2Line(bd1,line1)
	findex = findex + turning_index_0
	fpt = bh.ptSet[findex]

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

	local currentBH = {}

	local infle ={}
	infle[1] = startindex
	infle[2] = turning_index_0
	infle[3] = turning_index_1
	infle[4] = endindex
	currentBH.inflection = infle


	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end

	if (Point2LineUp(turning_pt_0,line0) == false) then
		return false
	end

	if (Point2LineDown(fpt,line1) == false) then
		return false
	end
	if(startpt.x >=  turning_pt_0.x)then
		return false
	end
	if(turning_pt_0.y >= turning_pt_1.y )then
		return false
	end
	if(turning_pt_1.y <= endpt.y)then
		return false
	end

	local tmpDis = 512
	local ptX = 0
	for i = turning_index_0, turning_index_1 do
		local curPt = bh.ptSet[i]
		local curDis = math.abs(curPt.y - endpt.y)
		if(curDis < tmpDis) then
			ptX = curPt.x
			tmpDis = curDis
		end
	end
	if(ptX > endpt.x) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	if (turning_pt_0.x ~= startpt.x) then
		local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
		angel0 = math.deg(math.atan(slope0))
	end
	local angel1 = 90
	if (turning_pt_1.y ~= turning_pt_0.y) then
		local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
		angel1 = math.deg ( math.atan(slope1))
	end
	print("jisuanliang")
	print(angel0,angel1)
	print(wanqu0,wanqu1)
	if(angel1 <= -3.89)then
		return false
	elseif(angel1 > 3.89 and angel0 <= -18.5)then
		return false
	elseif(angel1 > 3.89 and angel0 > 9.74)then
		return false
	end

	if(bl == 1)then
		if(angel1 > -3.89 and angel1 <= 3.89)then
			return true
		elseif(angel1 > 3.89 and angel0 > -18.5 and angel0 <= -6.78 )then
			return true
		elseif(angel1 > 3.89 and angel0 > -6.78 and angel0 <= 9.74)then
			return true
		else
			print("here")
			return false
		end
	end

	if(bl == 2)then
		if(angel1 > 3.89 and angel0 > 4.55 and angel0 <= 9.74 and wanqu1 > 1.01)then
			--print("ok1")
			return true
		elseif(angel1 > 6.57 and angel0 >-6.78 and angel0 <= 4.55 and wanqu1 > 1.01)then
			--print("ok2")
			return true
		else
			return false
		end
	end
end

function IsHengZheZhePie(bh,bl)--横折折撇
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line_spt_ept = GetLine(startpt,endpt)
	local tmpDis = 0

	local fpt = {}
	local findex = 0
	local maxX = 0
	for i = 1,#bh.ptSet do
		local curPt = bh.ptSet[i]
			if (curPt.x > maxX) then
			maxX = curPt.x
			fpt = bh.ptSet[i]
			findex = i
		end
	end
	if (findex == 1 or findex == endindex) then
		return false
	end

	local tmpBD1 = GetTempBD(bh,1,findex)
	local BD1_len = GetBDLen(tmpBD1)
	local BD1_dis = GetDistance(startpt,fpt)
	local BD1_curve = BD1_len / BD1_dis



	local turning_pt_0 = {}
	local turning_pt0_index = 1

	local turning_pt_1 = {}
	local turning_pt1_index = 1

	local turning_pt_2 = {}
	local turning_pt2_index = 1

	local loop_start_index = 1
	local loop_end_index = 1
	local tag = 1

	if (BD1_curve > 1.2) then

		loop_start_index = 1
		loop_end_index = findex
		if(loop_end_index < 1) then
			return false
		end
		turning_pt2_index = findex
		turning_pt_2 = bh.ptSet[findex]
		tag = 3
	else
		loop_start_index = findex + 5
		if(loop_start_index > endindex) then
			return false
		end
		loop_end_index = endindex
		turning_pt0_index = findex
		turning_pt_0 = bh.ptSet[findex]
		tag = 1
	end
	local beginIdx = 1
	local beginPt= bh.ptSet[1]
	local tmpAngle = 90

	for i = loop_start_index , loop_end_index  do
			local curPt = bh.ptSet[i]
			local curBD = {}
			if(tag == 3) then
				curBD = GetTempBD(bh,i,loop_end_index)
			end
			if(tag == 1) then
				curBD = GetTempBD(bh,loop_start_index,i)
			end
			local cur_len = GetBDLen(curBD)
			if(cur_len / BD1_len > 1/4) then
				local curAngel = 90
				if (curPt.y ~= fpt.y) then
					local slope = (curPt.x - fpt.x) / (curPt.y - fpt.y)
					curAngel = math.deg(math.atan(slope))
				end
				curAngel = math.abs(curAngel)
				if(curAngel < tmpAngle) then
					tmpAngle = curAngel
					beginIdx = i
					beginPt = bh.ptSet[i]
				end
				if(curAngel < 0.1) then
					beginIdx = i
					beginPt = bh.ptSet[i]
					break
				end
			end
		end


		tmpDis = 512
		loop_start_index = beginIdx
		loop_end_index = findex
		if (beginIdx > findex) then
			loop_start_index = findex
			loop_end_index = beginIdx
		end

		tmpDis = 0
		line_spt_ept = GetLine(bh.ptSet[loop_start_index],bh.ptSet[loop_end_index])
		for i = loop_start_index,loop_end_index do
			local curPt = bh.ptSet[i]
			if (Point2LineDown(curPt,line_spt_ept) == true) then
				local curDis = Cal_Point2LineDis(curPt,line_spt_ept[1],line_spt_ept[2],line_spt_ept[3])
				if (curDis > tmpDis) then
					tmpDis = curDis
					turning_pt1_index = i
					turning_pt_1 = bh.ptSet[i]
				end
			end
		end

	if (turning_pt1_index == 1 or turning_pt1_index == endindex) then
			return false
		end
	if (turning_pt0_index == 1) then
		local line1 = GetLine(startpt,turning_pt_1)
		tmpDis = 0
		for i = 1,turning_pt1_index do
			local curPt = bh.ptSet[i]
			local curDis = Cal_Point2LineDis(curPt,line1[1],line1[2],line1[3])
			if(curDis > tmpDis) then
				tmpDis = curDis
				turning_pt0_index = i
				turning_pt_0 = bh.ptSet[i]
			end
		end
	end

		if (turning_pt0_index == 1 or turning_pt0_index == endindex) then
			return false
		end

	if (turning_pt2_index == 1 )then
		local maxX = 0
		for i = turning_pt1_index,endindex do
			local curPt = bh.ptSet[i]
			if(curPt.x > maxX) then
				maxX = curPt.x
				turning_pt2_index = i
				turning_pt_2 = bh.ptSet[i]
			end
		end
	end
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_pt0_index
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_pt1_index
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_pt2_index
	--print"HENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezheHENGzhezhezhe"
	print(turning_pt0_index,turning_pt1_index,turning_pt2_index)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=turning_pt2_index
	infle[5]=endindex
	--InflectionPoint.HengZheZhePie= infle
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	local angel0 = 90
	local wanqu23 = 1
	local angel2 = 90
	local wanqu1 = 1

	if (turning_pt2_index == 1 or turning_pt2_index == endindex) then
			return false
		end
	if (startpt.x >= turning_pt_0.x or  turning_pt_0.y >= turning_pt_1.y
	or turning_pt_1.x >= turning_pt_2.x
	or turning_pt_2.x <= endpt.x or turning_pt_2.y >= endpt.y) then
		return false
	else
		local bd0 = GetTempBD(bh,1,turning_pt0_index)
		local bd1 = GetTempBD(bh,turning_pt0_index,turning_pt1_index)
		local bd2 = GetTempBD(bh,turning_pt1_index,turning_pt2_index)
		local bd3 = GetTempBD(bh,turning_pt2_index,endindex)



		if (turning_pt_0.x ~= startpt.x) then
			local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
			angel0 = math.deg(math.atan(slope0))
		end
		angel0 = math.abs(angel0)
		bd0_dis = GetDistance(startpt,turning_pt_0)
		bd0_len = GetBDLen(bd0)
		wanqu1 = bd0_len / bd0_dis


		bd1_dis = GetDistance(turning_pt_0,turning_pt_1)
		bd1_len = GetBDLen(bd1)
		bd2_dis = GetDistance(turning_pt_1,turning_pt_2)
		bd2_len = GetBDLen(bd2)

		wanqu23= bd1_len / bd1_dis + bd2_len / bd2_dis

		if (turning_pt_2.x ~= turning_pt_1.x) then
			local slope2 = (turning_pt_2.y -  turning_pt_1.y)/(turning_pt_2.x - turning_pt_1.x)
			angel2 = math.deg(math.atan(slope2))
		end
		angel2 = math.abs(angel2)
		print("==============jisuanliang")
		print(angel0,angel2)
		if(angel2 > 51.7)then
			return false
		elseif( wanqu1 > 1.3 or wanqu23 > 2.6)then
			return false
		elseif(angel2 > 38 and angel2 <= 51.7 and wanqu23 <= 2.1)then
			return false
		end

		if(bl == 1)then
			if(angel2 > 38 and angel2 <= 51.7 and wanqu1<= 1.3 and wanqu23 <= 2.6)then
				return true
			elseif(angel2 <= 38)then
				return true
			else
				return false
			end
		end
		print("here")
		print(angel0,angel2,wanqu1,wanqu23)
		if(bl == 2)then
			if(angel2 <= 20 and angel0 <= 13.7 and wanqu1 <= 1.05 and wanqu23 <= 2.3)then
				return true
			elseif(angel2 > 20 and angel2<= 28.4 and angel0 <= 11.1 and wanqu1 <= 1.05 and wanqu23 <= 2.3)then
				return true
			else
				return false
			end
		end
	end
end

function  IsPie(bh,bl)--撇
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	local fpt,fpt_index = GetFarthestPt2Line(bh,line)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis
	local angel = 90
	angel = GetYAngel(startpt,endpt);
	bh.InflectionPoint[#bh.InflectionPoint + 1] = endindex

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	--InflectionPoint.Pie= infle
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable
	allBHTable[#allBHTable+1] = currentBH

	print"PiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePiePie"
	--print(InflectionPoint.Pie)
	if (startpt.y >= endpt.y or startpt.x <= endpt.x) then
		return false
	end

	if (Point2LineUp(fpt,line) == false) then
		return false
	end

	if(curve > 1.46)then
		return false
	elseif(angel <= 1.11 and curve <=4.42)then
		return false
	end


	if (bl == 1 )then
		if(curve <= 1.46 and curve > 1.01 )then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(curve <= 1.3 and angel > 4.42 )then
			return true
		else
			return false
		end
	end
end

function  IsNa(bh,bl)--捺
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line = GetLine(startpt,endpt)
	local fpt,fpt_index = GetFarthestPt2Line(bh,line)
	local len = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len / dis
	local angel = 90

	bh.InflectionPoint[#bh.InflectionPoint + 1] = endindex

	print"?????????????????????????????????????????????????????????????????????????????????????????????????????????????"
	print(endindex)
	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	--InflectionPoint.Na= infle
   currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH
	--print(InflectionPoint.Na)
	if (startpt.y >= endpt.y or startpt.x >= endpt.x) then
		return false
	end



	if (Point2LineDown(fpt,line) == false) then
		return false
	end

	if (endpt.x ~= startpt.x) then
		angel = GetYAngel(startpt,endpt);
	end

	if(curve > 1.5)then
		return false
	elseif(curve <= 1.02 and angel <= 10.5)then
		return false
	end
	if (bl == 1 )then
		if(curve <= 1.02 and angel >10.5)then
			return true
		elseif(curve <= 1.2 and curve > 1.01)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(curve <= 1.2 and curve > 1.01)then
			return true
		else
			return false
		end
	end
end
function IsHengGou(bh,bl)--横钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_0,turning_index_0 = GetRightMostPoint(bh)

	--local line = GetLine(startpt,endpt)
	--local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	local scale = 1
	scale = len_bd1/len_bd0
	--local jiajiao = Cal_Angle(startpt,turning_pt_0,endpt)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (turning_pt_0.y >= endpt.y  or endpt.x >= turning_pt_0.x   ) then
		return false
	end

	if (startpt.x > endpt.x) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	--local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end
	if(angel0 > 7.4)then
		return false
	elseif(angel0 <= 7.4 and scale > 0.75)then
		return false
	elseif(angel0 <= -10.9 and scale <= 0.65 and wanqu0 > 1.1)then
		return false
	end

	if( bl == 1)then
		if(angel0 <= 7.4 and scale > 0.65 and scale <= 0.75)then
			return true
		elseif(angel0 <= -10.9 and scale <= 0.65 and wanqu0 <= 1.1)then
			return true
		elseif(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.65 and wanqu0 > 1.04 and wanqu0 <= 1.13)then
			return true
		elseif(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.65 and wanqu0 <= 1.4)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.65 and wanqu0 <= 1.05 and wanqu1 <= 1.2)then
			return true
		elseif(angel0 > -8.7 and angel0 <=7.4 and scale <= 0.65 and wanqu0 <= 1.05 and wanqu1 <= 1.2)then
			return true
		else
			return false
		end
	end
end

function IsWanGou(bh,bl)--弯钩
		local startpt,startindex = GetTopMostPoint(bh)
		local endpt =  GetEndPoint(bh)
		local endindex = #bh.ptSet
		local turning_pt,turning_index = GetBottomMostPoint(bh)
		local line = GetLine(startpt,turning_pt)
		bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index

		local currentBH = {}

		local infle ={}
		infle[1]=startindex
		infle[2]=turning_index
		infle[3]=endindex
		currentBH.inflection = infle

		local bdTable = {}
		bdTable[1] = GetBDByBH(bh,0)
		bdTable[2] = GetBDByBH(bh,1)
		currentBH.bd = bdTable

		allBHTable[#allBHTable+1] = currentBH

		if (endindex <= startindex) then
			return false
		end

		if(startpt.y >= turning_pt.y)then
			return false
		end

		if (turning_pt.y <= endpt.y) then
			return false
		end

		if (Point2LineDown(endpt,line) == false) then
			return false
		end

		 local bd0 = GetTempBD(bh,startindex,turning_index)
		 local bd1 = GetTempBD(bh,turning_index,endindex)

		 local line0 = GetLine(startpt,turning_pt)
		 local line1 = GetLine(turning_pt,endpt)

		 local dis_bd0 = GetDistance(startpt,turning_pt)
		 local dis_bd1 = GetDistance(turning_pt,endpt)

		 local len_bd0 = GetBDLen(bd0)
		 local len_bd1 = GetBDLen(bd1)

		if (len_bd0 == 0 or len_bd1 == 0) then
			return false
		end

		 local angel0 = 90
		 if (turning_pt.y ~= startpt.y) then
			 local slope = (turning_pt.x -  startpt.x)/(turning_pt.y - startpt.y)
			 angel0 = math.deg(math.atan(slope))
		 end

		local wanqu0 =  len_bd0 / dis_bd0
		local wanqu1 =  len_bd1 / dis_bd1
		local bili  = len_bd1 / len_bd0

		if(bili <= 0.07 and wanqu0 > 1.05)then
			return false
		elseif(bili > 0.07 and bili <= 0.41 and angel0 <= -25.2)then
			return false
		elseif(bili > 0.41 and angel0 > 21)then
			return false
		end

		if(bl == 1)then
			if(bili <= 0.07 and wanqu0 <= 1.05)then
				return true
			elseif(bili > 0.07 and bili <= 0.41 and angel0 > -25.2 and angel0 <= -16.5)then
				return true
			elseif(bili > 0.07 and bili <= 0.41 and angel0 > -16.5 and wanqu0 <= 1.03)then
				return true
			elseif(bili > 0.07 and bili <= 0.41 and angel0 > -16.5 and angel0 <= 16.7 and wanqu0 > 1.03)then
				return true
			elseif(bili > 0.07 and bili <= 0.41 and angel0 > 16.7 and wanqu0 > 1.03)then
				return true
			elseif(bili > 0.41 and angel0 <= 21)then
				return true
			else
				return false
			end
		end
		if(bl == 2)then
			if(bili > 0.07 and bili <= 0.41 and angel0 > -16.5 and angel0 <= 16.7 and wanqu0 > 1.03 and wanqu0 <= 1.21)then
				return true
			else
				return false
			end
		end
end

function IsHengZheGou(bh,bl)--横折钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

	local line0 = GetLine(startpt,turning_pt_1)
	local bdTemp = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)

	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)
	local dis_line = GetDistance(startpt,turning_pt_1)

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end

	if (startpt.x >= turning_pt_1.x or startpt.y >= turning_pt_1.y) then
		return false
	end

	if (endpt.x >= turning_pt_1.x or endpt.y >= turning_pt_1.y) then
		return false
	end
	if	(len_bd2/dis_line >= 1/2) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (turning_pt_1.y~= turning_pt_0.y) then
		angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	--print("jisuanliang==========")
	--print(angel0,angel1)
	--print(wanqu0,wanqu1)
	if(angel0 <= (-25.4) or angel0 > 12.7)then
		return false
	end

	if(bl == 1)then
		if(angel0 <=12.7 and angel0 > -25.4)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 > -16 and angel0 <= 12.7 and angel1 > -31.4 and angel1 <= 10.56 and wanqu0 <= 1.06 and wanqu1 <= 1.06)then
			return true
		else
			return false
		end
	end
end

function IsHengZheWanGou1(bh,bl)--横折弯钩1
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_maxDown
	local tmpDis = 512
	local turning_pt_2 = {}
	local turning_index_2 = 1

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_2 = i
			turning_pt_2 = bh.ptSet[i]
		end
	end

	local vpt = {}
	vpt.x = turning_pt_2.x
	vpt.y = startpt.y
	local turning_pt_0 = bh.ptSet[1]
	local turning_index_0 = 1
	local temp = 90
	local index=turning_index_2
	for i = 1, index do
		local cpt = bh.ptSet[i]
		local angel = 0
		if (cpt.y == endpt.y) then
			angel = 90
		else
			angel = Cal_Angle (cpt,turning_pt_2,vpt)
		end
		if (angel < temp) then
        temp = angel
        turning_index_0  = i
		  turning_pt_0  = bh.ptSet[i]
		end
	end

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bdmiddle = GetTempBD(bh,turning_index_0,turning_index_2)
	local line0 = GetLine(turning_pt_0,turning_pt_2)
	local turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdmiddle,line0)
	turning_index_1 = turning_index_1 + turning_index_0
	turning_pt_1 = bh.ptSet[turning_index_1]
	print("guaidian")
	print(startindex)
	print(turning_index_0)
	print(turning_index_1)
	print(turning_index_2)
	print(endindex)

	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local len_bd3 = GetBDLen(bd3)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=turning_pt2_index
	infle[5]=endindex
	--InflectionPoint.HengZheZhePie= infle
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0 or len_bd3 == 0 ) then
		return false
	end
	if (startpt.x >= turning_pt_0.x) then
	print(2)
	return false
	end

	if (turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end

	if (turning_pt_1.x >= turning_pt_2.x ) then
	print(4)
	return false
	end
	if (turning_pt_2.y <= endpt.y ) then
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 0
	local angel1 = 0
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (turning_pt_0.y ~= turning_pt_1.y) then
		 angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	if(wanqu1 <= 1.1 and angel1 <= -7.5 and angel0 <= -16.7)then
		return false
	elseif(wanqu1 > 1.3)then
		return false
	elseif(wanqu1 > 1.1 and wanqu1 <= 1.3 and angel0 > -0.65)then
		return false
	end

	if(bl == 1)then
		if(wanqu1 <= 1.1 and angel1 <= -7.5 and angel0 >= -16.7)then
			return true
		elseif(wanqu1 > 1.1 and wanqu1 <= 1.3 and angel0 <= 0.65)then
			return true
		elseif(wanqu1 <= 1.1 and angel1 > -7.5)then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(wanqu1 <= 1.1 and angel1 > -7.5)then
			return true
		else
			return false
		end
	end
end

function IsXieWanGou(bh,bl)--斜弯钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

	local line0 = GetLine(startpt,turning_pt_1)
	local bdTemp = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)

	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)
	print("111111111111")
	print(turning_index_0)
	print(turning_index_1)

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1

	print("inflectionPoint:")
	print(bh.InflectionPoint[1])

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end

	if (endpt.x >= turning_pt_1.x or endpt.y >= turning_pt_1.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (turning_pt_1.y ~= turning_pt_0.y) then
		angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	if(angel1 <= -33.5)then
		return false
	end

	if(bl == 1)then
		if(angel1 > -33.5)then
			return true
		else
			return false
		end
	end
print("============")
print(angel0,angel1,wanqu0,wanqu1)
	if(bl == 2)then
		if(angel1 > -23.1 and angel0 <= -8.4 and wanqu0 <= 1.04 and wanqu1 <= 1.06)then
			return true
		else
			return false
		end
	end
end

function IsShuWanGou(bh,bl)--竖弯钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt0 = {}
	cmpPt0.x = n_minLeft
	cmpPt0.y = n_maxDown
	local tmpDis = 512
	local turning_pt_0 = {}
	local turning_index_0 = 1

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt0)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_0 = i
			turning_pt_0 = bh.ptSet[i]
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_maxDown
	local tmpDis = 512
	local turning_pt_1 = {}
	local turning_index_1 = 1

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_1 = i
			turning_pt_1 = bh.ptSet[i]
		end
	end

	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1
	local line0 = GetLine(startpt,turning_pt_1)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 ==0) then
		return false
	end

	if(startpt.y >= turning_pt_0.y)then
		return false
	end

	if(turning_pt_0.x >= turning_pt_1.x)then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
		return false
	end

	if (endpt.y >= turning_pt_1.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel2(turning_pt_0,endpt);
	end

	if(angel0 <= -9.2 and wanqu0 > 1.03)then
		return false
	elseif(angel0 > -9.2 and wanqu0 > 1.24)then
		return false
	end

	if(bl == 1)then
		if(angel0 <= -9.2 and wanqu0 <= 1.03)then
			return true
		elseif(angel0 > -9.2 and wanqu0 <= 1.24)then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel0 > -9.2 and wanqu0 <= 1.03 and wanqu1 <= 1.07)then
			return true
		elseif(angel0 > -9.2 and wanqu0 > 1.03 and wanqu0 <= 1.13 and wanqu1 <= 1.02)then
			return true
		else
			return false
		end
	end
end

function IsZuoDian(bh,bl)--左点
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	currentBH.inflection= infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH



	if (startpt.y >= endpt.y or startpt.x <= endpt.x ) then
		return false
	end
	if(curve > 1.8)then
		return false
	end

	if(bl == 1 or bl == 2)then
		if(curve <= 1.8)then
			return true
		else
			return false
		end
	end
end

function IsDian(bh,bl)--点
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis
	InflectionPoint[#InflectionPoint+1] = endindex

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	currentBH.inflection= infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH



	if (startpt.y >= endpt.y or startpt.x >= endpt.x ) then
		return false
	end
	if(curve > 1.54)then
		return false
	end
end

function IsShuGou(bh,bl)--竖钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_0,turning_index_0 = GetBottomMostPoint(bh)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	local line = GetLine(startpt,turning_pt_0)
	local scale = 1
	scale = len_bd1/len_bd0
	--local jiajiao = Cal_Angle(startpt,turning_pt_0,endpt)
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if(startpt.y >= turning_pt_0.y)then
		return false
	end

	if (turning_pt_0.y <= endpt.y) then
		return false
	end

	if (Point2LineDown(endpt,line) == false) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 0
	--local angel1 = 0

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end

	if(scale > 0.7 or scale <= 0.05)then
		return false
	end

	if(bl == 1 )then
		if(angel0 <= -8.5 and scale > 0.05)then
			return true
		elseif(angel0 >-8.5)then
			return true
		else
			return false
		end
	end
	print(scale)
	print(angel0,wanqu0)
	if(bl ==2 )then
		if(angel0 > -8.5 and scale > 0.17 and scale <= 0.55 and wanqu0 <= 1.04)then
			return true
		else
			return false
		end
	end
end

function IsShuZheZheGou(bh,bl)--竖折折钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_2,turning_index_2 = GetBottomMostPoint(bh)

	local leftpt,leftindex = GetLeftMostPoint(bh)
	if (leftindex <= startindex+5) then
		n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
		for i = 1,#bh.ptSet do
			if (n_minLeft > bh.ptSet[i].x) then
				n_minLeft = bh.ptSet[i].x
			end

			if(n_maxRight < bh.ptSet[i].x) then
				n_maxRight = bh.ptSet[i].x
			end

			if(n_minUp > bh.ptSet[i].y) then
				n_minUp = bh.ptSet[i].y
			end

			if(n_maxDown < bh.ptSet[i].y) then
				n_maxDown = bh.ptSet[i].y
			end
		end

		local cmpPt0 = {}
		cmpPt0.x = n_maxRight
		cmpPt0.y = n_minUp
		local tmpDis = 512
		turning_pt_1 = bh.ptSet[1]
		turning_index_1 = 1
		for i = 1, #bh.ptSet do
			local curPt = bh.ptSet[i]
			local curDis = GetDistance(curPt,cmpPt0)
			if(curDis < tmpDis) then
				tmpDis = curDis
				turning_index_1 = i
				turning_pt_1 = bh.ptSet[i]
			end
		end
		local bds0 = GetTempBD(bh,startindex,turning_index_1)
		local lines0 = GetLine(startpt,turning_pt_1)
		turning_pt_0,turning_index_0 = GetFarthestPt2Line(bds0,lines0)
	else
		local vpt = {}
		vpt.x = leftpt.x
		vpt.y = turning_pt_2.y
		turning_pt_0 = bh.ptSet[1]
		turning_index_0 = 1
		local temp = 90
		for i = 1, turning_index_2 do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.x == endpt.x) then
				angel = 90
			else
				angel = Cal_Angle (cpt,turning_pt_2,vpt)
				--print(angel)
			end
			if (angel < temp) then
				temp = angel
				turning_index_0  = i
				turning_pt_0  = bh.ptSet[i]
			end
		end
		bd0 = GetTempBD(bh,startindex,turning_index_0)
		local bdlast = GetTempBD(bh,turning_index_0,turning_index_2)
		turning_pt_1 = {}
		turning_index_1 = 1
		local line0 = GetLine(turning_pt_0,turning_pt_2)
		local fpt,findex = GetFarthestPt2Line(bdlast,line0)
		turning_index_1 = findex + turning_index_0
		turning_pt_1 = bh.ptSet[turning_index_1]
	end
		print (turning_index_0)
		print (turning_index_1)
		print (turning_index_2)
		print (endindex)
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_1
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_2


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=turning_pt2_index
	infle[5]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local len_bd2 = GetBDLen(bd3)

	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)

	if (len_bd0 == 0) then
	print(1)
	return false
	end

	if (len_bd1 == 0) then
	print(11)
	return false
	end

	if (len_bd2 == 0 or len_bd3 == 0 ) then
	print(111)
	return false
	end

	if (startpt.y >= turning_pt_0.y ) then
	print(2)
	return false
	end

	if (turning_pt_0.x >= turning_pt_1.x) then
	print(3)
	return false
	end

	if (turning_pt_1.y >= turning_pt_2.y ) then
	print(4)
	return false
	end

	if (endpt.x >= turning_pt_2.x or endpt.y >= turning_pt_2.y) then
	print(6)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end
	if (turning_pt_1.y ~= turning_pt_2.y) then
		 angel1 = GetYAngel2(turning_pt_1,turning_pt_2);
	end



	if(wanqu1 <= 0.09 and angel0 > -16.6)then
		return false
	end

	if(bl == 1)then
		if(wanqu1 > 0.09)then
			return true
		elseif(wanqu1 <= 0.09 and angel0 <= -16.6)then
			return true
		else
			return false
		end
	end
	print("here")
	print(wanqu0,wanqu1)
	if(bl == 2)then
		if(wanqu0 <= 1.06  and wanqu1 <= 0.6 and angel0 <= -16.6)then
			return true
		elseif(wanqu0 <= 1.06  and wanqu1 <= 0.6 and angel1 <= -26 and wanqu1 <= 0.75)then
			return true
		elseif(wanqu0 <= 1.06  and wanqu1 <= 0.6 and angel1 > -26 and angel0 <= 4.5)then
			return true
		else
			return false
		end
	end
end

function IsShuZhe(bh,bl)--竖折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	print("==============")
	print(startindex)
	print(endindex)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)
	print("guaidian")
	print(turning_index_0)
	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0
	print("shuzhe is ok")

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	InflectionPoint.ShuZhe = infle
	currentBH.inflection = InflectionPoint.ShuZhe

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.BD = bdTable

	allBHTable[#allBHTable+1]=currentBH
	print(#allBHTable.."!!!!!!!!!!!!!!!!!")
	print(allBHTable[#allBHTable].BD)
	if (len_bd0 == 0 or len_bd1 == 0 ) then
			print("111111111")
		return false

	end

	if (startpt.y >= turning_pt_0.y or turning_pt_0.x >= endpt.x ) then
		print(startpt.y,turning_pt_0.y,turning_pt_0.x,endpt.x)
		print("2222222222")
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
	print("33333333333")
		return false
	end
	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel(turning_pt_0,endpt);
	end
print("here")
	print(angel0,angel1,wanqu0,wanqu1)
	if(angel1 > 14.4)then
		return false
	elseif(angel1 <= 14.4 and wanqu1 > 1.6)then
		return false
	elseif(angel1 > 7.4 and angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 > 50)then
		return false
	elseif(angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 > 50)then
		return false
	end

	if(bl == 1)then
		if(angel1 <= 14.4 and wanqu1 > 1.05 and wanqu1 <= 1.6)then
		print("111111111")
			return true
		elseif(angel1 > 7.4 and angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 <= 50)then
			print("211111111")
			return true
		elseif(angel1 <= 7.4 and wanqu1 <= 1.05 and angel0 <= 50)then
			print("311111111")
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,angel1,wanqu0,wanqu1)
	if(bl == 2)then
		if(angel1 <= 10 and wanqu1 <= 1.05 and angel0 <= 10 and wanqu0 <= 1.2)then
			return true
		else
			return false
		end
	end

end

function IsHengZheWan(bh,bl)--横折弯
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line0 =GetLine(startpt,endpt)
	local fpt,fpt_index=GetFarthestPt2Line(bh,line0);
	local turning_pt_0 = {}
	local turning_index_0 = 1
	local turning_pt_1 = {}
	local turning_index_1 = 1

	if(Point2LineDown(fpt,line0) == true)then
		turning_pt_1 = fpt
		turning_index_1 = fpt_index
		local bdTemp = GetTempBD(bh,startindex,turning_index_1)
		local lineTemp = GetLine(startpt,turning_pt_1)
		turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,lineTemp)
	else
		turning_pt_0 = fpt
		turning_index_0 = fpt_index
		local bdTemp = GetTempBD(bh,turning_index_0,endindex)
		local lineTemp = GetLine(turning_pt_0,endpt)
		turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdTemp,lineTemp)
		turning_index_1 = turning_index_1 + turning_index_0
		turning_pt_1 = bh.ptSet[turning_index_1]
	end
	print (turning_index_0)
	print (turning_index_1)
	print (endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_1)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,endpt)


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)

	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0) then
	print(11)
	return false
	end
	if (len_bd2 == 0) then
	print(111)
	return false
	end
	if (startpt.x >= turning_pt_0.x) then
	print(2)
	return false
	end

	if (startpt.y >= turning_pt_0.y and turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end

	if (turning_pt_1.x >= endpt.x ) then
	print(4)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel2(turning_pt_1,endpt);
	end
	if(angel1 <= -44.2)then
		return false
	elseif(angel1 > -25.2 and angel0 > 16.7)then
		return false
	elseif(angel1 > -25.2 and angel0 <= -21.4)then
		return false
	end

	if(bl == 1)then
		if(angel1 > -44.2 and angel1 <= -25.2)then
			return true
		elseif(angel1 > -25.2 and angel0 > -21.4 and angel0 <= 16.7)then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel1 > (-25.2) and angel1 <= 12.7 and angel0 > (-16.7)and angel0 <= 16.7 and wanqu1 <= 1.13)then
			return true
		else
			return false
		end
	end
end

function IsHengZheTi(bh,bl)--横折提
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

	local line0 = GetLine(startpt,turning_pt_1)
	local bdTemp = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)

	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_pt0_index
	infle[3]=turning_pt1_index
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end


	if (turning_pt_0.y >= turning_pt_1.y) then
		return false
	end


	if (endpt.x <= turning_pt_1.x or endpt.y >= turning_pt_1.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (turning_pt_1.y ~= turning_pt_0.y) then
		angel1 = GetYAngel(turning_pt_0,turning_pt_1);
	end

	if(angel0 > 37.6 and angel1 > 13.3)then

		return false
	end

	if(bl == 1 )then
		if(angel0 > 37.6 and angel1 <= 13.3)then
			return true
		elseif(angel0 > 23.5 and angel0 <= 37.6)then
			return true
		elseif(angel0 <= 23.5)then
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,angel1,wanqu1)
	if(bl == 2)then
		if(angel0 <= 23.5 and angel1 <= 10.2 and wanqu1 <= 1.04 )then
			return true
		elseif(angel1 <= 14.7 and angel1 > 10.2 and wanqu1 <= 1.04)then
			return true
		else
			return false
		end
	end
end

function IsHengPieWanGou(bh,bl)--横撇弯钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_2,turning_index_2 = GetBottomMostPoint(bh)
	local turning_pt_0 = {}
	local turning_index_0 = 1

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end
	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_minUp
	local tmpDis = 512

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_0 = i
			turning_pt_0 = bh.ptSet[i]
		end
	end

		local turning_pt_1 = bh.ptSet[1]
		local turning_index_1 = 1
		local temp = 0
		local a=turning_index_0
		local b=turning_index_2
		for i =a, b do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.x == endpt.x or cpt.x == startpt.x ) then
				angel = 0

			else
				angel = Cal_Angle (startpt,cpt,endpt)
			end
			if (angel > temp) then
			  temp = angel
			  turning_index_1  = i
			  turning_pt_1  = bh.ptSet[i]
			end
		end
	print(turning_index_0)
	print(turning_index_1)
	print(turning_index_2)
	print(endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)
	local line = GetLine(turning_pt_1,turning_pt_2)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=turning_index_2
	infle[5]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0) then
	print(11)
	return false
	end
	if (len_bd2 == 0 or len_bd3 == 0) then
	print(111)
	return false
	end
	if (startpt.y >= turning_pt_1.y ) then
	print(2)
	return false
	end
	if (turning_pt_0.x <= turning_pt_1.x or turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end
	if (turning_pt_1.y >= turning_pt_2.y ) then
	print(4)
	return false
	end
	if (turning_pt_2.y <= endpt.y ) then
	print(5)
	return false
	end
	if (Point2LineDown(endpt,line) == false) then
	return false
	end
	if (startpt.x >= turning_pt_0.x) then
	print(6)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0)
	end
	if (turning_pt_2.y ~= turning_pt_1.y) then
		 angel1 = GetYAngel2(turning_pt_1,turning_pt_2)
	end
	print("jisuanliang===========")
	print(angel0,angel1)
	print(wanqu0,wanqu1)
	if(angel0 > 21.4 and wanqu1 > 1.3)then
		return false
	elseif(angel0 > 16 and wanqu1 <= 1.1)then
		return false
	elseif(angel0 > -13 and angel1 <= 5.3 and wanqu1 <= 1.3 and angel1 > 19.6)then
		return false
	end
	if(bl == 1)then
		if(angel0 <= -13)then
			return true
		elseif(angel0 > -13 and angel0 <= 21.4 and wanqu1 > 1.05)then
			return true
		elseif(angel0 > 5.5 and angel0 <= 16.2 and wanqu1 <= 1.3 )then
			return true
		elseif(angel0 > -13 and angel0 <= 5.3 and wanqu1<= 1.3 and angel1 <= 19.6)then
			return true
		else
			return false
		end
	end
	if(bl == 2 )then
		if(angel0 > -10.6 and angel0 <= 5.3 and wanqu1<= 1.3 and angel1 <= 19.6)then
			return true
		else
			return false
		end
	end
end

function IsShuZheZhe(bh,bl)--竖折折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	print("=================")
	print(startindex,endindex)
	local line0 =GetLine(startpt,endpt);
	local fpt,fpt_index=GetFarthestPt2Line(bh,line0);
	print(fpt_index)
	local turning_pt_0 = {}
	local turning_index_0 = 1
	local turning_pt_1 = {}
	local turning_index_1 = 1

	if(Point2LineDown(fpt,line0) == false)then
		turning_pt_1 = fpt
		turning_index_1 = fpt_index
		local bdTemp = GetTempBD(bh,startindex,turning_index_1)
		local lineTemp = GetLine(startpt,turning_pt_1)
		turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,lineTemp)
	else
		turning_pt_0 = fpt
		turning_index_0 = fpt_index
		local bdTemp = GetTempBD(bh,turning_index_0,endindex)
		local lineTemp = GetLine(turning_pt_0,endpt)
		turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdTemp,lineTemp)
		turning_index_1 = turning_index_1 + turning_index_0
		turning_pt_1 = bh.ptSet[turning_index_1]
	end
	print (turning_index_0)
	print (turning_index_1)
	print (endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)

	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,endpt)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0) then
	print(1)
	return false
	end

	if (len_bd1 == 0) then
	print(11)
	return false
	end

	if (len_bd2 == 0) then
	print(111)
	return false
	end

	if (startpt.y >= turning_pt_0.y ) then
	print(2)
	return false
	end

	if (startpt.x >= turning_pt_1.x and turning_pt_0.x >= turning_pt_1.x) then
	print(3)
	return false
	end

	if (turning_pt_1.y >= endpt.y ) then
	print(4)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end
	if (endpt.y ~= turning_pt_0.y) then
		 angel1 = GetYAngel2(turning_pt_1,endpt);
	end
	if(turning_pt_0.x ~= turning_pt_1.x)then
		angelTemp=GetXAngel(turning_pt_0,turning_pt_1);
	end

	if(angel0 > 10.7 or angelTemp > 50)then
		return false
	elseif(angel0 > -15.3 and angel0 <= 10.7 and angel1 >17)then
		return false
	elseif(angel0 > -15.3 and angel0 <= 10.7 and angel1 <= -38)then
		return false
	end

	if(bl == 1)then
		if(angel0 > -15.3 and angel0 <= 10.7 and angel1 <= 17 and angel1 > -38)then
			return true
		elseif(angel0 <= 15.3) then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel0 > -15.3 and angel0 <= 10.7 and angel1 > -17.7 and angel1 <= 6.7 and angelTemp <= 30)then
			return true
		else
			return false
		end
	end
end

function IsXieShu(bh,bl)--斜竖
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	currentBH.inflection= infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (startpt.y >= endpt.y) then
		return false
	end

	local line = GetLine(startpt,endpt)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis
	local angel = 0
	if (endpt.y ~= startpt.y) then
		local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
		angel = math.deg(math.atan(slope))
	end

	if(angel > 9.2)then
		return false
	elseif(angel <= 9.2 and curve > 1.22)then
		return false
	end


	if (bl == 1 )then
		if(angel <= 9.2 and curve <= 1.22 and angel > -50)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(curve <= 1.22 and angel <= -1.36 and angel > -35 )then
			return true
		else
			return false
		end
	end
end
function IsXieShuZhe(bh,bl)--斜竖折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line0 = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH




	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (Point2LineDown(turning_pt_0,line0) == false ) then
		return false
	end

	if (turning_pt_0.x >= endpt.x or turning_pt_0.y <= startpt.y) then
		return false
	end

	if (startpt.y >= endpt.y ) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel(turning_pt_0,endpt);
	end

	if(angel1 > 37.7)then
		return false
	end
	if (bl == 1 )then
		if(angel1 <= 37.7 and angel1 > 24.5)then
			return true
		elseif(angel1 <= 24.5 and angel1 > 9.8 and wanqu0 > 1.03)then
			return true
		elseif(angel1 <= 24.5 and wanqu0 <= 1.03)then
			return true
		elseif(angel1 <= 9.8 and wanqu0 > 1.03)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(angel1 <= 24.5 and angel1 > 9.8 and wanqu0 > 1.03 and wanqu0 <= 1.05 and angel0 <= 20)then
			return true
		elseif(angel1 <= 24.5 and wanqu0 <= 1.03 and wanqu1 <= 1.03)then
			return true
		elseif(angel1 <= 9.8 and wanqu0 > 1.03 and wanqu0 <= 1.05 and wanqu1 <= 1.03)then
			return true
		else
			return false
		end
	end
end
function IsPieZhe(bh,bl)--撇折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line0 = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH





	if (len_bd0 == 0 or len_bd1 == 0 ) then

		return false
	end
	if (Point2LineDown(turning_pt_0,line0) == false ) then

		return false
	end

	if (turning_pt_0.x >= endpt.x or turning_pt_0.y <= startpt.y) then

		return false
	end
	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	if (turning_pt_0.y ~= startpt.y) then
		angel0 = GetYAngel(startpt,turning_pt_0);
	end
	if(wanqu1 > 1.23)then

		return false
	end

	if (bl == 1 )then
		if(wanqu1 <= 1.23 )then
			return true
		else

			return false
		end
	end
	if (bl == 2 )then
		if(wanqu1 <= 1.1 and angel0 > 10.8 )then
			return true
		else
			return false
		end
	end
end
function IsHengZhePieWan(bh,bl)--横折撇弯
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local rightest_pt,rightest_index = GetRightMostPoint(bh)


	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_minUp
	local tmpDis = 512
	local rightUp_pt = {}
	local rightUp_index = 1
	local cmpRightDown = {}
	cmpRightDown.x = n_maxRight
	cmpRightDown.y = n_maxDown
	local cmpRightUp = {}
	cmpRightUp.x = n_maxRight
	cmpRightUp.y = n_minUp

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			rightUp_index = i
			rightUp_pt = bh.ptSet[i]
		end
	end
	local bdAll = GetTempBD(bh,startindex,endindex)
	local bdRight2RightUp = GetTempBD(bh,rightUp_index,rightest_index)
	local len_bdAll = GetBDLen(bdAll)
	local len_bdRight2RightUp = GetBDLen(bdRight2RightUp)
	local bili=len_bdRight2RightUp/len_bdAll
	print(rightUp_index)
	print(rightest_index)
	local turning_pt_0 = bh.ptSet[1]
	local turning_index_0 = 1
	local turning_pt_2 = bh.ptSet[1]
	local turning_index_2 = 1







	if (bili < 1/10) then
		turning_pt_0 = rightUp_pt
		turning_index_0 = rightUp_index
		--local bdlast = GetTempBD(bh,rightest_index,endindex)
		local temp = 90
		local a=rightest_index+5
		local b=endindex-5
		for i =a, b do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.y == endpt.y ) then
				angel = 90
			else
				angel = Cal_Angle (cpt,rightest_pt,cmpRightDown)
			end
			if (angel < temp) then
			  temp = angel
			  turning_index_2  = i
			  --print(i)
			  turning_pt_2  = bh.ptSet[i]
			end
		end
	else
		turning_pt_2 = rightest_pt
		turning_index_2 = rightest_index
		--local bdlast = GetTempBD(bh,startindex,rightest_index)
		--[[local temp = 90
		local a=startindex
		local b=rightest_index
		for i =a, b do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.y == rightest_pt.y ) then
				angel = 90
			else
				angel = Cal_Angle (rightest_pt,cpt,cmpRightUp)
			end
			if (angel < temp) then
			  temp = angel
			  turning_index_0  = i
			  turning_pt_0  = bh.ptSet[i]
			end
		end--]]
		turning_index_0  = rightUp_index
		turning_pt_0  = rightUp_pt
	end
	local bdTurning02 = GetTempBD(bh,turning_index_0,turning_index_2)
	local lineTurning02 = GetLine(turning_pt_0,turning_pt_2)
	local turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdTurning02,lineTurning02)
	turning_index_1 = turning_index_0 + turning_index_1
	turning_pt_1 = bh.ptSet[turning_index_1]
	print(turning_index_0)
	print(turning_index_1)
	print(turning_index_2)
	print(endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,endpt)


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=turning_index_2
	infle[5]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0) then
	print(11)
	return false
	end
	if (len_bd2 == 0) then
	print(111)
	return false
	end
	if (startpt.x >= turning_pt_0.x ) then
	print(2)
	return false
	end
	if (turning_pt_0.x <= turning_pt_1.x or turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end
	if (turning_pt_1.x >= turning_pt_2.x) then
	print(4)
	return false
	end
	if (turning_pt_1.y >= endpt.y ) then
	print(5)
	return false
	end
	if (turning_pt_2.x <= endpt.x or turning_pt_2.y >= endpt.y ) then
	print(6)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0)
	end
	if (endpt.y ~= turning_pt_1.y) then
		 angel1 = GetYAngel2(turning_pt_1,endpt)
	end
	print("here")
	print(angel0,angel1)
	if(angel0 <= -32.5)then
		return false
	elseif(angel0 > 12.4)then
		return false
	end

	if (bl == 1 )then
		if(angel0 > -32.5 and angel0 <= -13 )then
			return true
		elseif(angel0 > -13 and angel0 <=12.4)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(angel0 > -8.3 and angel0 <=12.4 and angel1 <= 4.5)then
			return true
		else
			return false
		end
	end
end

function IsPieTi(bh,bl)--撇提
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line0 = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (Point2LineDown(turning_pt_0,line0) == false ) then
		return false
	end

	if (turning_pt_0.x >= endpt.x or turning_pt_0.x >= startpt.x or turning_pt_0.y <= startpt.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel(startpt,turning_pt_0)
	end
	if(turning_pt_0.x ~= endpt.x) then
		angel1 =GetXAngel2(turning_pt_0,endpt)
	end

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (bl == 1 )then
		return true
	end
	print(angel0,angel1,wanqu0,wanqu1)
	if (bl == 2 )then
		if(angel0 > 9.2 and wanqu0 <= 1.02 and angel1 <10 )then
			return true
		elseif(angel0 > 9.2 and wanqu0 > 1.02 and wanqu1 <= 1.05 and angel1 <10 )then
			return true
		else
			return false
		end
	end
end

function IsTi(bh,bl)--提
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	if (startpt.y <= endpt.y or startpt.x >= endpt.x) then
		return false
	end
		local line = GetLine(startpt,endpt)
		local len1 = GetBDLen(bh)
		local dis = GetDistance(startpt,endpt)
		local curve = len1 / dis
		vars = curve


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = bh
	currentBH.bd = bdTable
	allBHTable[#allBHTable+1] = currentBH

	if (bl == 1 )then
		return true
	end

	if (bl == 2 )then
		if(curve <= 1.07)then
			return true
		else
			return false
		end
	end
end

function IsWoGou(bh,bl)--卧钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end


		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_maxDown
	local tmpDis = 512
	local turning_pt_0 = {}
	local turning_index_0 = 1

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_0 = i
			turning_pt_0 = bh.ptSet[i]
		end
	end

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local line0 = GetLine(startpt,turning_pt_0)
	local fpt,findex = GetFarthestPt2Line(bd0,line0)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	local scale = 1
	scale = len_bd1/len_bd0

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		print (1)
		return false
	end

	if (startpt.x >= turning_pt_0.x or startpt.y >= turning_pt_0.y ) then
		print ("4")
		return false
	end

	if (endpt.y >= turning_pt_0.y ) then
		print ("5")
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	angel1 =GetYAngel2(turning_pt_0,endpt);


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (angel1 < -10) then
		print ("6")
		return false
	end
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end
	if(scale <= 0.383 and wanqu0 > 1.312)then
		return false
	end

	if (bl == 1 )then
		return true
	end

	if (bl == 2 )then
		if(scale <= 0.383 and wanqu0 > 1.02)then
			return true
		else
			return false
		end
	end
end

function IsHengZheZhe(bh,bl)--横折折
	local startpt,startindex = GetLeftMostPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

   local line0 =GetLine(startpt,endpt);
	local fpt,fpt_index=GetFarthestPt2Line(bh,line0)
	local turning_pt_0 = {}
	local turning_index_0 = 1
	local turning_pt_1 = {}
	local turning_index_1 = 1

	if(Point2LineDown(fpt,line0) == true)then
		turning_pt_1 = fpt
		turning_index_1 = fpt_index
		local bdTemp = GetTempBD(bh,startindex,turning_index_1)
		local lineTemp = GetLine(startpt,turning_pt_1)
		turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdTemp,lineTemp)
	else
		turning_pt_0 = fpt
		turning_index_0 = fpt_index
		local bdTemp = GetTempBD(bh,turning_index_0,endindex)
		local lineTemp = GetLine(turning_pt_0,endpt)
		turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdTemp,lineTemp)
		turning_index_1 = turning_index_1 + turning_index_0
		turning_pt_1 = bh.ptSet[turning_index_1]
	end
	print (turning_index_0)
	print (turning_index_1)
	print (endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)

	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,endpt)


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH





	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0) then
	print(11)
	return false
	end
	if (len_bd2 == 0) then
	print(111)
	return false
	end
	if (startpt.x >= turning_pt_0.x) then
	print(2)
	return false
	end

	if (startpt.y >= turning_pt_0.y and turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end

	if (turning_pt_1.x >= endpt.x ) then
	print(4)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end
	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel2(turning_pt_1,endpt);
	end
	if(angel1 <= -14 and angel0 <= -13.6)then
		return false
	elseif(angel1 > -14 and angel0 <= 20.8 and wanqu1 > 1.1)then
		return false
	elseif(angel1 > -14 and angel0 >= 20.8)then
		return false
	end

	if(bl == 1)then
		if(angel1 <= -14 and angel0 > -13.6)then
			return true
		elseif(angel1 > -14 and wanqu1 <= 1.1 and angel0 <= 20.8)then
			return true
		else
			return false
		end
	end

	if(bl ==2 )then
		if(angel1 > -14 and wanqu1 <= 1.1 and angel0 <= 20.8 and angel0 > -15.6)then
			return true
		else
			return false
		end
	end
end

function IsShuTi(bh,bl)--竖提
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local turning_pt_0,turning_index_0 = GetBottomMostPoint(bh)
	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	local line = GetLine(startpt,turning_pt_0)
	local scale = 1
	scale = len_bd1/len_bd0


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH




	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (turning_pt_0.y <= endpt.y ) then
		return false
	end

	if (startpt.y >= turning_pt_0.y) then
		return false
	end
	if (Point2LineUp(endpt,line) == false) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end
	if(scale > 0.82 or angel0 > 20 or angel0 <= -30)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.03 and angel0 > 13)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.26 and angel0 <= 13 and wanqu1 <= 1.02)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.03 and angel0 <= -8.2 and wanqu1 <= 1.02)then
		return
	end

	if(bl == 1)then
		if(scale > 0.64 and scale <= 0.82 )then
			return true
		elseif(scale <= 0.64 and wanqu0 <= 1.03)then
			return true
		elseif(scale <= 0.64 and wanqu0 > 1.03 and angel0 <=13 and wanqu1 > 1.02)then
			return true
		elseif(scale <= 0.64 and wanqu0 > 1.03 and wanqu0 <= 1.26 and angel0 > -8.2 and angel0 <= 13 and wanqu1 <= 1.02)then
			return true
		else
			return false
		end
	end
print("here")
print(scale,wanqu0,wanqu1,angel0)
	if(bl == 2 )then
		if(scale <= 0.64 and wanqu0 > 1.03 and wanqu0 <= 1.26 and angel0 > -1.4 and angel0 <= 13 and wanqu1 <= 1.05)then
			return true
		elseif(scale <= 0.64 and wanqu0 <= 1.03 and angel0 > -8.5 and angel0 <= 13)then
			return true
		else
			return false
		end
	end
end

function IsHengZheZheZhe(bh,bl)--横折折折
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
		local turning_pt_1 = bh.ptSet[1]
		local turning_index_1 = 1
		local temp = 90
		local a=math.ceil(1/6*endindex)
		local b=math.ceil(5/6*endindex)
		for i =a, b do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.x == endpt.x or cpt.x == startpt.x ) then
				angel = 0
			else
				angel = Cal_Angle (startpt,cpt,endpt)
			end
			if (angel > temp) then
			  temp = angel
			  turning_index_1  = i
			  turning_pt_1  = bh.ptSet[i]
			end
		end
	local line0 = GetLine(startpt,turning_pt_1)
	local bdlast1 = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdlast1,line0)

	local bdlast = GetTempBD(bh,turning_index_1,endindex)
	local line1 = GetLine(turning_pt_1,endpt)
	local fpt,findex = GetFarthestPt2Line(bdlast,line1)
	turning_index_2 = findex + turning_index_1
	turning_pt_2 = bh.ptSet[turning_index_2]
	print (startindex)
	print (turning_index_0)
	print (turning_index_1)
	print (turning_index_2)
	print (endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local len_bd3 = GetBDLen(bd3)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=turning_index_2
	infle[5]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0) then
	print(11)
	return false
	end
	if (len_bd2 == 0 or len_bd3 == 0) then
	print(111)
	return false
	end

	if (startpt.x >= turning_pt_0.x)then
		return false
	end

	if(startpt.y >= turning_pt_1.y or turning_pt_0.y >= turning_pt_1.y) then
	print(2)
	return false
	end

	if (turning_pt_1.x >= turning_pt_2.x) then
	print(3)
	return false
	end

	if (turning_pt_2.y >= endpt.x or turning_pt_1.y >= endpt.y ) then
	print(5)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90


	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end
	if (turning_pt_2.x ~= turning_pt_1.x) then
		 angel1 = GetXAngel2(turning_pt_1,turning_pt_2);
	end

	if (turning_pt_2.y ~= endpt.y) then
		 angel2 = GetYAngel2(turning_pt_2,endpt);
	end

	if(angel2 > 6.9 or angel2 <= -34.5 )then
		return false
	end

	if(bl == 1)then
		if(angel2 > -12.4 and angel2 <= 6.9 and angel1 > -30 and angel1 <= 30)then
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,angel1,angel2,wanqu1)
	if(bl == 2)then
		if(angel2 > -12.4 and angel2 <= 6.9  and angel0 > -11.6 and angel1 > -15 and angel1 <= 11.7 and wanqu1 <= 1.15)then
			return true
		else
			return false
		end
		--return false
	end
end

function IsHengZheWanGou2(bh,bl)--横折弯钩2
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_maxDown
	local tmpDis = 512
	local turning_pt_2 = {}
	local turning_index_2 = 1

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_2 = i
			turning_pt_2 = bh.ptSet[i]
		end
	end

	local vpt = {}
	vpt.x = turning_pt_2.x
	vpt.y = startpt.y
	local turning_pt_0 = bh.ptSet[1]
	local turning_index_0 = 1
	local temp = 90
	local index=turning_index_2
	for i = 1, index do
		local cpt = bh.ptSet[i]
		local angel = 0
		if (cpt.y == endpt.y) then
			angel = 90
		else
			angel = Cal_Angle (cpt,turning_pt_2,vpt)
		end
		if (angel < temp) then
        temp = angel
        turning_index_0  = i
		  turning_pt_0  = bh.ptSet[i]
		end
	end

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bdmiddle = GetTempBD(bh,turning_index_0,turning_index_2)
	local line0 = GetLine(turning_pt_0,turning_pt_2)
	local turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdmiddle,line0)
	turning_index_1 = turning_index_1 + turning_index_0
	turning_pt_1 = bh.ptSet[turning_index_1]

	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local len_bd3 = GetBDLen(bd3)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=turning_index_2
	infle[5]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH



	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0 or len_bd3 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x) then
	print(2)
	return false
	end

	if (startpt.y >= turning_pt_0.y and turning_pt_0.y >= turning_pt_1.y ) then
	print(3)
	return false
	end

	if (turning_pt_1.x >= turning_pt_2.x ) then
	print(4)
	return false
	end

	if (turning_pt_2.y <= endpt.y ) then
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (turning_pt_0.y ~= turning_pt_1.y) then
		 angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	if(angel1 > -7.5)then
		return false
	elseif(angel1 <= -7.5 and angel0 > 12.6)then
		return false
	end
	if(bl == 1)then
		if(angel1 <= -7.5  and angel0 <= 12.6 )then
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,angel1,wanqu1)
	if(bl == 2)then
		if(angel1 > -22.8 and angel1 <= -7.5 and angel0 <= 12.6 and wanqu1 > 1.04)then
			return true
		elseif(angel1 > -60 and angel1 <= -7.5 and angel0 <= 6.06 and wanqu1 <= 1.08)then
			return true
		else
			return false
		end
	end
end

function IsShuZhePie(bh,bl)--竖折撇
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local rightpt,rightindex = GetRightMostPoint(bh)
	local bottompt,bottomindex = GetBottomMostPoint(bh)
	local vpt = {}
	vpt.x = rightpt.x
	vpt.y = bottompt.y
	local turning_pt_1 = {}
	local turning_index_1 = 1
	if	(rightindex == startindex) then
		local temp = 90
		for i = 1, #bh.ptSet do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.y == bottompt.y) then
				angel = 90
			else
				angel = Cal_Angle (cpt,startpt,vpt)
			end
			if (angel < temp) then
			  temp = angel
			  turning_index_1  = i
			  turning_pt_1 = bh.ptSet[i]
			end
		end
	else
        turning_index_1  = rightindex
		  turning_pt_1 = rightpt
	end

	local line = GetLine(startpt,turning_pt_1)
	local bdlast = GetTempBD(bh,startindex,turning_index_1)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bdlast,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)

	local line1 = GetLine(startpt,endpt)


	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH




	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0 ) then
		return false
	end

	if (Point2LineDown(turning_pt_0,line1) == false) then
		return false
	end

	if (Point2LineUp(turning_pt_1,line1) == false) then
		return false
	end

	if (startpt.y >= turning_pt_0.y  or startpt.y >= turning_pt_1.y) then
		return false
	end

	if (turning_pt_0.x >= turning_pt_1.x) then
		return false
	end

	if (turning_pt_1.x <= endpt.x or turning_pt_1.y >= endpt.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	local angel2 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel(startpt,turning_pt_0);
	end

	if (turning_pt_1.x ~= turning_pt_0.x) then
		 angel1 = GetXAngel(turning_pt_0,endpt);
	end

	if (turning_pt_1.y ~= endpt.y) then
		 angel2 = GetYAngel(turning_pt_1,endpt);
	end

	if(angel2 > 49 and angel0 > 24 and angel1 > 75)then
		return false
	elseif(angel2 <= 11.7 and wanqu0 > 1.03)then
		return false
	end

	if(bl == 1 )then
		if(angel2 <= 11.7 and wanqu0 <= 1.03)then
			return true
		elseif(angel2 <= 15.4 and angel2 > 11.7)then
			return true
		elseif(angel2 > 15.4 and angel2 <= 49)then
			return true
		elseif(angel2 > 49 and angel0 > 23.8 and angel1 <= 74.6)then
			return true
		elseif(angel2 > 49 and angel0 <= 23.8)then
			return true
		else
			return false
		end
	end

	if(bl == 2 )then
		if(angel2 <= 15.4 and angel2 > 11.7 and wanqu0 <= 1.04 and wanqu1 <= 1.03)then
			return true
		elseif(angel2 > 27.6 and angel2 <= 49 and angel0 <= 14.6 and angel1 <= 87 and wanqu0 <= 1.18)then
			return true
		elseif(angel2 >15.4 and angel2 <= 49 and angel0 > 14.6 and wanqu0 <= 1.04 and wanqu1 <= 1.03)then
			return true
		elseif(angel2 > 49 and angel0 <= 23.8 and wanqu0 <= 1.04 and wanqu1 <= 1.03)then
			return true
		else
			return false
		end
	end
end
function IsXieGou(bh,bl)--斜钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_0,turning_index_0 = GetBottomMostPoint(bh)
	local line0 = GetLine(startpt,turning_pt_0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	local scale = 1
	scale = len_bd1/len_bd0

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH



	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.y >= endpt.y) then
		return false
	end

	if (startpt.y >= turning_pt_0.y ) then
		return false
	end

	if(turning_pt_0.y <= endpt.y)then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0);
	end

	if(scale <= 0.38 and wanqu0 <= 1.14 and angel0 <= 7.1)then
		return false
	elseif(scale <= 0.38 and wanqu0 <= 1.14 and angel0 > 7.1 and scale <= 0.071)then
		return false
	elseif(scale <= 0.38 and wanqu0 > 1.14)then
		return false
	elseif(scale > 0.61)then
		return false
	elseif(scale > 0.38 and scale <= 0.61 and wanqu0 > 1.05)then
		return false
	end

	if (bl == 1 )then
		if(scale >= 0.38 and scale <= 0.61 and wanqu0 <= 1.05)then
			return true
		elseif(scale <= 0.38 and wanqu0 <= 1.14 and angel0 > 7.1)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(scale <= 0.33 and scale > 0.07 and wanqu0 <= 1.14 and angel0 > 7.1)then
			return true
		else
			return false
		end
	end
end

function IsShuWan(bh,bl)--竖弯
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line0 = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line0)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)

	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH




	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.y >= turning_pt_0.y) then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y) then
		return false
	end

	if (turning_pt_0.x >= endpt.x) then
		return false
	end

	if (Point2LineDown(turning_pt_0,line0) == false) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.y ~= startpt.y) then
		angel0 = GetYAngel2(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		angel1 = GetXAngel2(turning_pt_0,endpt);
	end
	if(angel1 <= -33.9)then
		return false
	elseif(angel1 > -33.9 and angel0 > 20.2)then
		return false
	elseif(angel1 > 45 and angel0 <= 20.2)then
		return false
	end
	if(bl == 1)then
		if(angel1 > -33.9 and angel1 <= 45 and angel0 <= 20.2)then
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,angel1,wanqu0,wanqu1)
	if(bl == 2)then
		if(angel1 > - 10 and angel1 <= 27.5 and angel0 > -7.6 and angel0 <= 7.6 and wanqu0 <= 1.06 and wanqu1 <= 1.18)then
			return true
		else
			return false
		end
	end
end

function IsPieDian(bh,bl)--撇点
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line0 = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line0)
	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	print(startindex)
	print(turning_index_0)

    bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (Point2LineDown(turning_pt_0,line0) == false ) then
		return false
	end
	if (turning_pt_0.x >= endpt.x or turning_pt_0.y <= startpt.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel2(startpt,turning_pt_0)
	end
	if(angel0 > 6.5)then
		return false
	elseif(angel0 <= -32.1 and wanqu1 > 1.1)then
		return false
	end

	if(bl == 1)then
		if( angel0 > -7.4 and angel0 <= 6.5)then
			return true
		elseif(angel0 <= -7.4 and wanqu1 <= 1.1)then
			return true
		elseif(angel0 > -40 and angel0 <= -7.4 and wanqu1 > 1.1)then
			return true
		else
			return false
		end
	end
	print("here")
	print(angel0,wanqu0,wanqu1)

	if(bl == 2)then
		if(angel0 <= -7.4 and wanqu0 <= 1.05 and wanqu1 <= 1.06)then
			return true
		elseif(angel0 > -40 and angel0 <= -7.4 and wanqu0 <= 1.05 and wanqu1 <= 1.06 )then
			return true
		else
			return false
		end
	end
end

function IsHengPie(bh,bl)--横撇
local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_0,turning_index_0 = GetRightMostPoint(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)
	bh.InflectionPoint[#bh.InflectionPoint + 1] = turning_index_0



	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x ) then
		return false
	end

	if (turning_pt_0.x <= endpt.x or turning_pt_0.y >= endpt.y ) then
		return false
	end

	if (startpt.y >= endpt.y ) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (endpt.y ~= turning_pt_0.y) then
		 angel1 = GetYAngel(turning_pt_0,endpt);
	end
	print("=====")
	print(angel0,angel1,wanqu0)
	if(angel0 > 25.4)then
		return false
	elseif(angel0 <= 25.4 and angel1 <= 26.8)then
		return false
	elseif(angel0 <= 25.4 and angel1 > 26.8 and wanqu0 > 1.3)then
		return false
	end
	if(bl == 1)then
		if(angel0 <= 25.4 and angel1 > 26.8 and wanqu0 <= 1.3)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 <= 25.4 and angel1 > 26.8 and wanqu0 <= 1.1)then
			return true
		elseif(angel0 <= 13.2 and wanqu0 <= 1.2 and angel1 > 26.8)then
			return true
		else
			return false
		end
	end
end

function	IsHengZheZheZheGou(bh,bl)--横折折折钩
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local rightest_pt,rightest_index = GetRightMostPoint(bh)
	local turning_pt_3,turning_index_3 = GetBottomMostPoint(bh)

	local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
	for i = 1,#bh.ptSet do
		if (n_minLeft > bh.ptSet[i].x) then
			n_minLeft = bh.ptSet[i].x
		end

		if(n_maxRight < bh.ptSet[i].x) then
			n_maxRight = bh.ptSet[i].x
		end

		if(n_minUp > bh.ptSet[i].y) then
			n_minUp = bh.ptSet[i].y
		end

		if(n_maxDown < bh.ptSet[i].y) then
			n_maxDown = bh.ptSet[i].y
		end
	end

	local cmpPt1 = {}
	cmpPt1.x = n_maxRight
	cmpPt1.y = n_minUp
	local tmpDis = 512
	local rightUp_pt = {}
	local rightUp_index = 1
	local cmpRightDown = {}
	cmpRightDown.x = n_maxRight
	cmpRightDown.y = n_maxDown
	local cmpRightUp = {}
	cmpRightUp.x = n_maxRight
	cmpRightUp.y = n_minUp

	for i = 1, endindex do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt1)
		if(curDis < tmpDis) then
			tmpDis = curDis
			rightUp_index = i
			rightUp_pt = bh.ptSet[i]
		end
	end
	local bdAll = GetTempBD(bh,startindex,endindex)
	local bdRight2RightUp = GetTempBD(bh,rightUp_index,rightest_index)
	local len_bdAll = GetBDLen(bdAll)
	local len_bdRight2RightUp = GetBDLen(bdRight2RightUp)
	local bili=len_bdRight2RightUp/len_bdAll
	print(rightUp_index)
	print(rightest_index)
	 turning_pt_0 = bh.ptSet[1]
	 turning_index_0 = 1
	 turning_pt_1 = bh.ptSet[1]
	 turning_index_1 = 1
	 turning_pt_2 = bh.ptSet[1]
	 turning_index_2 = 1

	if (bili > 1/10) then
		turning_pt_0 = rightUp_pt
		turning_index_0 = rightUp_index
		--local bdlast = GetTempBD(bh,rightest_index,endindex)
		local temp = 0
		for i =turning_index_0, rightest_index do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.y == startpt.y ) then
				angel = 0
			else
				angel = Cal_Angle (cpt,startpt,cmpRightUp)
			end
			if (angel > temp) then
			  temp = angel
			  turning_index_1  = i
			  --print(i)
			  turning_pt_1  = bh.ptSet[i]
			end
		local bdTurning13 = GetTempBD(bh,turning_index_1,turning_index_3)
		local lineTurning13 = GetLine(turning_pt_1,turning_pt_3)
		turning_pt_2,turning_index_2 = GetFarthestPt2Line(bdTurning13,lineTurning13)
		turning_index_2 = turning_index_2 + turning_index_1-1
		turning_pt_2 = bh.ptSet[turning_index_2]
		end
		--turning_pt_2 = rightest_pt
		--turning_index_2 = rightest_index
	else
		turning_pt_2 = rightUp_pt
		turning_index_2 = rightUp_index
		--local bdlast = GetTempBD(bh,startindex,turning_index_2)
		local temp = 90
		local a=startindex
		local b=turning_index_2
		for i =a, b do
			local cpt = bh.ptSet[i]
			local angel = 0
			if (cpt.y == rightUp_pt.y) then
				angel = 90
			else
				angel = Cal_Angle (cpt,rightUp_pt,cmpRightUp)
			end
			if (angel < temp) then
			  temp = angel
			  turning_index_0  = i
			  turning_pt_0  = bh.ptSet[i]
			end
		end
	local bdTurning02 = GetTempBD(bh,turning_index_0,turning_index_2)
	local lineTurning02 = GetLine(turning_pt_0,turning_pt_2)
	turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdTurning02,lineTurning02)
	--print("=========================")
	--print(turning_index_1)
	turning_index_1 = turning_index_0 + turning_index_1-1
	turning_pt_1 = bh.ptSet[turning_index_1]
	end

	print(turning_index_0)
	print(turning_index_1)
	print(turning_index_2)
	print(turning_index_3)
	print(endindex)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,turning_index_2)
	local bd3 = GetTempBD(bh,turning_index_2,turning_index_3)
	local bd4 = GetTempBD(bh,turning_index_3,endindex)
	local len_bd0 = GetBDLen(bd0)
		local len_bd1 = GetBDLen(bd1)
			local len_bd2 = GetBDLen(bd2)
				local len_bd3 = GetBDLen(bd3)
					local len_bd4 = GetBDLen(bd4)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd3 = GetDistance(turning_pt_2,turning_pt_3)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)
	local dis_bd2 = GetDistance(turning_pt_1,turning_pt_2)

	local currentBH = {}

	local infle ={}
	infle[1]=startindex
	infle[2]=turning_index_0
	infle[3]=turning_index_1
	infle[4]=turning_index_2
	infle[5]=turning_index_3
	infle[6]=endindex
	currentBH.inflection = infle

	local bdTable = {}
	bdTable[1] = GetBDByBH(bh,0)
	bdTable[2] = GetBDByBH(bh,1)
	bdTable[3] = GetBDByBH(bh,2)
	bdTable[4] = GetBDByBH(bh,3)
	bdTable[5] = GetBDByBH(bh,4)

	currentBH.bd = bdTable

	allBHTable[#allBHTable+1] = currentBH


	if (len_bd0 == 0) then
	print(1)
	return false
	end
	if (len_bd1 == 0 or len_bd2 == 0) then
	print(11)
	return false
	end
	if (len_bd3 == 0 or len_bd4 == 0) then
	print(111)
	return false
	end
	if (startpt.x >= turning_pt_0.x ) then
	print(2)
	return false
	end

	if(startpt.y >= turning_pt_1.y or turning_pt_0.y >= turning_pt_1.y) then
	print(3)
	return false
	end

	if (turning_pt_1.x >= turning_pt_2.x ) then
	print(5)
	return false
	end
	if (turning_pt_1.y >= turning_pt_3.y  or turning_pt_2.y >= turning_pt_3.y ) then
	print(6)
	return false
	end
	if (endpt.x >= turning_pt_3.x or endpt.y >= turning_pt_3.y ) then
	print(7)
	return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd3/dis_bd3
	local wanqu23 = len_bd1/dis_bd1 + len_bd2/dis_bd2
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0)
	end
	if (turning_pt_2.x ~= turning_pt_1.x) then
		 angel1 = GetXAngel2(turning_pt_1,turning_pt_2)
	end
	print(angel0,angel1,wanqu0,wanqu1,wanqu23)
	if(angel1 > 38 or wanqu23 > 2.6)then
		return false
	elseif(angel1 <= 22.6 and angel0 <= -22.6)then
		return false
	end
	if(bl == 1)then
		if(angel1 <= 22.6 and angel0 > -22.6 )then
			return true
		elseif(angel1 > 22.6 and angel1 <= 38)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel1 <= 22.6 and angel0 > -17.9 and wanqu0 <= 1.06 and wanqu1 <= 1.08 and wanqu23 <= 2.5)then
			return true
		else
			return false
		end
	end
end


-----------------------------------------------------部件的主观侧面评判-----------------------------------------------------------------------------------------------------------------------------

--部件侧面1：水平平齐（已经确认）
--水平平齐的评判思路：判断各个点是否在一条水平线上
--BHtable表示所有笔画，turingtable表示各个笔画的拐点序号，起点为0，第一个拐点为1......
function IsShuiPingPingQi(BHtable,turingtable)--水平平齐 字例：大
	print(#BHtable,#turingtable)
	if(#BHtable ~= #turingtable)then
		return false
	end

	------------计算各个点的索引
	local PTtable={}

	for i=1, #BHtable do
		if(turingtable[i]==0) then
			PTtable[i]=1
		elseif(turingtable[i]<=#BHtable[i].InflectionPoint) then
			PTtable[i]=BHtable[i].InflectionPoint[turingtable[i]]
		else
			PTtable[i]=#BHtable[i].ptSet
		end
	end

	------------阈值评判
	for i = #BHtable,2,-1 do
		local bh2 = BHtable[i]--bh2
		local bh1 = BHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		print(i,index1,index2)
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.y-firpt.y)
		print"水平平齐的差值%%%%%%%%%%%%"
		print(Dvalue)

		if(Dvalue <= 35)then--参数待修改
			print"IsShuiPingPingQiOKOK"
		else
			local strokeNum1 =GetBHByPoint(bh1)
			local strokeNum2 =GetBHByPoint(bh2)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum1]=""..index1/#bh1.ptSet
			StrokeAndPoint[""..strokeNum2]=""..index2/#bh2.ptSet
			temp={}
			temp["errortype"]="A0001"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=math.pow(Dvalue/220.0, 1.2)
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
	return true
end

--部件侧面2：竖直平齐（已经确认）
--竖直平齐的评判思路：判断各个点是否在一条竖直线上
--BHtable表示所有笔画，turingtable表示各个笔画的拐点序号，起点为0，第一个拐点为1......
function IsShuZhiPingQi(BHtable,turingtable)--竖直平齐  字例：片 气 友
	print(#BHtable,#turingtable)
	if(#BHtable ~= #turingtable)then
		return false
	end
	------------计算各个点的索引
	local PTtable={}
	for i=1, #BHtable do
		if(turingtable[i]==0) then
			PTtable[i]=1
		elseif(turingtable[i]<=#BHtable[i].InflectionPoint) then
			PTtable[i]=BHtable[i].InflectionPoint[turingtable[i]]
		else
			PTtable[i]=#BHtable[i].ptSet
		end
	end

	------------阈值评判
	for i = #BHtable,2,-1 do
		local bh2 = BHtable[i]--bh2
		local bh1 = BHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		print(i,index1,index2)
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.x-firpt.x)
		print"竖直平齐的差值%%%%%%%%%%%%"
		print(Dvalue)
		if(Dvalue <= 35)then--参数待修改
			print"IsShuZhiPingQiOKOK"
		else
			local strokeNum1 =GetBHByPoint(bh1)
			local strokeNum2 =GetBHByPoint(bh2)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum1]=""..index1/#bh1.ptSet
			StrokeAndPoint[""..strokeNum2]=""..index2/#bh2.ptSet
			temp={}
			temp["errortype"]="A0008"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=math.pow(Dvalue/220.0, 1.2)
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
	return true
end

----部件侧面3：竖直等分（已经确认）
----竖直等分的评判思路：HengBHtable存储了各个笔画，bdtable存储了各个笔画上的笔段索引
--要求：各个笔段的中点在竖直方向上是等间隔的。
function IsShuZhiDengFen(HengBHtable, bdtable)--竖直等分 字例：三
    if(#HengBHtable ~= #bdtable)then
		return false
	end

	------计算各个点的索引
	local PTtable={}
    for i=1,#bdtable do
		local bh_bd=GetBDByBH(HengBHtable[i],bdtable[i])
		local mid,midIdx = GetMidPoint(bh_bd)

		local tempindex=0
		if(bdtable[i] == 0) then
			tempindex=0
		else
			tempindex=HengBHtable[i].InflectionPoint[bdtable[i]]
		end

		PTtable[i]=tempindex+midIdx
	end

	------计算标准的等分间隔
	local firstbh = HengBHtable[1]
	local firstIndex = PTtable[1]
	local lastbh = HengBHtable[#HengBHtable]
	local lastIndex = PTtable[#PTtable]
	local first = firstbh.ptSet[firstIndex]
	local last = lastbh.ptSet[lastIndex]
	print(first.y,last.y)
	local aver =  math.abs((first.y-last.y)/(#HengBHtable-1))
	print"等距的平均值%%%%%%%%%%%%"
	print(aver)

	--------阈值评判
	for i = #HengBHtable,2,-1 do
		local bh2 = HengBHtable[i]--bh2
		local bh1 = HengBHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.y-firpt.y)
		print"实际的差值"
		print (Dvalue)
		if((math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver<= 0.19)then--参数待修改
			print(Dvalue-aver)
			print"NNNNNNNNNNNNNNNNNNNNNNNNNNNN"
			print(Dvalue,aver,Dvalue-aver)
		else
			print"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
			print(Dvalue,aver,Dvalue-aver)
			local StrokeAndPoint = {}
			local LastNum = 0

			for j = 1,#HengBHtable,1 do
				local bh = HengBHtable[j]
				local strokeNum = GetBHByPoint(bh)
				local index = PTtable[j]
				------------------------------------------------------
				local Isrepeat=false
				for kk, vv in pairs(StrokeAndPoint) do
					if kk == ""..strokeNum then
						Isrepeat=true
					end
				end
				------------------------------------------------------

				if(Isrepeat) then
				    StrokeAndPoint[""..strokeNum]=StrokeAndPoint[""..strokeNum]..index/#bh.ptSet.."/"
				else
				    StrokeAndPoint[""..strokeNum]=""..index/#bh.ptSet.."/"
				end
				print ("StrokeAndPoint".." "..#StrokeAndPoint)
			end
			temp={}
			temp["errortype"]="A0005"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=(math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver*1.5
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
	return true
end

--部件侧面4：水平等分（已经确认）
--水平等分的评判思路：ShuBHtable存储了各个笔画，bdtable存储了各个笔画上的笔段索引
--要求：各个笔段的中点在水平方向上是等间隔的。
function IsShuiPingDengFen(ShuBHtable,bdtable)--水平等分 字例：山
	print(#ShuBHtable,#bdtable)
	if(#ShuBHtable ~= #bdtable)then
		return false
	end

	------计算各个点的索引
	local PTtable={}
	for i=1,#bdtable do
		local bh_bd=GetBDByBH(ShuBHtable[i],bdtable[i])
		local mid,midIdx = GetMidPoint(bh_bd)

		local tempindex=0
		if(bdtable[i] == 0) then
			tempindex=0
		else
			tempindex=ShuBHtable[i].InflectionPoint[bdtable[i]]
		end

		PTtable[i]=tempindex+midIdx
	end

	------计算标准的等分间隔
	local firstbh = ShuBHtable[1]
	local firstIndex = PTtable[1]
	local lastbh = ShuBHtable[#ShuBHtable]
	local lastIndex = PTtable[#PTtable]
	local first = firstbh.ptSet[firstIndex]
	local last = lastbh.ptSet[lastIndex]
	local aver =  math.abs(first.x-last.x)/(#ShuBHtable-1)

	print (math.abs(first.x-last.x))
	print (#ShuBHtable-1)
	print"等分的差值%%%%%%%%%%%%"
	print(aver)

	--------阈值评判
	for i = #ShuBHtable,2,-1 do
		local bh2 = ShuBHtable[i]--bh2
		local bh1 = ShuBHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		--print(i,index1,index2)
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.x-firpt.x)
		print"实际的差值"
		print (Dvalue)
		print(secpt.x,firpt.x)
		if((math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver<= 0.19)then--参数待修改
			print(Dvalue-aver)
			print"NNNNNNNNNNNNNNNNNNNNNNNNNNNN"
			print(Dvalue,aver,Dvalue-aver)
		else
			print"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
			print(Dvalue,aver,Dvalue-aver)
			local StrokeAndPoint = {}
			local LastNum = 0
			for j = 1,#ShuBHtable,1 do
				local bh = ShuBHtable[j]
				local strokeNum = GetBHByPoint(bh)
				local index = PTtable[j]
				StrokeAndPoint[""..strokeNum]=""..index/#bh.ptSet
			end
			temp={}
			temp["errortype"]="A0006"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=(math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver*1.5
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end

	return true
end


--部件侧面4的另一个版本：水平等分（已经确认）
--水平等分的评判思路：BHtable存储了各个笔画，keypointtable存储了各个笔画上的关键点索引
--要求：各个笔画上的关键点在水平方向上是等间隔的。
function IsShuiPingDengFen2(BHtable,keypointtable)--水平等分 字例：开
	if(#BHtable ~= #keypointtable)then
		return false
	end

	------计算各个点的索引
	local PTtable={}
	for i=1,#keypointtable do
		PTtable[i]=keypointtable[i]
	end

	------计算标准的等分间隔
	local firstbh = BHtable[1]
	local firstIndex = PTtable[1]
	local lastbh = BHtable[#BHtable]
	local lastIndex = PTtable[#PTtable]
	local first = firstbh.ptSet[firstIndex]
	local last = lastbh.ptSet[lastIndex]
	local aver =  math.abs(first.x-last.x)/(#BHtable-1)

	print (#BHtable-1)
	print"等分的差值%%%%%%%%%%%%"
	print(aver)

	--------阈值评判
	for i = #BHtable,2,-1 do
		local bh2 = BHtable[i]--bh2
		local bh1 = BHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]

		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.x-firpt.x)
		print"实际的差值"
		print (Dvalue)
		print(secpt.x,firpt.x)
		if((math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver<= 0.19)then--参数待修改
			print(Dvalue-aver)
			print"NNNNNNNNNNNNNNNNNNNNNNNNNNNN"
			print(Dvalue,aver,Dvalue-aver)
		else
			print"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
			print(Dvalue,aver,Dvalue-aver)
			local StrokeAndPoint = {}
			local LastNum = 0

			for j = 1,#BHtable,1 do
				local bh = BHtable[j]
				local strokeNum = GetBHByPoint(bh)
				local index = PTtable[j]
				------------------------------------------------------
				local Isrepeat=false
				for kk, vv in pairs(StrokeAndPoint) do
					if kk == ""..strokeNum then
						Isrepeat=true
					end
				end
				------------------------------------------------------
				if(Isrepeat) then
					StrokeAndPoint[""..strokeNum]=StrokeAndPoint[""..strokeNum]..index/#bh.ptSet.."/"
				else
					StrokeAndPoint[""..strokeNum]=""..index/#bh.ptSet.."/"
				end
			end

			temp={}
			temp["errortype"]="A0006"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=(math.max(Dvalue,aver)-math.min(Dvalue,aver))/aver*1.5
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end

	return true
end


--部件侧面5：中点切分（已确认）
--中点切分的评判思路：
--笔画bh1的第bd1idx笔段，被笔画bh2的第bd2idx笔段切分为相等的两段。笔段索引号从0开始。
--切分形式可以是，相交或相接。
function IsZhongDianQieFen(bh1, bd1idx, bh2, bd2idx)--中点切分 字例：主
    --------计算好误差
	local bh1_bd=GetBDByBH(bh1,bd1idx)
	local bh2_bd=GetBDByBH(bh2,bd2idx)

    local midNode,midNodeIndex = BH2BHXiangJiaoDIAN(bh1_bd,bh2_bd)

    if(midNode == nil)then
	    local startpt,startindex = GetStartPoint(bh2_bd)
	    local endpt,endindex = GetEndPoint(bh2_bd)
	    local line0 = GetLine(startpt,endpt)
		midNode,midNodeIndex = GetNearestPt2Line(line0,bh1_bd)
    end

	local lefthalf=math.sqrt((bh1_bd.ptSet[1].x-midNode.x)*(bh1_bd.ptSet[1].x-midNode.x)+(bh1_bd.ptSet[1].y-midNode.y)*(bh1_bd.ptSet[1].y-midNode.y))
    local righthalf=math.sqrt((bh1_bd.ptSet[#bh1_bd.ptSet].x-midNode.x)*(bh1_bd.ptSet[#bh1_bd.ptSet].x-midNode.x)+(bh1_bd.ptSet[#bh1_bd.ptSet].y-midNode.y)*(bh1_bd.ptSet[#bh1_bd.ptSet].y-midNode.y))
    print"是否中点切分&&&&&&&&&&&&&"
    print(lefthalf,righthalf)

    -------准备好索引
    local startindexforreturn=0
	if(bd1idx == 0) then
	    startindexforreturn=0
	else
	    startindexforreturn=bh1.InflectionPoint[bd1idx]
	end
	local return_index=midNodeIndex+startindexforreturn
	local right_index=(1+#bh1_bd.ptSet)/2+startindexforreturn

	-------阈值评判
    if(lefthalf == 0 or righthalf == 0)then
	    local strokeNum =GetBHByPoint(bh1)
	    local StrokeAndPoint= {}
	    StrokeAndPoint[""..strokeNum]=""..return_index/#bh1.ptSet

		local location= {}
		location[""..strokeNum]=""..right_index/#bh1.ptSet

	    temp={}
	    temp["errortype"]="A0002"
	    temp["errorstroke"]=StrokeAndPoint
		temp["rightposition"]=location
	    temp["errordiff"]=100.0
	    typeInfo[#typeInfo+1]=temp
	    return false
	elseif(lefthalf/righthalf > 1.5 or righthalf/lefthalf > 1.5)then-- 1.5
		local strokeNum =GetBHByPoint(bh1)
		local StrokeAndPoint= {}
		StrokeAndPoint[""..strokeNum]=""..return_index/#bh1.ptSet

		local location= {}
		location[""..strokeNum]=""..right_index/#bh1.ptSet

		temp={}
		temp["errortype"]="A0002"
		temp["errorstroke"]=StrokeAndPoint
		temp["rightposition"]=location
		temp["errordiff"]=math.pow(math.abs((lefthalf-righthalf)/2.0) / ((lefthalf+righthalf)/2.0) , 0.65)
		typeInfo[#typeInfo+1]=temp
		return false
	else
		return true
	end
end

--部件侧面6：黄金分割（已经确认）
--黄金分割用于点和线之间，参数bd1和bd2必须是笔画，不能是笔段（bd1是被切割为两段的；bd2是用于提供切割点的；type1为0代表首点，为1代表末点, 为2代表交点）
--type2用于说明谁占0.618，0表示起端部分占0.618，1表示末端部分占0.618
function IsGoldenSection(bd1,bd2,type1,type2)--黄金分割 字例：下-------------------------需要对app动画做修改
	local flag = 0
	local index = 0
	local mindis = 512
	local tempdis = 0
	local yValue1 = 0
	local yValue2 = 0
	local startPoint,startindex=GetStartPoint(bd1)
	local endPoint,endindex=GetEndPoint(bd1)
	if(type1 == 0) then
		for i=1,#bd1.ptSet do
			local pt1 = bd1.ptSet[i]
			local pt2 = bd2.ptSet[1]
			local tempdis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
			if (tempdis < mindis)then
				mindis = tempdis
				index = i
			end
		end
	elseif(type1 == 1) then
		for i=1,#bd1.ptSet do
			local pt1 = bd1.ptSet[i]
			local pt2 = bd2.ptSet[#bd2.ptSet]
			local tempdis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
			if (tempdis < mindis)then
				mindis = tempdis
				index = i
			end
		end
	elseif(type1 == 2) then
		local midNode,midindex = BH2BHXiangJiaoDIAN(bd1,bd2)
		if(midNode == nil)then
			return true
		else
			index=midindex
		end
	else
		print"请输入type1类型："
	end

	local nearestPoint =bd1.ptSet[index]
	yValue1=math.sqrt(math.pow(startPoint.y-nearestPoint.y,2)+math.pow(startPoint.x-nearestPoint.x,2))
	yValue2=math.sqrt(math.pow(endPoint.y-nearestPoint.y,2)+math.pow(endPoint.x-nearestPoint.x,2))

	--------------------------------------------------------------------------------
	if(type2 == 0) then
		if(index/#bd1.ptSet >=0.41 and index/#bd1.ptSet <=0.75) then
			return true
		end
	elseif(type2 == 1) then
		if((#bd1.ptSet-index)/#bd1.ptSet >=0.41 and (#bd1.ptSet-index)/#bd1.ptSet <=0.75) then
			return true
		end
	end
	--------------------------------------------------------------------------------

	table.insert(errorType,"A0003")

	local strokeNum =GetBHByPoint(bd1)
	local StrokeAndPoint= {}
	StrokeAndPoint[""..strokeNum]=""..index/#bd1.ptSet

	local temp={}
	temp["errortype"]="A0003"
	temp["errorstroke"]=StrokeAndPoint

	if(type2 == 0) then
	    local location={}
		location[""..strokeNum]=""..(0.618)
	    temp["rightposition"]=location
		temp["errordiff"]=math.pow(math.abs(yValue1-(yValue1+yValue2)*0.618) / math.min(yValue1, yValue2),1.2)
	elseif(type2 == 1) then
	    local location={}
		location[""..strokeNum]=""..(0.382)
	    temp["rightposition"]=location
		temp["errordiff"]=math.pow(math.abs(yValue2-(yValue1+yValue2)*0.618) / math.min(yValue1, yValue2),1.2)
	end
	typeInfo[#typeInfo+1]=temp
	return false
end

------部件侧面7：首点居正（已经确认）
------首点居正的评判思路：bh1的尾点和bh2中笔段bh2bdidx的中点是竖直对齐的，即x坐标相等。
------要求：变量bh1和bh2是笔画；bh1是“点“；bh2bhidx是bh2上面的笔段索引。
function	IsShouDianJuZheng(bh1,bh2,bh2bdidx)--首点居正 字例：文 市
    -------计算端点的索引：
	local startindex2=-1
	local endindex2=-1
	if(bh2bdidx == 0) then
	    startindex2=1
	else
	    startindex2=bh2.InflectionPoint[bh2bdidx]+1
	end

	if(bh2bdidx == #bh2.InflectionPoint) then
	    endindex2 = #bh2.ptSet
	else
		endindex2 = bh2.InflectionPoint[bh2bdidx + 1]
	end
	local return_idx=(startindex2+endindex2)/2

    -------计算坐标点
    local bh2_bd=GetBDByBH(bh2,bh2bdidx)

	local end1,end1Idx = GetEndPoint(bh1)
	local mid2,mid2Idx = GetMidPoint(bh2_bd)
	local Dvalue = math.abs(end1.x - mid2.x)
	print"首点和第二笔中点的x坐标差值是%%%%%%%%%%"
	print(Dvalue)

	local start2, start2Idx = GetStartPoint(bh2_bd)
	local end2, end2Idx = GetEndPoint(bh2_bd)
	local len2=math.abs(end2.x-start2.x)

	--------阈值评判
	if(Dvalue/len2 <= 0.18)then--参数待修改
		return true
	else
		local strokeNum1 =GetBHByPoint(bh1)
		local strokeNum2 =GetBHByPoint(bh2)

		local StrokeAndPoint = {}
		StrokeAndPoint[""..strokeNum1]=""..end1Idx/#bh1.ptSet
		StrokeAndPoint[""..strokeNum2]=""..return_idx/#bh2.ptSet

		local temp={}
		temp["errortype"]="A0004"
		temp["errorstroke"]=StrokeAndPoint
		temp["errordiff"]=Dvalue/len2-0.05

		typeInfo[#typeInfo+1]=temp
		return false
	end
end

-----部件侧面8：突出主笔（已经确认）
-----突出主笔评判思路：主笔应该是一个部件中最长的横笔或竖笔。
-----参数heng，shu是笔画，不是笔段。它们的取值可以为nil
function	IsTuChuZhuBi(heng,shu)--突出主笔 字例：下 士
	if(heng ~= nil) then
		print"横可以传进来"
		--if (IsHeng(heng,1)==true) then
			--print"你这个横写得很好啊"
		local startpt,startindex = GetStartPoint(heng)
		local endpt,endindex = GetEndPoint(heng)
		local curve_var = 0
		local angel_var = 0
		local line = GetLine(startpt,endpt)
		local dis_var = GetBDLen(heng)
		local line_len = GetDistance(startpt,endpt)
		curve_var = dis_var / line_len
		print"横笔的曲率是%%%%%%%%%%"
		print(curve_var)
		local angel = 90
		if(startpt.x ~= endpt.x) then
			local slope = (endpt.y - startpt.y) / (endpt.x - startpt.x)
			angel = math.deg ( math.atan(slope))
		end
		angel_var = math.abs(angel)
		print"横笔的斜率角度是%%%%%%%%%%"
		print(angel_var)
		if(angel_var <= 14.86 and curve_var <= 1.1)then
			print"你这个横写得很好啊"
		else
			local strokeNum1 =GetBHByPoint(heng)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum1]="null"
			local temp={}
			temp["errortype"]="A0009"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=0.08
			typeInfo[#typeInfo+1]=temp
			return false
	   end
	end
	if(shu ~= nil) then
		print"竖可以传进来"
		--if (IsShu(shu,1)==true) then
			--print"你这个竖写得很好啊"
		local startpt,startindex = GetStartPoint(shu)
		local endpt,endindex = GetEndPoint(shu)
		local curve = 0
		local angel = 90
		local line = GetLine(startpt,endpt)
		local len1 = GetBDLen(shu)
		local dis = GetDistance(startpt,endpt)
		curve = len1 / dis
		print(len1)
		print(dis)
		print"竖笔的曲率是%%%%%%%%%%"
		print(curve)

		if (endpt.y ~= startpt.y) then
			local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
			angel = math.deg(math.atan(slope))
		end
		angel = math.abs(angel)
		print"竖的斜率角度是%%%%%%%%%%"
		print(angel)
		if(angel <= 12.7 and curve <= 1.12)then
			print"你这个竖写得很好啊"
			return true

		else
			local strokeNum2 =GetBHByPoint(shu)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum2]="null"
			local temp={}
			temp["errortype"]="A0009"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=0.08
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
end

-----部件侧面9：重心平稳（已经确认）
-----重心平稳评判思路：bh1和bh2是两个笔画(不是笔段)，它们应该相交或相接。将它们的交点或接点作为重心。
-----如果相接的话，bh1应该被bh2的首点分为两段.
function IsZhongXinPingWen(bh1,bh2)--重心平稳 字例：火
	local keyDotIndex=0
	local keyDot= WZEnv.POINT:new()
	if(BH2BHXiangJiao(bh1,bh2)== true) then
	    keyDot,keyDotIndex = BH2BHXiangJiaoDIAN(bh1,bh2)----注意：这里返回的是bh1上面的索引点
	else--计算第二笔首点与第一笔上哪个点最近
		local firstPoint = bh2.ptSet[1]
		local Dis = 1000
		for i = 1, #bh1.ptSet - 1 do
			local tempDis = GetDistance(bh1.ptSet[i],firstPoint)
			if (tempDis < Dis)then
				Dis = tempDis
				keyDot = bh1.ptSet[i]
				keyDotIndex = i
			end
		end
	end

	--------------------------------------------------
    --阈值评判
	local tempXmin,tempXmax,tempYmin,tempYmax=Box()
	print(tempXmin,tempXmax,tempYmin,tempYmax)
	local Xdiff=tempXmax-tempXmin
	local Ydiff=tempYmax-tempYmin
	local trueX = tempXmin + Xdiff/2

	local Dvalue = math.abs(trueX - keyDot.x)
	print"包围盒中点x坐标是*****"
	print(trueX)
	print"关键点x的坐标是*****"
	print(keyDot.x)
	print"包围盒中点和关键点中点的x坐标差值是%%%%%%%%%%"
	print(Dvalue)
	if(Dvalue/Xdiff <= 0.13)then--参数待修改
		return true
	else
		----------------------------------------------------------
		local strokeNum1 =GetBHByPoint(bh1)
		local StrokeAndPoint = {}
		StrokeAndPoint[""..strokeNum1]=""..keyDotIndex/#bh1.ptSet--是第二笔写错导致第一笔上的关键点不对
		local temp={}
		temp["errortype"]="A0007"
		temp["errorstroke"]=StrokeAndPoint
		temp["errordiff"]=Dvalue/Xdiff
		typeInfo[#typeInfo+1]=temp
		return false
		-----------------------------------------------------------
	end
end



--部件侧面10：长度比例（已经确认）
--长度比例评判思路：bh1的指定笔段/bh2的指定笔段>=ratio。  注意：笔段索引号从0开始。
function IsChangDuBiLi(bh1,bh1_bdindex,bh2,bh2_bdindex,ratio)--长度比例
    local bh1_bd=GetBDByBH(bh1,bh1_bdindex)
	local bh2_bd=GetBDByBH(bh2,bh2_bdindex)
	---------------------------------------
	local startindex1=-1
	local endindex1=-1
	local startindex2=-1
	local endindex2=-1
	--计算端点的索引：
	if(bh1_bdindex == 0) then
	    startindex1=1
	else
	    startindex1=bh1.InflectionPoint[bh1_bdindex]+1
	end

	if(bh1_bdindex == #bh1.InflectionPoint) then
	    endindex1 = #bh1.ptSet
	else
		endindex1 = bh1.InflectionPoint[bh1_bdindex + 1]
	end

	if(bh2_bdindex == 0) then
	    startindex2=1
	else
	    startindex2=bh2.InflectionPoint[bh2_bdindex]+1
	end

	if(bh2_bdindex == #bh2.InflectionPoint) then
	    endindex2 = #bh2.ptSet
	else
		endindex2 = bh2.InflectionPoint[bh2_bdindex + 1]
	end

	---------------------------------------
	--阈值评判
	local l1 = GetBDLen(bh1_bd)
	local l2 = GetBDLen(bh2_bd)
	local temp_ratio = math.abs(l1/l2)

	print("-----------------------------------changdubili--------------------------------")
	print("startindex1".." "..startindex1, "endindex1".." "..endindex1,"all".." "..#bh1.ptSet)
	print("startindex2".." "..startindex2, "endindex2".." "..endindex2,"all".." "..#bh2.ptSet)
	print("l1".." "..l1,"l2".." "..l2,"temp_ratio".." "..temp_ratio)
	print(#bh1.InflectionPoint,#bh1.InflectionPoint)
	print("-----------------------------------changdubili--------------------------------")

	if(temp_ratio >= ratio)then
		return true
	else
		local strokeNum1 =GetBHByPoint(bh1)
		local strokeNum2 =GetBHByPoint(bh2)
		local StrokeAndPoint = {}
		StrokeAndPoint[""..strokeNum1]=""..startindex1/#bh1.ptSet.."/"..endindex1/#bh1.ptSet
		StrokeAndPoint[""..strokeNum2]=""..startindex2/#bh2.ptSet.."/"..endindex2/#bh2.ptSet
		local temp={}
		temp["errortype"]="A0010"
		temp["errorstroke"]=StrokeAndPoint
		temp["errordiff"]=(ratio-temp_ratio)*0.9
		typeInfo[#typeInfo+1]=temp
		return false
	end
end

-------部件侧面11：中点竖直对齐，返回值与竖直平齐相同
-------HengBHtable表示所有笔画，bdtable表示各个笔画的笔段索引。起始笔段索引为0
function IsZhongDianShuZhiDuiQi(HengBHtable,bdtable)
    --------计算索引
    if(#HengBHtable ~= #bdtable)then
		return false
	end

	------计算各个点的索引
	local PTtable={}
    for i=1,#bdtable do
		local bh_bd=GetBDByBH(HengBHtable[i],bdtable[i])
		local mid,midIdx = GetMidPoint(bh_bd)

		local tempindex=0
		if(bdtable[i] == 0) then
			tempindex=0
		else
			tempindex=HengBHtable[i].InflectionPoint[bdtable[i]]
		end

		PTtable[i]=tempindex+midIdx
	end

	--------计算阈值
    for i = #HengBHtable,2,-1 do
		local bh2 = HengBHtable[i]--bh2
		local bh1 = HengBHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		print(i,index1,index2)
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.x-firpt.x)
		print"竖直平齐的差值%%%%%%%%%%%%"
		print(Dvalue)
		if(Dvalue <= 35)then--参数待修改
			print"IsShuZhiPingQiOKOK"
		else
			local strokeNum1 =GetBHByPoint(bh1)
			local strokeNum2 =GetBHByPoint(bh2)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum1]=""..index1/#bh1.ptSet
			StrokeAndPoint[""..strokeNum2]=""..index2/#bh2.ptSet
			temp={}
			temp["errortype"]="A0008"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=math.pow(Dvalue/220.0, 1.2)
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
	return true
end

-------部件侧面12：中点水平对齐，返回值与水平平齐相同
-------ShuBHtable表示所有笔画，bdtable表示各个笔画的笔段索引。起始笔段索引为0
function IsZhongDianShuiPingDuiQi(ShuBHtable,bdtable)
    --------计算索引
    if(#ShuBHtable ~= #bdtable)then
		return false
	end

	------计算各个点的索引
	local PTtable={}
    for i=1,#bdtable do
		local bh_bd=GetBDByBH(ShuBHtable[i],bdtable[i])
		local mid,midIdx = GetMidPoint(bh_bd)

		local tempindex=0
		if(bdtable[i] == 0) then
			tempindex=0
		else
			tempindex=ShuBHtable[i].InflectionPoint[bdtable[i]]
		end

		PTtable[i]=tempindex+midIdx
	end

	--------计算阈值
	for i = #ShuBHtable,2,-1 do
		local bh2 = ShuBHtable[i]--bh2
		local bh1 = ShuBHtable[i-1]--bh1
		local index2 = PTtable[i]
		local index1 = PTtable[i-1]
		print(i,index1,index2)
		local secpt = bh2.ptSet[index2]
		local firpt = bh1.ptSet[index1]
		local Dvalue= math.abs(secpt.y-firpt.y)
		print"水平平齐的差值%%%%%%%%%%%%"
		print(Dvalue)

		if(Dvalue <= 35)then--参数待修改
			print"IsShuiPingPingQiOKOK"
		else
			local strokeNum1 =GetBHByPoint(bh1)
			local strokeNum2 =GetBHByPoint(bh2)
			local StrokeAndPoint = {}
			StrokeAndPoint[""..strokeNum1]=""..index1/#bh1.ptSet
			StrokeAndPoint[""..strokeNum2]=""..index2/#bh2.ptSet
			temp={}
			temp["errortype"]="A0001"
			temp["errorstroke"]=StrokeAndPoint
			temp["errordiff"]=math.pow(Dvalue/220.0, 1.2)
			typeInfo[#typeInfo+1]=temp
			return false
		end
	end
	return true
end

--[[function JudgeDotLinePoint(pt,bd)
	--local newbh = resample(bd)
	local tempDis =  512
	local disThreshold = 50
	local startPoint,startindex=GetStartPoint(bd)
	local endPoint,endindex=GetEndPoint(bd)
	--local line1 = GetLine(startindex,endindex)
	--local near,nearIndex=GetNearestPt2Line(line1,pt)
	local yValue1=math.abs(startPoint.y-pt.y)
	local yValue2=math.abs(endPoint.y-pt.y)
	--print(startPoint.y,endPoint.y,pt.y)

	if(yValue1/yValue2 >= 0.5 and yValue1/yValue2 <= 0.7)then
		return true
	elseif(yValue2/yValue1 >= 0.5 and yValue2/yValue1 <= 0.7)then
		return true
	else
		table.insert(errorType,"A0003")
		return false
	end
end--]]

--------------------------------------------------------------------------------------------------整字的主观侧面评判规则------------------------------------------------------------------

--遍历点集的最上最下最左最右，形成字包围盒，算出宽高比
function IsAspectRatio(type)--字宽高比 1瘦高型 2矮胖型
	local strokeStrs  = {}
	local tempYmin =512
	local tempYmax =0
	local tempXmin =512
	local tempXmax =0
	--print"0000000"
	--print(pointstr)
	local ptSets = {}
	for strx,stry in string.gmatch(pointstr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSets[#ptSets+1] = pt
	end
	for i= 1,#ptSets do
		--print(ptSets[i].x,ptSets[i].y)
		if (ptSets[i].y > tempYmax) then
		tempYmax=ptSets[i].y
		end
		if (ptSets[i].y < tempYmin) then
		tempYmin=ptSets[i].y
		end
		if (ptSets[i].x> tempXmax) then
		tempXmax=ptSets[i].x
		end
		if (ptSets[i].x < tempXmin) then
		tempXmin=ptSets[i].x
		end
	end


	--print(tempXmin,tempXmax,tempYmin,tempYmax)
	local Xdiff=tempXmax-tempXmin
	local Ydiff=tempYmax-tempYmin
	if(tonumber(type) == 1)then
		print"PPPPPPPPPPPPPPPPP"
		print(Xdiff,Ydiff)
		if(Xdiff>Ydiff)then
			local StrokeAndPoint={ }
			StrokeAndPoint["null"]="null"
			--typeInfo.errortype[#typeInfo.errortype+1] = "A0000"
			--typeInfo.errorstroke[#typeInfo.errorstroke+1] = StrokeAndPoint
			local temp={}
			temp["errortype"]="C0001"
			temp["errorstroke"]=StrokeAndPoint
			print"kuangaobi00000000000000000000"
			typeInfo[#typeInfo+1]=temp
			return false
		else
			return true
		end
	elseif(tonumber(type) == 2)then
		if(Xdiff<Ydiff)then
			local StrokeAndPoint={ }
			StrokeAndPoint["null"]="null"
			--typeInfo.errortype[#typeInfo.errortype+1] = "A0000"
			--typeInfo.errorstroke[#typeInfo.errorstroke+1] = StrokeAndPoint
			local temp={}
			temp["errortype"]="C0001"
			temp["errorstroke"]=StrokeAndPoint
			print"kuangaobi00000000000000000000"
			typeInfo[#typeInfo+1]=temp
			return false
		else
			return true
		end
	elseif(tonumber(type) == 3)then
		if((Xdiff/Ydiff) > 1.3 or (Ydiff/Xdiff) > 1.3 )then
			local StrokeAndPoint={ }
			StrokeAndPoint["null"]="null"
			--typeInfo.errortype[#typeInfo.errortype+1] = "A0000"
			--typeInfo.errorstroke[#typeInfo.errorstroke+1] = StrokeAndPoint
			local temp={}
			temp["errortype"]="C0001"
			temp["errorstroke"]=StrokeAndPoint
			print"kuangaobi00000000000000000000"
			typeInfo[#typeInfo+1]=temp
			return false
		else
			return true
		end
	end
end
