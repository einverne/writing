function func1()
	--local WriteZi =[[53/186/53/188/53/190/53/192/53/192/55/192/57/192/59/192/61/192/63/192/65/192/66/192/68/192/70/192/72/192/74/192/76/192/78/192/79/192/81/192/83/192/85/192/87/192/89/192/91/192/93/192/93/192/95/192/97/192/99/192/101/192/103/192/105/192/107/192/109/192/111/192/113/192/113/192/115/193/117/194/119/195/121/196/123/197/125/198/127/199/129/200/131/201/133/202/135/203/137/204/139/205/139/199/141/199/143/199/145/199/147/199/149/199/151/199/153/199/155/199/157/199/159/199/161/199/163/199/165/199/166/199/168/199/170/199/172/199/174/199/176/199/178/199/180/199/182/199/184/199/186/199/188/199/190/199/192/199/192/199/194/199/196/199/198/199/200/199/202/199/204/199/206/199/208/199/210/199/212/199/214/199/216/199/218/199/220/199/222/199/224/199/226/199/226/206/228/206/230/206/232/206/234/206/236/206/238/206/240/206/242/206/244/206/246/206/246/206/248/207/250/208/252/209/254/210/256/211/258/212/260/213/262/214/264/215/265/212/267/212/269/212/271/212/273/212/275/212/277/212/279/212/281/212/283/212/285/212/287/212/289/212/291/212/292/212/294/212/296/212/298/212/300/212/302/212/304/212/305/212/307/212/309/212/311/212/313/212/315/212/317/212/319/212/321/212/323/212/325/212/325/212/327/212/329/212/331/212/333/212/335/212/337/212/339/212/341/212/343/212/345/212/345/212/347/212/349/212/351/212/353/212/355/212/357/212/359/212/359/212/361/212/363/212/365/212/367/212/369/212/371/212/372/212/374/212/376/212/378/212/379/212/380/213/381/214/382/215/383/216/385/219/387/219/389/219/391/219/393/219/395/219/397/219/398/219/400/219/402/219/404/219/405/219/407/219/409/219/411/219/412/219/414/219/416/219/418/219/420/219/422/219/424/219/425/219/427/219/429/219/431/219/433/219/435/219/437/219/438/219/440/219/442/219/444/219/446/219/448/219/450/219/452/219/452/219/454/219/456/219/458/219/458/219/460/219/462/219/464/219/465/219/467/219/469/219/471/219/472/219/474/219/476/219/478/219/478/219/480/219/482/219/484/219/485/219/487/219/489/219/491/219/492/219/@]]
	local WriteZi = GetWriteInfoFromC()
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
	//&&:230082��----
	--local inflection= ReadFromTxt()
	--��ά���飬�ڶ��ʵĵ�һ���յ㣬�����ʵĵ�һ���յ�
	print"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
	print (InflectionPoint[2],InflectionPoint[3])
	if(IsShuiPingPingQi(bh1,inflection[2][1],bh2,inflection[3][1]) == false)then
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

	errorlist=ReturnErrorType()
	errorpoint=ReturnErrorPoint()
	errorbh=ReturnErrorBH()

	os.remove("inflection.txt")

	//##end]]

	local CharacterRule = [[//##begin
	//^^��----
	if(IsAspectRatio() == false)then
		cflag = 0
		print"���߱�budui"
	end

	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print (result)
	return result
end
local ret = func1()
return ret