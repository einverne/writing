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
	print(pt1.x,pt1.y)
	print(pt2.x,pt2.y)
	print(linevar[1])
	print(linevar[2])
	print(linevar[3])
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
	local n = math.sqrt(a*a+b*b)
	--print(m,n)
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


--四个角度的计算
function GetXAngel(spt,ept)
	local angel = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		angel = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))
		angel = math.abs(angel)
	end
	return angel
end
function GetXAngel2(spt,ept)
	local angel = 0
	if (math.abs(spt.x - ept.x)>0.0001 ) then
		angel = math.deg(math.atan( (ept.y- spt.y) / (ept.x - spt.x)))
		--angel = math.abs(angel)
	end
	return angel
end

function GetYAngel(spt,ept)
	local angel = 0
	if (math.abs(spt.y - ept.y)>0.0001 ) then
		angel = math.deg(math.atan( (ept.x- spt.x) / (ept.y - spt.y)))
		angel = math.abs(angel)
	end
	return angel
end
function GetYAngel2(spt,ept)
	local angel = 0
	if (math.abs(spt.y - ept.y)>0.0001 ) then
		angel = math.deg(math.atan( (ept.x- spt.x) / (ept.y - spt.y)))
		--angel = math.abs(angel)
	end
	return angel
end

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
print("shu is ok")
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local curve = 0
	local angel = 90
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

	if(angel > 17.6)then
		return false
	elseif(angel > 4.4 and angel <= 12.7 and curve> 1.12)then
		return false
	end

	if(bl == 1)then
		if(angel > 12.7 and angel <= 17.6)then
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

function  IsHeng(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local curve_var = 0
	local angel_var = 0
	if(startpt.x >= endpt.x) then
			return false
	else
		local line = GetLine(startpt,endpt)
	--	local dis_var = GetFarDis2Line(bh,line)
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

	if(angel_var > 15.3)  then
		return false
	end
	if(angel_var <= 9.86 and curve_var > 1.1) then
		return false
	end


	if (bl == 1 )then
		if(angel_var > 9.86 and angel_var <= 15.3)then
			return true
		elseif(angel_var <= 9.86 and curve_var <= 1.1)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel_var <= 9.86 and curve_var <= 1.1)then
			return true
		else
			return false
		end

	end
end
function IsHengZhe(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= endpt.y ) then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	--[[if (turning_pt_0.x ~= startpt.x) then
		local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
		angel0 = math.deg(math.atan(slope0))
	end
	angel0 = math.abs(angel0)--]]
	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (endpt.y ~= turning_pt_0.y) then
		 angel1 = GetYAngel(turning_pt_0,endpt);
	end
	if(angel0 > 21.6 )then
		return false
	elseif(angel0 <= 9.95 and angel1 <= 13.7 and wanqu1 > 1.08)then
		return false
	end

	if(bl == 1)then
		if(angel0 > 9.95 and angel0 <= 21.6 )then
			return true
		elseif(angel0 <= 9.95 and angel1 > 13.7)then
			return true
		elseif(angel0 <= 9.95 and angel1 <= 13.7 and wanqu1 <= 1.08)then
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel0 <= 9.95 and angel1 <= 13.7 and wanqu1 <= 1.08)then
			return true
		else
			return false
		end
	end
