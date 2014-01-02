-----------------------------------------
--封装手写汉字数据结构
--点 笔段 笔画 矩形框四种数据类型 POINT BD BH RECT
--TODO 弱引用模式设置成“V”
--TODO 手写字信息归一化: 点的密度进行调整 && 字的大小进行放缩
-----------------------------------------


local setmetatable = setmetatable
local pairs = pairs
local string = string
local tonumber = tonumber
local print = print
local math = math
local table =  table
local type = type

module("WriteZiInfo")
POINT = { GeoType = "KPOINT", x = 0, y = 0}
--ptSet { {x,y} {x,y} {x,y}…… }
BD = { GeoType = "KBD", ptSet = {} }
--BDSet {bd1 bd2 bd3}
--InflectionPoint 存放的是拐点的索引
BH = { GeoType = "KBH", ptSet = {}, InflectionPoint = {}, BDSet = {} }
--BHIdxSet {1,2,3}
RECT = { GeoType = "KRECT", BHIdxSet = {}, Edges = {left = -1, right = -1, top = -1,bottom = -1} }
--strokeStrings = {"x/y……","x/y……"}
--strokes = {bh1,bh2,bh3}
WriteHZ = { strokeNum = 0, strokes = {}, strokeStrings = {}  }

--##### 设定标准正字信息 begin#####--
local stdHZStruct = nil
function setStdHZ(stdHZ)
	stdHZStruct = stdHZ
end


--split 函数，sep是待切分字符串，sign为分割字符
function string:split(sep,sign)
	local sep, fields = sep or "\t", {}
	local pattern = string.format("([^"..sign.."]+)", sep)
	self:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end


--##### 设定标准正字信息 end#####--

--##### 点 begin#####--
function POINT:new(o)
	local o = o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end
--##### 点 end#####--

--##### 笔段 begin#####--
function BD:new(o)
	local o = o or {}
	o.ptSet = {}
	setmetatable(o,self)
	self.__index = self
	return o
end
--##### 笔段 end#####--

--##### 笔画begin#####--
function BH:new(o)
	local o = o or {}
	o.ptSet = {}
	o.InflectionPoint = {}
	o.BDSet = {}
	setmetatable(o,self)
	self.__index = self
	return o
end

