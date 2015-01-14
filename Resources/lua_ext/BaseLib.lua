-----------------------------------------
--鍏冨嚱鏁板簱
--TODO杩斿洖瀛楃涓诧細鏈€缁堣繑鍥炰竴涓瓧绗︿覆锛岀粰鐣岄潰灞備娇鐢?
--TODO 鍘熷嚱鏁板紓甯稿鐞嗭紝鍖呮嫭绫诲瀷鍒ゆ柇锛岃竟鐣屽垽鏂瓑绛?
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

--浠ｇ爜杩愯缁撴灉杩斿洖淇℃伅
allInfoStr = ""
retInfoStr = ""

--#####杩愯鐜璁剧疆鍑芥暟#####--
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
--#####鍏冨嚱鏁板疄鐜?####--
--鎵€鏈夋搷浣滅储寮曚粠0寮€濮?
function GetBH( idx )
	local bh = WriteHZ.strokes[idx + 1]
	return bh
end

--娉ㄦ剰涓嶨etBH鐨勫尯鍒紝杩欓噷鍙栧緱鐨勬槸鏍囧噯瀛楃殑绗旂敾
function GetPreBH(idx)
	local bh = StdHZ.strokes[idx + 1]
	return bh
end

--鍙傛暟涓篵d鎴朾h閮藉彲
function GetStartPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[1].x
	point.y = bh.ptSet[1].y
	return point ,1
end

function GetEndPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[#bh.ptSet].x
	point.y = bh.ptSet[#bh.ptSet].y
	return point ,#bh.ptSet
end

--鑾峰緱涓偣
function	GetMidPoint ( bh )
 	local point = WZEnv.POINT:new()
	local len= #bh.ptSet
	local tmpIdx = math.floor ( len / 2 )
	point.x = bh.ptSet[tmpIdx].x
	point.y = bh.ptSet[tmpIdx].y
	print(len,tmpIdx)
	return point,tmpIdx
end

function	GetMidIndex( bd )
 	local point = WZEnv.POINT:new()
	local len= #bd.ptSet
	local tmpIdx = math.floor ( len / 2 )
	return tmpIdx
end



--鑾峰緱鐐圭殑妯旱鍧愭爣
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


--璁＄畻curIdx瀵瑰簲鐨勮搴?
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

--鑾峰緱鎷愮偣锛屾殏鏈敤鍒?
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


---------------------------------鍘婚櫎鎶栧姩-----------------------------------------------
--濡傛灉閫夊嚭鐨勬嫄鐐规暟鐩负0锛岀洿鎺ヨ繑鍥烇紝姝ゆ椂CInflectionPts鏄┖鐨?
	if(#turning_ind <= 0) then
		return 0
	end
	--濡傛灉鎷愮偣鏁扮洰姣旀爣鍑嗗皯锛屽皢鎷愮偣鏁扮粍璧嬪€硷紝杩斿洖
	if (#turning_ind < BDNum - 1) then
		bh.InflectionPoint = turning_ind
		return #turning_ind;
	end


	--濡傛灉鎷愮偣鏁扮洰澶т簬鎴栬€呯瓑浜庢爣鍑嗗€?
	if (#turning_ind >= BDNum-1) then
		--濡傛灉澶氬嚭涓€涓嫄鐐癸紝浼嫄鐐逛竴瀹氭槸鍦ㄧ涓€涓垨鑰呮渶鍚庝竴涓紝鍒嗗埆璁＄畻鍏惰窛绂婚浣嶇偣鐨勮窛绂?
		local pos1 = turning_ind[1]
		local pos2 = turning_ind[#turning_ind]
		local spt = bh.ptSet[1]
		local ept = bh.ptSet[#bh.ptSet]
		local dis1 = math.sqrt( math.pow(bh.ptSet[pos1].x - spt.x,2) + math.pow(bh.ptSet[pos1].y - spt.y,2))
		local dis2 = math.sqrt( math.pow(bh.ptSet[pos2].x - ept.x,2) + math.pow(bh.ptSet[pos2].y - ept.y,2))
		--1.濡傛灉绗竴涓嫄鐐规槸鎶栧姩鐐?
		if( dis1 < 30 ) then
			n_prePos = turning_ind[1]
			n_postPos = #bh.ptSet
			--鎶婃姈绗旀浠庣瑪鐢讳腑鍒犻櫎
			for i = 1, n_prePos-1 do
				table.remove(bh.ptSet,1)
			end
			--浠庢嫄鐐规暟缁勪腑鍒犻櫎璇ユ嫄鐐?
			table.remove(turning_ind,1)
			--绉诲姩鎷愮偣绱㈠紩鏁扮粍涓储寮曠殑浣嶇疆
			if ( #turning_ind > 0 ) then
				for i = 1,#turning_ind do
					 turning_ind[i] = turning_ind[i] - n_prePos + 1
				end
			end
		end
		--2.濡傛灉鏈€鍚庝竴涓嫄鐐规槸鎶栧姩鐐?
		if (dis2 < 30) then
			n_prePos = 1
			n_postPos = turning_ind[#turning_ind]
			--浠庢嫄鐐硅〃鏍间腑鍒犻櫎璇ユ嫄鐐?
			table.remove(turning_ind,#turning_ind)
			--鍒犻櫎鎶栫瑪娈?
			for i = #bh.ptSet,n_postPos+1,-1 do
				table.remove(bh.ptSet,i)
			end
		end

	--璁＄畻鎷愮偣涔嬮棿鐨勮窛绂?
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
		print ("鎷愮偣涓暟涓?..")
		print (#turning_ind)
		print ("test over")
		return #turning_ind
	end
end




--[[
--鑾峰緱绗旂敾鎷愮偣鐨勪釜鏁?
function GetTurningPtNum(bh,BDNum)
	local n_step = 5			--lua绱㈠紩浠?寮€濮?
	if( #bh.ptSet <2*n_step+1) then
		return nil
	end
	local n_curIdx = n_step+1
	local n_preIdx = 1
	local n_postIdx = 2*n_step+1
	local angleArr = {}
	--璁＄畻鐐瑰搴旂殑瑙掑害
	for i = n_curIdx,(#bh.ptSet)-n_step do
		local ele = {}
		local angle = Cal_Angle(bh.ptSet[n_preIdx],bh.ptSet[i],bh.ptSet[n_postIdx])
		ele.pos = i
		ele.angle = angle
		table.insert(angleArr,ele)
		n_preIdx = n_preIdx + 1
		n_postIdx = n_postIdx + 1
	end
	--鎶婅搴︽寜鐓т粠灏忓埌澶ф帓搴?
	table.sort(angleArr,sortingFun)
	local nCandidateNum = #angleArr

	local CInflectionPts = {}		--瀛樺偍鍊欓€夋嫄鐐圭殑绱㈠紩
	--閫夊嚭浜嗘渶澶欱DNum+1涓嫄鐐癸紝瀛樺偍鍦–InflectionPts涓?
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

	--浠嶤InflectionPts涓户缁€夊嚭鏈€缁堟嫄鐐癸紝
	--杩欐牱涓昏鏄负浜嗗睆钄界瑪鐢诲皷绔殑鎶栧姩锛屽噺灏忔嫄鐐规娴嬬殑璇樊
	local n_prePos
	local n_postPos
	table.sort(CInflectionPts)

--濡傛灉閫夊嚭鐨勬嫄鐐规暟鐩负0锛岀洿鎺ヨ繑鍥烇紝姝ゆ椂CInflectionPts鏄┖鐨?
	if(#CInflectionPts <= 0) then
		return 0
	end
	--濡傛灉鎷愮偣鏁扮洰姣旀爣鍑嗗皯锛屽皢鎷愮偣鏁扮粍璧嬪€硷紝杩斿洖
	if (#CInflectionPts < BDNum - 1) then
		bh.InflectionPoint = CInflectionPts
		return #CInflectionPts
	end

   --濡傛灉鎷愮偣鏁扮洰澶т簬鎴栬€呯瓑浜庢爣鍑嗗€?
	if (#CInflectionPts >= BDNum-1) then
		--濡傛灉澶氬嚭涓€涓嫄鐐癸紝浼嫄鐐逛竴瀹氭槸鍦ㄧ涓€涓垨鑰呮渶鍚庝竴涓紝鍒嗗埆璁＄畻鍏惰窛绂婚浣嶇偣鐨勮窛绂?
		local pos1 = CInflectionPts[1]
		local pos2 = CInflectionPts[#CInflectionPts]
		local spt = bh.ptSet[1]
		local ept = bh.ptSet[#bh.ptSet]
		local dis1 = math.sqrt( math.pow(bh.ptSet[pos1].x - spt.x,2) + math.pow(bh.ptSet[pos1].y - spt.y,2))
		local dis2 = math.sqrt( math.pow(bh.ptSet[pos2].x - ept.x,2) + math.pow(bh.ptSet[pos2].y - ept.y,2))
		--1.濡傛灉绗竴涓嫄鐐规槸鎶栧姩鐐?
		if( dis1 < 5 ) then
			n_prePos = CInflectionPts[1]
			n_postPos = #bh.ptSet
			--鎶婃姈绗旀浠庣瑪鐢讳腑鍒犻櫎
			for i = 1, n_prePos-1 do
				table.remove(bh.ptSet,1)
			end

			--浠庢嫄鐐规暟缁勪腑鍒犻櫎璇ユ嫄鐐?
			table.remove(CInflectionPts,1)

			--绉诲姩鎷愮偣绱㈠紩鏁扮粍涓储寮曠殑浣嶇疆
			if ( #CInflectionPts > 0 ) then
				for i = 1,#CInflectionPts do
					 CInflectionPts[i] = CInflectionPts[i] - n_prePos + 1
				end
			end
		end
		--2.濡傛灉鏈€鍚庝竴涓嫄鐐规槸鎶栧姩鐐?
		if (dis2 < 5) then
			n_prePos = 1
			n_postPos = CInflectionPts[#CInflectionPts]
			--浠庢嫄鐐硅〃鏍间腑鍒犻櫎璇ユ嫄鐐?
			table.remove(CInflectionPts,#CInflectionPts)
			--鍒犻櫎鎶栫瑪娈?
			for i = #bh.ptSet,n_postPos+1,-1 do
				table.remove(bh.ptSet,i)
			end
		end
		bh.InflectionPoint = CInflectionPts
		return #CInflectionPts
	end
end
]]--


--鑾峰緱绗旂敾鐨勬嫄鐐?绱㈠紩浠?寮€濮?
function GetTurningPt(bh,index)
	local ptIdx = bh.InflectionPoint[index + 1]
	return bh.ptSet[ptIdx]
end

 --鑾峰緱涓ょ偣闂磋窛绂?
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



--鑾峰緱缁忚繃鐐筽t1 pt2鐨勭洿绾挎柟绋媋x+by+c = 0 杩斿洖鏁扮粍linevar涓緷娆℃槸绯绘暟a b c
function GetLine(pt1,pt2)
	local linevar = {}
	linevar[1] = pt2.y - pt1.y
	linevar[2] = pt1.x - pt2.x
	linevar[3] = pt1.y*pt2.x - pt2.y*pt1.x
	return linevar
end

--鍒ゆ柇鐐规槸鍚﹀湪鐩寸嚎涓嬭竟
function Point2LineDown(pt,line)
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result < 0) then
		return true
	end
	return false
end

--鍒ゆ柇鐐规槸鍚﹀湪鐩寸嚎鐨勪笂杈?
function Point2LineUp(pt,line)
	local a,b,c = line[1],line[2],line[3]
	local result = a*pt.x + b*pt.y + c
	if (result > 0) then
		return true
	end
	return false
end


--判断点是否在直线的右边
function Point2LineRight(pt,line)
	local a,b,c = line[1],line[2],line[3]
	--line: ax + by + c = 0
	local x = pt.x
	local y = pt.y
	--如果直线是平行于X轴的，那么判断点在直线的左边还是右边
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


--鑾峰緱鏈€宸﹂潰鐨勭偣 bh/bd
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

--鑾峰緱鏈€鍙抽潰鐨勭偣 bh/bd
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

--鑾峰緱鏈€涓婃柟鐨勭偣
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

--鑾峰緱鏈€涓嬫柟鐨勭偣 bd/bd
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

--鑾峰緱bh涓婄pt鏈€杩戠殑鐐?
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


--鑾峰緱涓や釜绗旂敾鐨勪氦鐐? 濡傛灉鏈変氦鐐?杩斿洖璇ヤ氦鐐?
--							  濡傛灉娌℃湁浜ょ偣  杩斿洖nil
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



--pt 鍒癮x + by + c = 0鐨勮窛绂?
function Cal_Point2LineDis( pt, a, b, c)
	local x = pt.x
	local y = pt.y
	local m = math.abs(a*x+b*y+c)
	local n = 	math.sqrt(a*a+b*b)
	local dis = m/n
	return dis
end

--鍒ゆ柇pt鍦ㄧ洿绾縜x+by+c=0鐨勬柟浣?
function Cal_Direction(pt, a, b,c)
	local x = pt.x
	local y = pt.y
	local dir = a*x+b*y+c
	return dir
end


--鑾峰緱绗旂敾鍒扮洿绾?line ax + by + c = 0璺濈鏈€杩滅殑鐐?
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


--鑾峰緱绗旂敾鍒扮洿绾縧ine  ax + by + c = 0鐨?
function GetBHTrend(bh,line)
	local a,b,c = line[1],line[2],line[3]
	local trendArray = {}
	trendArray[#trendArray + 1 ] = 0   --绗竴涓猼ag鍒濆鍖栦负0
	local preDis = 0
	--1 琛ㄧず鍚庨潰鐨?
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


--杩斿洖bh鐨刾tSet涓紝绱㈠紩涓簆reIdx鍜宲ostIdx涔嬮棿鐨勭偣闆嗙粍鎴愮殑绗旀
function GetTempBD(bh,preIdx,postIdx)
	local bd = WZEnv.BD:new()
	for i = preIdx, postIdx do
		bd.ptSet[#bd.ptSet + 1] = bh.ptSet[i]
	end
	return bd
end


--浼犲叆鐨勫弬鏁颁负褰撳墠绗旂敾鐨勭储寮?绗旂敾绱㈠紩鍧囨槸浠?寮€濮?
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
		if (k1 == k2) then		--濡傛灉涓ょ嚎骞宠
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

		local vp = {}  --vp鏄袱涓洿绾挎鐨勪氦鐐?
		local k1,c1
		local k2,c2
		if (flag1 == 1 and flag2 == 1)  then --濡傛灉涓や釜鐩寸嚎娈甸兘骞宠浜巠杞?
			return false
		elseif(flag1 == 1 ) then --濡傛灉鐩寸嚎娈?骞宠浜巠杞?
			vp.x = pt11.x
			k2 = (pt22.y - pt21.y) /(pt22.x - pt21.x)
			c2 = pt21.y - pt21.x*k2
			vp.y = k2*(vp.x) + c2
		elseif(flag2 == 1) then --濡傛灉鐩寸嚎娈?骞宠浜巠杞?
			vp.x = pt21.x
			k1 = (pt12.y - pt11.y) /(pt12.x - pt11.x)
			c1 = pt11.y - pt11.x*k1
			vp.y = k1*(vp.x) + c1
		else  --濡傛灉涓や釜鐩寸嚎娈靛潎涓嶅钩琛屼簬y杞?
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
--瀵逛簬鍗曠瑪娈垫潵璇达紝鍙渶瑕佷紶鍏ョ瑪鐢诲嵆鍙?
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
	local loose_angel_max_threshold = 17
	local tight_angel_max_threshold = 10


	local loose_curve_max_threshold = 1.052
	local tight_curve_max_threshold = 1.035

	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	if (startpt.y >= endpt.y) then
		return false
	end

	local line = GetLine(startpt,endpt)
	local len = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len / dis
	local angel = 90
	if (endpt.y ~= startpt.y) then
		local slope = (endpt.x -  startpt.x)/(endpt.y - startpt.y)
		angel = math.deg(math.atan(slope))
	end
	angel = math.abs(angel)


 if (bl == 1) then
	if (curve <= loose_curve_max_threshold and angel <= loose_angel_max_threshold) then
		return true
	end

		return false
 end

 if (bl == 2) then
   if (curve <= tight_curve_max_threshold and angel <= tight_angel_max_threshold) then
		return true
	end
		return false
 end

end



function  IsHengZhe(bh,bl)
	if (bl ~= 1 and bl~=2) then return end

	local bd0_loose_angel_max_threshold = 20
	local bd0_tight_angel_max_threshold = 15

	local bd1_loose_angel_max_threshold = 40
	local bd1_tight_angel_max_threshold = 11

	local bd1_loose_curve_max_threshold = 1.25
	local bd1_tight_curve_max_threshold = 1.027

	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	if (startpt.x >= endpt.x or startpt.y >= endpt.y) then
		return false
	end

	local line = GetLine(startpt,endpt)
	local turning_pt,turning_index = GetFarthestPt2Line(bh,line)
	if(Point2LineUp(turning_pt,line) == false) then
		return false
	else
		local bd0 = GetTempBD(bh,startindex,turning_index)
		local line0 = GetLine(startpt,turning_pt)
		local len_bd0 = GetBDLen(bd0)
		local dis_bd0 = GetDistance(startpt,turning_pt)
		local curve0 = len_bd0 / dis_bd0

		local bd1 = GetTempBD(bh,turning_index,endindex)
		local line1 = GetLine(turning_pt,endpt)
		local len_bd1 = GetBDLen(bd1)
		local dis_bd1 = GetDistance(turning_pt,endpt)
		local curve1 = len_bd1 / dis_bd1

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

	if(curve1 > 1.027 and angel0 > 16) then
		return false
	end
	if(curve1 > 1.027 and angel0 <= 16 and angel1 > 35) then
		return false
	end
	angel1 = math.abs(angel1)
	angel0 = math.abs(angel0)
	if (bl == 1) then
		if (curve1 < 1.027 and angel1 < 40)then
			return true
		elseif(curve1 >= 1.027 and curve1 < 1.25 and angel0 < 16 and angel1 < 35) then
			return true
		else
			return false
		end
	end

	if (bl == 2) then
		if (angel0 < 15 and angel1 < 12 and curve1 < 1.03) then
			return true
		else
			return false
		end
	end
end
end

function  IsHeng(bh,bl)
	if (bl~= 1 and bl~=2) then return end
	local loose_curve_max_threshold = 1.1
	local tight_curve_max_threshold = 1.08

	local loose_angel_max_threshold = 20
	local tight_angel_max_threshold = 10


	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	 if (startpt.x >= endpt.x) then
		return false
	 end
	local line = GetLine(startpt,endpt)

	local len_var = GetBDLen(bh)
	local dis_var = GetDistance(startpt,endpt)
	local curve = len_var / dis_var

	local angel = 90
		if(startpt.x ~= endpt.x) then
			local slope = (endpt.y - startpt.y) / (endpt.x - startpt.x)
			angel = math.deg ( math.atan(slope))
	end

	local angel_var = math.abs(angel)


 if (bl == 1) then
	if (angel_var <= loose_angel_max_threshold  and curve <= loose_curve_max_threshold) then
		return true
	else
		return false
	end
 end

 if (bl == 2) then
	if (angel_var <= tight_angel_max_threshold  and curve <= tight_curve_max_threshold) then
		return true
	else
		return false
	end
 end

end


function IsHengZheXieGou(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_1,turning_index_1 = GetBottomMostPoint(bh)

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

	local cmpPt = {}
	cmpPt.x = n_maxRight
	cmpPt.y = n_minUp
	local tmpDis = 512
	local turning_pt_0 = {}
	local turning_index_0 = 1

	for i = 1, turning_index_1 do
		local curPt = bh.ptSet[i]
		local curDis = GetDistance(curPt,cmpPt)
		if(curDis < tmpDis) then
			tmpDis = curDis
			turning_index_0 = i
			turning_pt_0 = bh.ptSet[i]
		end
	end
	local line0 = GetLine(startpt,turning_pt_1)
	local tmpBD = GetTempBD(bh,startindex,turning_index_1)
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
	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end

	if (Point2LineUp(turning_pt_0,line0) == false) then
		return false
	end

	if (Point2LineDown(fpt,line1) == false) then
		return false
	end

	tmpDis = 512
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
	angel0 = math.abs(angel0)

	local angel1 = 90
	if (turning_pt_1.y ~= turning_pt_0.y) then
		local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
		angel1 = math.deg ( math.atan(slope1))
	end

	if(angel1 < -4 or wanqu0 > 1.038 or angel0 > 15) then
		return false
	end
	if (bl == 1) then
		return true
	end

	if(bl == 2) then
		if(angel1 > -4 and angel0 < 15) then
			return true
		end
		if(angel0 < 15 and wanqu0 < 1.03) then
			return true
		end
	end
	return false
end

function IsHengZheZhePie(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
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
	--�жϸõ��ǵ�һ���յ㻹�����һ���յ�
	local tmpBD1 = GetTempBD(bh,1,findex)
	local BD1_len = GetBDLen(tmpBD1)
	local BD1_dis = GetDistance(startpt,fpt)
	local BD1_curve = BD1_len / BD1_dis


	--�����������������Ҫ�Ĺյ�turning_pt_1
	local turning_pt_0 = {}
	local turning_pt0_index = 1

	local turning_pt_1 = {}
	local turning_pt1_index = 1

	local turning_pt_2 = {}
	local turning_pt2_index = 1

	local loop_start_index = 1
	local loop_end_index = 1

	if (BD1_curve > 1.2) then
	-- ˵���ùյ������һ���յ�
		loop_start_index = 1
		loop_end_index = findex - 5
		if(loop_end_index < 1) then
			return false
		end
		turning_pt2_index = findex
		turning_pt_2 = bh.ptSet[findex]
	else
		loop_start_index = findex + 5
		if(loop_start_index > endindex) then
			return false
		end
		loop_end_index = endindex
		turning_pt0_index = findex
		turning_pt_0 = bh.ptSet[findex]
	end
	local beginIdx = 1
	local beginPt= bh.ptSet[1]
	local tmpAngle = 90

	for i = loop_start_index , loop_end_index  do
		local curPt = bh.ptSet[i]
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
		local line1 = GetLine(turning_pt_1,endpt)
		tmpDis = 0
		for i = turning_pt1_index,endindex do
			local curPt = bh.ptSet[i]
			local curDis = Cal_Point2LineDis(curPt,line1[1],line1[2],line1[3])
			if(curDis > tmpDis) then
				tmpDis = curDis
				turning_pt2_index = i
				turning_pt_2 = bh.ptSet[i]
			end
		end
	end
	if (turning_pt2_index == 1 or turning_pt2_index == endindex) then
			return false
		end
	if (turning_pt_0.x <= turning_pt_1.x or turning_pt_0.y >= turning_pt_1.y
	or turning_pt_1.x >= turning_pt_2.x
	or turning_pt_2.x <= endpt.x or turning_pt_2.y >= endpt.y) then
		return false
	else
		local bd0 = GetTempBD(bh,1,turning_pt0_index)
		local bd1 = GetTempBD(bh,turning_pt0_index,turning_pt1_index)
		local bd2 = GetTempBD(bh,turning_pt1_index,turning_pt2_index)
		local bd3 = GetTempBD(bh,turning_pt2_index,endindex)

		--bd0 �Ĳ������� ƫ��ˮƽ����ĽǶ� len/dis
		local angel0 = 90
		if (turning_pt_0.x ~= startpt.x) then
			local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
			angel0 = math.deg(math.atan(slope0))
		end
		angel0 = math.abs(angel0)
		bd0_dis = GetDistance(startpt,turning_pt_0)
		bd0_len = GetBDLen(bd0)
		local wanqu0= bd0_len / bd0_dis

		--bd1�Ĳ�������
		local angel1 = 90
		if (turning_pt_1.y ~= turning_pt_0.y) then
			local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
			angel1 = math.deg ( math.atan(slope1))
		end
		angel1 = math.abs(angel1)
		bd1_dis = GetDistance(turning_pt_0,turning_pt_1)
		bd1_len = GetBDLen(bd1)
		local wanqu1 = bd1_len / bd1_dis

		-- bd3�Ĳ�������
		local angel2 = 90
		if (turning_pt_2.x ~= turning_pt_1.x) then
			local slope2 = (turning_pt_2.y -  turning_pt_1.y)/(turning_pt_2.x - turning_pt_1.x)
			angel2 = math.deg(math.atan(slope2))
		end
		angel2 = math.abs(angel2)


		--bd4�Ĳ�������
		local angel3 = 90
		if (endpt.y ~= turning_pt_2.y) then
			local slope3 = (endpt.x - turning_pt_2.x) / (endpt.y - turning_pt_2.y)
			angel3 = math.deg ( math.atan(slope3))
		end
		angel3 = math.abs(angel3)

		if (angel0  > 19) then
			return false
		end
		if (bl == 1) then
			if (angel0 <= 19) then
				return true
			end
		end
		if(bl == 2) then
			if(angel0 < 19 and wanqu1 < 1.059 and angel1 > 24) then
				return true
			end
			if(angel0 < 19 and wanqu1 < 1.059 and angel1 < 24 and angel3 > 44)then
				return true
			end
		end
		return false
	end
end


function  IsPie(bh,bl)
	return true
end

function  IsDian(bh,bl)
	return true
end

function  IsNa(bh,bl)
	return true
end