end
function IsHengZhe2(bh,bl)--横折折比较弯
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line = GetLine(startpt,endpt)
	local turning_pt_0,turning_index_0 = GetFarthestPt2Line(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

	if (len_bd0 == 0 or len_bd1 == 0 ) then
		return false
	end

	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= endpt.y ) then
		return false
	end

	if (startpt.x >= endpt.x or startpt.y >= endpt.y ) then
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
		 angel1 = GetYAngel(turning_pt_0,endpt);
	end
	if(angel0 > 21.6)then
		return false
	elseif(angel0 > 12.7 and angel0 <= 21.6 and angel1 > 17.3)then
		return false
	end

	if(bl == 1)then
		if(angel0 > 12.7 and angel0 <= 21.6 and angel1 <= 17.3)then
			return true
		elseif(angel0 <= 12.7)then
			return true
		else
			return false
		end
	end

	if(bl == 2 )then
		if(angel0 <= 12.7 and wanqu1 > 1.03 and wanqu0 <= 1.01)then
			return true
		elseif(angel0 <= 12.7 and wanqu1 <= 1.03)then
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

	--[[local n_minLeft,n_maxRight,n_minUp,n_maxDown = 512,0,512,0
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
	end--]]

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
		-- if(angel1 > 3.89 and angel0 > 4.55 and angel0 <= 9.74 and wanqu1 > 1.01)then
			-- print("ok1")
			-- return true
		-- elseif(angel1 > 6.57 and angel0 >-6.78 and angel0 <= 4.55 and wanqu1 > 1.01)then
			-- print("ok2")
			-- return true
		-- else
			-- return false
		-- end
		-- if(angel1 > -3.89 and angel1 <= 3.89)then
			-- return true
		-- elseif(angel1 > 3.89 and angel0 > -18.5 and angel0 <= -6.78 )then
			-- return true
		-- elseif(angel1 > 3.89 and angel0 > -6.78 and angel0 <= 9.74)then
			-- return true
		-- else
			-- print("here")
			-- return true
		-- end
		print("easy")
		return true
	end

end

function IsHengZheZhePie(bh,bl)
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
	--判断该点是第一个拐点还是最后一个拐点
	local tmpBD1 = GetTempBD(bh,1,findex)
	local BD1_len = GetBDLen(tmpBD1)
	local BD1_dis = GetDistance(startpt,fpt)
	local BD1_curve = BD1_len / BD1_dis


	--下面来计算出来最重要的拐点turning_pt_1
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
	-- 说明该拐点是最后一个拐点
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

		--bd0 的参数计算 偏离水平方向的角度 len/dis
		
		if (turning_pt_0.x ~= startpt.x) then
			local slope0 = (turning_pt_0.y -  startpt.y)/(turning_pt_0.x - startpt.x)
			angel0 = math.deg(math.atan(slope0))
		end
		angel0 = math.abs(angel0)
		bd0_dis = GetDistance(startpt,turning_pt_0)
		bd0_len = GetBDLen(bd0)
		wanqu1 = bd0_len / bd0_dis

		--bd1的参数计算
		local angel1 = 90
		if (turning_pt_1.y ~= turning_pt_0.y) then
			local slope1 = (turning_pt_1.x - turning_pt_0.x) / (turning_pt_1.y - turning_pt_0.y)
			angel1 = math.deg ( math.atan(slope1))
		end
		angel1 = math.abs(angel1)
		bd1_dis = GetDistance(turning_pt_0,turning_pt_1)
		bd1_len = GetBDLen(bd1)
		bd2_dis = GetDistance(turning_pt_1,turning_pt_2)
		bd2_len = GetBDLen(bd2)

		wanqu23= bd1_len / bd1_dis + bd2_len / bd2_dis

		-- bd3的参数计算
		
		if (turning_pt_2.x ~= turning_pt_1.x) then
			local slope2 = (turning_pt_2.y -  turning_pt_1.y)/(turning_pt_2.x - turning_pt_1.x)
			angel2 = math.deg(math.atan(slope2))
		end
		angel2 = math.abs(angel2)
		if(angel2 > 51.7)then
			return false
		elseif(angel2 > 38 and angel2 <= 51.7 and wanqu23 <= 2.1)then
			return false
		end

		if(bl == 1)then
			if(angel2 > 38 and angel2 <= 51.7 and wanqu23 > 2.1)then
				return true
			elseif(angel2 <= 38)then
				return true
			else
				return false
			end
		end
		if(bl == 2)then
			if(angel2 <= 20 and angel1 <= 13.7)then
				return true
			elseif(angel2 > 20 and angel2<= 28.4 and angel1 <= 11.1 )then
				return true
			else
				return false
			end
		end
	end
end