--将点集字符串分割，并初始化笔画的点集
function BH:splitPoints(ptStr)
	local ptSet = {}
	for strx,stry in string.gmatch(ptStr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSet[#ptSet+1] = pt
	end
	self.ptSet = ptSet
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

function BH:getStrokeBDNum(bh)

	local n_step = 5			--lua索引从1开始
	local n_curIdx = n_step+1
	local n_preIdx = 1
	local n_postIdx = 2*n_step+1
	local angleArr = {}
	--计算点对应的角度
	for i = n_curIdx,(#self.ptSet)-n_step do
		local ele = {}
		local angle = self:calAngle(n_preIdx,i,n_postIdx)
		ele.pos = i
		ele.angle = angle
		table.insert(angleArr,ele)
		n_preIdx = n_preIdx+1
		n_postIdx= n_postIdx+1
	end
	--把角度按照从小到大排序
	table.sort(angleArr,sortingFun)

	local nCandidateNum = #angleArr;
	if(nCandidateNum == 0) then
		return 1
	end
	local CInflectionPts = {}		--存储候选拐点的索引


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

	if ( #CInflectionPts <= 1 ) then
		return #CInflectionPts+1
	end

	table.sort(CInflectionPts)

	return #CInflectionPts+1

end



--计算笔画拐点
function BH:CalInflectionPoint(BDNum)
	local n_step = 5			--lua索引从1开始
	if( #self.ptSet <2*n_step+1) then
		return
	end
	local n_curIdx = n_step+1
	local n_preIdx = 1
	local n_postIdx = 2*n_step+1
	local angleArr = {}
	--计算点对应的角度
	for i = n_curIdx,(#self.ptSet)-n_step do
		local ele = {}
		local angle = self:calAngle(n_preIdx,i,n_postIdx)
		ele.pos = i
		ele.angle = angle
		table.insert(angleArr,ele)
		n_preIdx = n_preIdx+1
		n_postIdx= n_postIdx+1
	end
	--把角度按照从小到大排序
	table.sort(angleArr,sortingFun)
	local nCandidateNum = (BDNum-1)*10;
	if(nCandidateNum > #angleArr ) then
		nCandidateNum = #angleArr
	end
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
			if(#CInflectionPts == BDNum+1) then
				break
			end
		end
	end

	--如果上一步CInflectionPts 选出的拐点数量小于(BDnum -1), then select again
	if( #CInflectionPts < BDNum-1) then
		for i = 1,#CInflectionPts do --	清空
			CInflectionPts[i] = nil
		end
		for p = 1,nCandidateNum do
			local cFlag = false
			for q = 1,#CInflectionPts do
				if( math.abs(angleArr[p].pos - CInflectionPts[q]) < n_step) then
					cFlag = true
				end
			end
			if (cFlag == false) then
				CInflectionPts[#CInflectionPts+1] = angleArr[p].pos
				if(#CInflectionPts == BDNum-1) then
					break
				end
			end
		end
	end
	--从CInflectionPts中继续选出最终拐点，
	--这样主要是为了屏蔽笔画尖端的抖动，减小拐点检测的误差
	local n_prePos
	local n_postPos
	table.sort(CInflectionPts)

	if(#CInflectionPts <= 0) then
		return
	end

	if(#CInflectionPts == BDNum-1) then
		n_prePos = 1
		n_postPos = #self.ptSet
		self.InflectionPoint = CInflectionPts
	elseif(#CInflectionPts == BDNum) then
		local pos1 = CInflectionPts[1]
		local pos2 = CInflectionPts[#CInflectionPts]
		local diff1 = pos1 - 1
		local diff2 = #self.ptSet - pos2
		if(diff1 > diff2) then
			n_prePos = 1
			n_postPos = CInflectionPts[#CInflectionPts]
			table.remove(CInflectionPts,#CInflectionPts)
		else
			n_prePos = CInflectionPts[1]
			n_postPos = #self.ptSet
			table.remove(CInflectionPts,1)
		end
		self.InflectionPoint = CInflectionPts
	elseif(#CInflectionPts == BDNum+1) then
		n_prePos = CInflectionPts[1]
		n_postPos = CInflectionPts[#CInflectionPts]
		table.remove(CInflectionPts,1)
		table.remove(CInflectionPts,#CInflectionPts)
		self.InflectionPoint = CInflectionPts
	else
		n_prePos = 1
		n_postPos = #self.ptSet
		self.InflectionPoint = CInflectionPts
	end
	--清除笔画两端可能的抖动点
	self:clearEndPoints(n_prePos,n_postPos)
end




--计算curIdx对应的角度
function BH:calAngle(preIdx, curIdx, postIdx)
	local prePt = self.ptSet[preIdx]
	local curPt = self.ptSet[curIdx]
	local postPt = self.ptSet[postIdx]
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

--清除笔画两端可能有的抖动
function BH:clearEndPoints(n_prePos, n_postPos)
	--先从ptset中清除，再移动拐点数组的位置
	if (n_prePos == 1 and n_postPos == #self.ptSet) then
		return
	end
	if n_postPos ~= #self.ptSet then
		local postPoint = self.ptSet[n_postPos]
		local lastPoint = self.ptSet[#self.ptSet]
		local postDis = math.sqrt(math.pow(lastPoint.x - postPoint.x,2) + math.pow(lastPoint.y - postPoint.y,2))
		if postDis < 15 then
		for i = #self.ptSet,n_postPos+1,-1 do
			table.remove(self.ptSet,i)
			end
		end
	end
	if n_prePos ~= 1 then
		local prePoint = self.ptSet[n_prePos]
		local firstPoint = self.ptSet[1]
		local preDis = math.sqrt(math.pow(prePoint.x - firstPoint.x,2) + math.pow(prePoint.y - firstPoint.y,2))
		if predis < 15 then
			for i = 1, n_prePos-1 do
			table.remove(self.ptSet,1)
			end
			--移动拐点索引数组中索引的位置
			for i = 1,#self.InflectionPoint do
				self.InflectionPoint[i] = self.InflectionPoint[i] - n_prePos+1
			end
		end
	end
end

--根据拐点信息构造笔段
function BH:initBDSet()
	local prepos = 1
	for i = 1, #self.InflectionPoint do
		local curpos = self.InflectionPoint[i]
		local bd = BD:new()
		for j = prepos, curpos do
			bd.ptSet[#bd.ptSet+1] = self.ptSet[j]
		end
		self.BDSet[#self.BDSet+1] = bd
		prepos = self.InflectionPoint[i]+1
	end
	local bd = BD:new()
	for i = prepos, #self.ptSet do
		bd.ptSet[#bd.ptSet+1] = self.ptSet[i]
	end
	self.BDSet[#self.BDSet+1] = bd
end
--##### 笔画 end#####--

--#####矩形框 begin#####--

--矩形框构造函数
function RECT:new(o)
	local o = o or {}
	o.BHIdxSet = {}
	o.Edges = {left = -1, right = -1, top = -1,bottom = -1}
	setmetatable(o,self)
	self.__index = self
	return o
end
--#####矩形框 end#####--




--#####手写字 begin#####--

function WriteHZ:new()
	local o = o or {}
	o.strokes = {}
	o.strokeStrings = {}
	setmetatable(o,self)
	self.__index = self
	return o
end




--------对手写字笔画应该先进行点密度的调整再进行归一化
function getDistance(pt1,pt2)
	local dis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
	return dis
end

--计算一个笔画的长度
function getLength(ptSet)
	local length = 0.0
	for i = 1,#ptSet-1 do
		length = length + getDistance(ptSet[i],ptSet[i+1])
	end
	return length
end

function isIntable(ele,list)
	for j = 1,#list do
		if(ele.x == list[j].x and ele.y == list[j].y) then
			return true
		end
	end
	return false
end

--切分字符串
function splitPoints(ptStr,ptSet)
	for strx,stry in string.gmatch(ptStr,"(%d+)/(%d+)") do
		local pt = {}
		pt.x = tonumber(strx)
		pt.y = tonumber(stry)
		ptSet[#ptSet+1] = pt
	end
end

--对于点的插值 ptSet 和 dstPts 是每一个笔画的点集
function adjustPtDensity(ptSet,dstPts)
	local nCount = getLength(ptSet)
	countInter,countFloat = math.modf (nCount)
	if (countFloat > 0.5) then
		nCount = countInter + 1
	else
		nCount = countInter
	end
	srcPts = {}
	for i = 1, #ptSet do
		srcPts[i] = ptSet[i]
	end
	local I  = 1
	local D = 0.0


	local xInter,xFloat = math.modf(srcPts[1].x)
	local yInter,yFloat = math.modf(srcPts[1].y)
	if ( xFloat > 0.5 ) then
		xInter = xInter + 1
	end
	if ( yFloat >0.5 ) then
		yInter = yInter + 1
	end
	dstPts[1] = {}
	dstPts[1].x = xInter
	dstPts[1].y = yInter

	local i = 2
	while (true) do
		local pt1 = srcPts[i-1]
		local pt2 = srcPts[i]
		local d = getDistance(pt1,pt2)
		if ( (D + d) >= I ) then
			local qx = pt1.x + ( (I - D)/d )*(pt2.x - pt1.x)
			local qy = pt1.y + ( (I - D)/d )*(pt2.y - pt1.y)
			local tmpPT = {}
			xInter,xFloat = math.modf (qx)
			yInter,yFloat = math.modf (qy)

			if (xFloat > 0.5) then
				xInter = xInter + 1
			end

			if (yFloat > 0.5) then
				yInter = yInter + 1
			end
			tmpPT.x = xInter
			tmpPT.y = yInter
			dstPts[#dstPts+1] = tmpPT
			if (isIntable(tmpPT,srcPts) == false) then
				table.insert(srcPts,i,tmpPT)
			end
			D = 0.0
		else
			D = D + d
		end
		i = i + 1
		if( i == #srcPts + 1) then
			break
		end
	end
end

	m_norWidth = 128
	m_norHeight = 128
	m_nLeftMargine = 0
	m_nUpMargine = 0

--计算整个字的放缩比例
function calZoomRatio (strokeStrsArr,zoomRatio,xOffset,yOffset)
	local n_minLeft = 0
	local n_maxRight = 0
	local n_minUp = 0
	local n_maxDown = 0
	local onestroke = {}
	for j = 1,#strokeStrsArr do
		onestroke = strokeStrsArr[j]
		local ptSet = {}
		splitPoints(onestroke,ptSet)

		for i =1,#ptSet do
			if (n_minLeft > ptSet[i].x) then
				n_minLeft = ptSet[i].x
			end

			if(n_maxRight < ptSet[i].x) then
				n_maxRight = ptSet[i].x
			end

			if(n_minUp > ptSet[i].y) then
				n_minUp = ptSet[i].y
			end

			if(n_maxDown < ptSet[i].y) then
				n_maxDown = ptSet[i].y
			end
		end
	end
	local n_preWidth = n_maxRight - n_minLeft
	local n_preHeight = n_maxDown - n_minUp
	if (n_preWidth == 0 and n_preHeight == 0) then
		zoomRatio = 1.0
		xOffset = m_norWidth/2  -n_minLeft
		yOffset = m_norHeight/2 - n_minUp
	elseif (n_preWidth > n_preHeight) then
		xOffset = -n_minLeft
		yOffset = (n_preWidth - n_preHeight) / 2 - n_minUp
		zoomRatio = m_norWidth / n_preWidth
	else
		xOffset = ( n_preHeight - n_preWidth )/2 - n_minLeft
		yOffset = -n_minUp
		zoomRatio =  m_norHeight /n_preHeight
	end
end


--归一化与插值
function WriteHZ:Normailize(strokeStrs)
	local zoomRatio = 1
	local xOffset = 0
	local yOffset = 0
	local strokeStrsArr = {}

	--midPtSet 是经过放缩后中间点集 每一笔的所有点集组成一个小table
	local midPtSet = {}

	for i = 1,#strokeStrs do
		strokeStrsArr[#strokeStrsArr+1] = strokeStrs[i]
	end

	--1.先进行整个字大小的放缩
	calZoomRatio(strokeStrsArr,zoomRatio,xOffset,yOffset)
	for i = 1 ,#strokeStrsArr do
		local onestrokePtSet = {}
		local ptset = {}
		splitPoints(strokeStrsArr[i],ptset)
		for j =1 ,#ptset do
			local tmpPT = {}
			tmpPT.x = ( ptset[j].x + xOffset )*zoomRatio + m_nLeftMargine + 0.5
			tmpPT.y = ( ptset[j].y + yOffset )*zoomRatio + m_nUpMargine + 0.5
			onestrokePtSet[#onestrokePtSet+1] = tmpPT
		end
		midPtSet[#midPtSet+1] = onestrokePtSet
	end
	--放缩完成后点的坐标存在了midPtSet中
	--2.点的插值
	local tmp = {}
	for i = 1, #midPtSet do
		local prePtSet = midPtSet[i]
		local onePtSet = {}
		adjustPtDensity(prePtSet,onePtSet)
		tmp[#tmp+1] = onePtSet
	end
	return tmp
end



--------对手写字笔画应该先进行点密度的调整再进行归一化

function WriteHZ:initialize(str)
	local strokeStrs = {}
	strokeStrs = str:split(str,"@")
	local dstStrokes = {}
	dstStrokes = self:Normailize(strokeStrs)
	self:initStrokeStrs(dstStrokes )
	self:initStrokeNum(#dstStrokes )
end

--初始化手写字笔画个数信息
function WriteHZ:initStrokeNum(strokeNum)
	self.strokeNum = strokeNum
end


--初始化手写字点集信息
function WriteHZ:initStrokeStrs( strokeStrs )
	local Strs = {}
	for i = 1,#strokeStrs do
		local onestroke = ""
		for j = 1,#strokeStrs[i] do
			onestroke = onestroke .. strokeStrs[i][j].x .. "/"..strokeStrs[i][j].y .. "/"
		end
		Strs[#Strs+1] = onestroke
	end

	for i = 1,#Strs do
		self.strokeStrings[#self.strokeStrings+1] = Strs[i]
	end
	self:initStrokes()
end


--初始化笔画信息
function WriteHZ:initStrokes()
	for i=1,#self.strokeStrings do
		local str = self.strokeStrings[i]
		local onestroke = BH:new()
		onestroke:splitPoints(str)

		--手写字笔段数目的读取不应该从标准字信息中取
		--local curStrokeBDNum = stdHZStruct:getStrokeBDNumByIdx(i)
		local curStrokeBDNum = onestroke:getStrokeBDNum(onestroke)
		--计算拐点和初始化笔段
		onestroke:CalInflectionPoint(curStrokeBDNum)
		onestroke:initBDSet()
		self.strokes[#self.strokes+1] = onestroke
	end
end



--[[
--初始化笔画信息
function WriteHZ:initStrokes()
	--tmpPTSet {(x,y),(x,y)...}
	local tmpPTSet = {}
	for i=1,#self.strokeStrings do
		local str = self.strokeStrings[i]
		local onestroke = BH:new()
		onestroke:splitPoints(str)
		for j=1,#onestroke.ptSet do
			tmpPTSet[#tmpPTSet+1] = onestroke.ptSet[j]
		end
	end
	normalize(tmpPTSet)
end

--信息清空

]]--
function WriteHZ:clearData()
	self.strokeNum = 0
	self.strokeStrings = {}
	self.strokes = {}
end

--#####手写字 end#####--