function  IsPie(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis
	local angel = 90
	angel = GetYAngel(startpt,endpt);

	if (startpt.y >= endpt.y or startpt.x <= endpt.x) then
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
		if(curve <= 1.1 and angel > 4.42 )then
			return true
		else
			return false
		end
	end

end


function  IsNa(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local line = GetLine(startpt,endpt)
	local len = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len / dis
	local angel = 90

	if (startpt.y >= endpt.y or startpt.x >= endpt.x) then
		return vars
	end

	if (endpt.x ~= startpt.x) then
		angel = GetYAngel(startpt,endpt);
	end

	if(curve > 1.2)then
		return false
	elseif(curve <= 1.02 and angel <= 10.5)then
		return false
	end
	if (bl == 1 )then
		if(curve <= 1.02 and angel >10.5)then
			return true
		elseif(curve <= 1.2 and curve > 1.02)then
			return true
		else
			return false
		end
	end

	if (bl == 2 )then
		if(curve <= 1.2 and curve > 1.02)then
			return true
		else
			return false
		end
	end
end
function IsHengGou(bh,bl)--去掉夹角和钩的斜率
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
	elseif(angel0 <= -10.9 and scale <= 0.55 and wanqu0 > 1.1)then
		return false
	end

	if( bl == 1)then
		if(angel0 <= 7.4 and scale > 0.55 and scale <= 0.75)then
			return true
		elseif(angel0 <= -10.9 and scale <= 0.55 and wanqu0 <= 1.1)then
			return true
		elseif(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.55 and wanqu0 > 1.04 and wanqu0 <= 1.13)then
			return true
		elseif(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.55 and wanqu0 <= 1.4)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 > -10.9 and angel0 <= 7.4 and scale <= 0.55 and wanqu0 <= 1.4)then
			return true
		elseif(angel0 > -8.7 and angel0 <=7.4 and scale <= 0.42 and wanqu0> 1.04 and wanqu0 <= 1.08)then
			return true
		else
			return false
		end
	end
end
function IsWanGou(bh,bl)
		local startpt,startindex = GetTopMostPoint(bh)
		local endpt =  GetEndPoint(bh)
		local endindex = #bh.ptSet
		local turning_pt,turning_index = GetBottomMostPoint(bh)
		local line = GetLine(startpt,turning_pt)

		if (endindex <= startindex) then
			return false
		end

		if(startpt.y >= turning_pt.y)then
			return false
		end

		if (turning_pt.y <= endpt.y) then
			return false
		end
		--在连线的左边
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
function IsHengZheGou(bh,bl)
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

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end
	--拐点0在s点右边，在拐点1上
	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end
	--拐点1在s点的右下
	if (startpt.x >= turning_pt_1.x or startpt.y >= turning_pt_1.y) then
		return false
	end
	--尾点在拐点1的左上
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
	if (turning_pt_0.y ~= startpt.y) then
		angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (turning_pt_1.x~= turning_pt_0.x) then
		angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	if(angel0 <= (-25.4) or angel0 > 8.7)then
		return false
	end

	if(bl == 1)then
		if(angel0 <=8.7 and angel0 > -25.4)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 > -16 and angel0 <= 8.7 and angel1 > -31.4 and angel1 <= 1.56)then
			return true
		else
			return false
		end
	end
end
function IsHengZheWanGou1(bh,bl)--横折弯钩（折比较直）
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
	--找出拐点2，离右下角最近的点
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
	--找出拐点0，夹角
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
	--找拐点1
	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bdmiddle = GetTempBD(bh,turning_index_0,turning_index_2)
	local line0 = GetLine(turning_pt_0,turning_pt_2)
	local turning_pt_1,turning_index_1 = GetFarthestPt2Line(bdmiddle,line0)
	turning_index_1 = turning_index_1 + turning_index_0
	turning_pt_1 = bh.ptSet[turning_index_1]
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

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0 or len_bd3 == 0 ) then
		return vars
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
	local wanqu1 = len_bd2/dis_bd2--第三个笔段的弯曲度
	local angel0 = 0
	local angel1 = 0
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end
--第二个笔段的角度
	if (turning_pt_0.y ~= turning_pt_1.y) then
		 angel1 = GetYAngel2(turning_pt_0,turning_pt_1);
	end
	if(wanqu1 <= 1.1 and angel1 <= -7.5 and angel0 <= -16.7)then
		return false
	elseif(wanqu1 > 1.2)then
		return false
	elseif(wanqu1 > 1.1 and wanqu1 <= 1.2 and angel0 > -0.65)then
		return false
	end

	if(bl == 1)then
		if(wanqu1 <= 1.1 and angel1 <= -7.5 and angel0 >= -16.7)then
			return true
		elseif(wanqu1 > 1.1 and wanqu1 <= 1.2 and angel0 <= 0.65)then
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
function IsXieWanGou(bh,bl)
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

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end
	--拐点0在s点右边，在拐点1上
	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end
	--尾点在拐点0的下,在初始点的右边
	--if (endpt.y <= turning_pt_0.y or startpt.x >= endpt.x) then
		--return false
	--end
	--尾点在拐点1的左上
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

	if(bl == 2)then
		if(angel1 > -23.1 and angel0 <= -8.4)then
			return true
		else
			return false
		end
	end
end
function IsShuWanGou(bh,bl)
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
	--找出拐点0，离左下角最近的点
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
	--找出拐点1，离右下角最近的点
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

	local line0 = GetLine(startpt,turning_pt_1)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,turning_index_1)
	local bd2 = GetTempBD(bh,turning_index_1,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local len_bd2 = GetBDLen(bd2)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,turning_pt_1)

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 ==0) then
		return false
	end

	--if (Point2LineDown(turning_pt_0,line0) == false ) then
		--return false
	--end

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

	--if	(len_bd2/len_bd0 >= 1/2) then
		--return false
	--end

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
function IsZuoDian(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis

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
function IsDian(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local len1 = GetBDLen(bh)
	local dis = GetDistance(startpt,endpt)
	local curve = len1 / dis

	if (startpt.y >= endpt.y or startpt.x >= endpt.x ) then
		return false
	end
	if(curve > 1.54)then
		return false
	end

	if(bl == 1 or bl == 2)then
		return true
	else
		return false
	end
end
function IsShuGou(bh,bl)
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

	if(scale > 0.5 or scale <= 0.05)then
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

	if(bl ==2 )then
		if(angel0 > -8.5 and scale > 0.17 and scale <= 0.35 and wanqu0 <= 1.04)then
			return true
		else
			return false
		end
	end
end
function IsShuZheZheGou(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_2,turning_index_2 = GetBottomMostPoint(bh)

	local leftpt,leftindex = GetLeftMostPoint(bh)
	if (leftindex <= startindex+5) then--如果最左点是初始点附近的点，那么通过右上角先找拐点1，再找拐点0
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
		--找出拐点1，右上
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
	else--如果最左点不是初始点附近的点，那么直接通过 角度找拐点0，再找拐点1
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

	if(bl == 2)then
		if(wanqu1 <= 0.09 and angel0 <= -16.6)then
			return true
		elseif(wanqu1 > 0.09 and angel1 <= -26 and wanqu1 <= 0.75)then
			return true
		elseif(wanqu1 > 0.09 and angel1 > -26 and angel0 <= 4.5)then
			return true
		else
			return false
		end
	end
end

function IsShuZhe(bh,bl)
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
	print("shuzhe is ok")

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

	if(angel1 > 14.4)then
		return false
	elseif(angel1 <= 14.4 and wanqu1 > 1.6)then
		return false
	elseif(angel1 > 7.4 and angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 > 13.4)then
		return false
	elseif(angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 > 26.8)then
		return false
	end

	if(bl == 1)then
		if(angel1 <= 14.4 and wanqu1 > 1.05 and wanqu1 <= 1.6)then
		print("111111111")
			return true
		elseif(angel1 > 7.4 and angel1 <= 14.4 and wanqu1 <= 1.05 and angel0 <= 13.4)then
			print("211111111")
			return true
		elseif(angel1 <= 7.4 and wanqu1 <= 1.05 and angel0 <= 7.6)then
			print("311111111")
			return true
		else
			return false
		end
	end
	if(bl == 2)then
		if(angel1 <= 7.4 and wanqu1 <= 1.05 and angel0 <= 7.6 and wanqu0 <= 1.02)then
			return true
		else
			return false
		end
	end

end
function IsHengZheWan(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

	local line0 =GetLine(startpt,endpt)
	local fpt,fpt_index=GetFarthestPt2Line(bh,line0);
	local turning_pt_0 = {}
	local turning_index_0 = 1
	local turning_pt_1 = {}
	local turning_index_1 = 1
	--在直线上方,那么是turning_pt_0
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
--第三个笔段的角度
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
function IsHengZheTi(bh,bl)
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

	if (len_bd0 == 0 or len_bd1 == 0 or len_bd2 == 0) then
		return false
	end
	--拐点0在s点右边，在拐点1上
	if (startpt.x >= turning_pt_0.x or turning_pt_0.y >= turning_pt_1.y) then
		return false
	end
	--拐点1在拐点0下方
	if (turning_pt_0.y >= turning_pt_1.y) then
		return false
	end
	--尾点在拐点1的右上
	if (endpt.x <= turning_pt_1.x or endpt.y >= turning_pt_1.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.y ~= startpt.y) then
		angel0 = GetXAngel(startpt,turning_pt_0);
	end

	if (turning_pt_1.x ~= turning_pt_0.x) then
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
	if(bl == 2)then
		if(angel0 <= 23.5 and angel1 <= 10.2 )then
			return true
		elseif(angel1 <= 14.7 and angel1 > 10.2 and wanqu1 <= 1.1)then
			return true
		else
			return false
		end
	end
end
function IsHengPieWanGou(bh,bl)--耳朵旁
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_2,turning_index_2 = GetBottomMostPoint(bh)
	local turning_pt_0 = {}
	local turning_index_0 = 1
	--local turning_pt_0，turning_index_0 = GetRightMostPoint(bh)
	--找出离右上最近的点
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

	--[[local line1 = GetLine(turning_pt_1,turning_pt_2)
	local fpt,findex = GetFarthestPt2Line(bd2,line1)
	findex = findex + turning_index_1
	fpt = bh.ptSet[findex]
	local tmpDis = 512
	local ptX = 0
	for i = turning_index_1, turning_index_2 do
		local curPt = bh.ptSet[i]
		local curDis = math.abs(curPt.y - endpt.y)
		if(curDis < tmpDis) then
			ptX = curPt.x
			tmpDis = curDis
		end
	end
	if(ptX > endpt.x) then
		return vars
	end--]]
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
	if(angel0 > 21.4 and wanqu1 > 1.3)then
		return false
	elseif(angel0 > 16 and wanqu1 <= 1.3)then
		return false
	elseif(angel0 > -13 and angel1 <= 5.3 and wanqu1 <= 1.3)then
		return false
	end

	if(bl == 1)then
		if(angel0 <= -13)then
			return true
		elseif(angel0 > -13 and angel0 <= 21.4 and wanqu1 > 1.3)then
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
function IsShuZheZhe(bh,bl)
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
	--在直线上方,那么是turning_pt_1
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

	if(angel0 > 10.7)then
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
		if(angel0 > -15.3 and angel0 <= 10.7 and angel1 > -17.7 and angel1 <= 6.7)then
			return true
		else
			return false
		end
	end
end
function IsXieShu(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

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
		if(angel <= 9.2 and curve <= 1.22)then
			return ture
		else
			return false
		end
	end

	if (bl == 2 )then
		if(curve <= 1.22 and angel <= -1.36 )then
			return ture
		else
			return false
		end
	end
end
function IsXieShuZhe(bh,bl)
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
		if(angel <= 37.7 and angel > 24.5)then
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
		if(angel1 <= 24.5 and angel1 > 9.8 and wanqu0 > 1.03 and angel0 <= 20)then
			return true
		elseif(angel1 <= 24.5 and wanqu0 <= 1.03)then
			return true
		elseif(angel1 <= 9.8 and wanqu0 > 1.03)then
			return true
		else
			return false
		end
	end
end
function IsPieZhe(bh,bl)
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
--拐点在首点下面，在末点左边
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
function IsHengZhePieWan(bh,bl)--走字底
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local rightest_pt,rightest_index = GetRightMostPoint(bh)

		--找出离右上最近的点
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

	if (bili < 1/10) then--如果小于十分之一，那么说明是拐点0,通过夹角找拐点2
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
	else--如果大于十分之一，说明是拐点2，通过夹角找拐点0
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
--通过拐点0和2，找到拐点1
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
	if(angel0 <= -32.5)then
		return false
	elseif(angel0 > 12.4)then
		return false
	elseif(angel0 > -13 and angel0 <= 12.4 and angel1 > 4.5)then
		return false
	end

	if (bl == 1 )then
		if(angel0 > -32.5 and angel0 <= -13 )then
			return true
		elseif(angel0 > -13 and angel0 <=12.4 and angel1 <= 4.5)then
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
function IsPieTi(bh,bl)
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
--拐点在首点下面，在末点左边
	if (turning_pt_0.x >= endpt.x or turning_pt_0.y <= startpt.y) then
		return false
	end

	local wanqu0 = len_bd0/dis_bd0
	local wanqu1 = len_bd1/dis_bd1
	local angel0 = 0

	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetYAngel(startpt,turning_pt_0);
	end

	if (bl == 1 )then
		return true
	end

	if (bl == 2 )then
		if(angel0 > 9.2 and wanqu0 <= 1.07 )then
			return true
		elseif(angel0 > 9.2 and wanqu0 > 1.07 and wanqu1 <= 1.06 )then
			return true
		else
			return false
		end
	end
end
function IsTi(bh,bl)
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
function IsWoGou(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
--找出离右下角最近的点
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
function IsHengZheZhe(bh,bl)
	local startpt,startindex = GetLeftMostPoint(bh)
	local endpt,endindex = GetEndPoint(bh)

   local line0 =GetLine(startpt,endpt);
	local fpt,fpt_index=GetFarthestPt2Line(bh,line0)
	local turning_pt_0 = {}
	local turning_index_0 = 1
	local turning_pt_1 = {}
	local turning_index_1 = 1
	--在直线上方,那么是turning_pt_0
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
function IsShuTi(bh,bl)
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
	if(scale > 0.82)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.03 and angel0 > 13)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.26 and angel0 <= 13 and wanqu1 <= 1.02)then
		return false
	elseif(scale <= 0.64 and wanqu0 > 1.03 and angel0 <= -8.2 and wanqu1 <= 1.02)then
		return
	end

	if(bl == 1)then
		if(scale > 0.64 and scale <= 0.82)then
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

	if(bl == 2 )then
		if(scale <= 0.64 and wanqu0 > 1.03 and wanqu0 <= 1.26 and angel0 > -1.4 and angel0 <= 13 and wanqu1 <= 1.02)then
			return true
		elseif(scale <= 0.64 and wanqu0 <= 1.03 and angel0 > -4.5)then
			return true
		else
			return false
		end
	end
end
function IsHengZheZheZhe(bh,bl)
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

	if(angel2 > 6.9 or angel2 <= -34.5)then
		return false
	end

	if(bl == 1)then
		if(angel2 > -12.4 and angel2 <= 6.9)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel2 > -12.4 and angel2 <= 6.9 and angel1 <= 22.9 and angel0 > -11.6 and angel1 <= 6.7)then
			return true
		end
	end
end
function IsHengZheWanGou2(bh,bl)--横折弯钩（折比较弯）
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
	--找出拐点2，离右下角最近的点
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
	--找出拐点0，夹角
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
	--找拐点1
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
	local wanqu1 = len_bd2/dis_bd2--第三个笔段的弯曲度
	local angel0 = 90
	local angel1 = 90
	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel(startpt,turning_pt_0);
	end
--第二个笔段的角度
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
	if(bl == 2)then
		if(angel1 > -22.8 and angel1 <= -7.5 and angel0 <= 12.6 and wanqu1 > 1.04)then
			return true
		elseif(angel1 > -36.7 and angel1 <= -7.5 and angel0 <= 6.06 and wanqu1 <= 1.04)then
			return true
		else
			return false
		end
	end
end
function IsShuZhePie(bh,bl)
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

	local line1 = GetLine(startpt,endpt)

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
		if(angel2 <= 15.4 and angel2 > 11.7)then
			return true
		elseif(angel2 > 27.6 and angel2 <= 49 and angel0 <= 14.6 and angel1 <= 87 and wanqu0 <= 1.18)then
			return true
		elseif(angel2 >15.4 and angel2 <= 49 and angel0 > 14.6)then
			return true
		elseif(angel2 > 49 and angel0 <= 23.8)then
			return true
		else
			return false
		end
	end
end
function IsXieGou(bh,bl)--删掉钩的斜率
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
	elseif(scale > 0.38 and scale <= 0.61 and wanqu > 1.05)then
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
function IsShuWan(bh,bl)
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

	--拐点0在s点下面
	if (startpt.y >= turning_pt_0.y) then
		return false
	end
	--e在s点的右下
	if (startpt.x >= endpt.x or startpt.y >= endpt.y) then
		return false
	end
	--尾点在拐点0的右边
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
	if(angel1 > -33.9 and angel1 <= 45 and angel0 <= 20.2)then
		return true
	else
		return false
	end
	if(angel1 > -2.4 and angel1 <= 27.5 and angel0 > 15.1 and angel0 <= 20.2 and wanqu0 > 1.005)then
		return true
	else
		return false
	end
end
function IsPieDian(bh,bl)
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
		elseif(angel0 > -32.1 and angel0 <= -7.4 and wanqu1 > 1.1)then
			return true
		else
			return false
		end
	end

	if(bl == 2)then
		if(angel0 <= -7.4 and wanqu1 <= 1.1)then
			return true
		elseif(angel0 > -32.1 and angel0 <= -7.4 and wanqu1 > 1.1)then
			return true
		else
			return false
		end
	end
end
function IsHengPie(bh,bl)
local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local turning_pt_0,turning_index_0 = GetRightMostPoint(bh,line)

	local bd0 = GetTempBD(bh,startindex,turning_index_0)
	local bd1 = GetTempBD(bh,turning_index_0,endindex)
	local len_bd0 = GetBDLen(bd0)
	local len_bd1 = GetBDLen(bd1)
	local dis_bd0 = GetDistance(startpt,turning_pt_0)
	local dis_bd1 = GetDistance(turning_pt_0,endpt)

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
	if (turning_pt_0.y ~= startpt.y) then
		 angel0 = GetXAngel2(startpt,turning_pt_0);
	end

	if (endpt.x ~= turning_pt_0.x) then
		 angel1 = GetYAngel(turning_pt_0,endpt);
	end
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
		elseif(angel0 <= 13.2 and wanqu0 > 1.1 and wanqu0 <= 1.3 and angel1 > 26.8)then
			return true
		else
			return false
		end
	end
end
function	IsHengZheZheZheGou(bh,bl)
	local startpt,startindex = GetStartPoint(bh)
	local endpt,endindex = GetEndPoint(bh)
	local rightest_pt,rightest_index = GetRightMostPoint(bh)
	local turning_pt_3,turning_index_3 = GetBottomMostPoint(bh)
		--找出离右上最近的点
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

	if (bili > 1/10) then--如果大于十分之一，那么说明是右上点找到了拐点0,通过夹角找拐点0
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
	else--如果小iaoyu十分之一，说明是youshang拐点2，通过夹角找拐点0
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
--通过拐点0和2，找到拐点1

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
	local angel0 = 90
	local angel1 = 90

	if (turning_pt_0.x ~= startpt.x) then
		 angel0 = GetXAngel2(startpt,turning_pt_0)
	end
	if (turning_pt_2.y ~= turning_pt_1.y) then
		 angel1 = GetXAngel2(turning_pt_1,turning_pt_2)
	end
	if(angel1 > 38)then
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
		if(angel1 <= 22.6 and angel0 > -17.9)then
			return true
		else
			return false
		end
	end
end
