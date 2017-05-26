

function func1()
	local WriteZi = [[106/57/106/61/106/66/106/70/106/75/106/79/106/84/106/88/106/93/106/97/106/102/106/106/106/111/106/115/106/120/106/124/106/129/106/134/106/138/106/143/106/147/106/152/106/156/106/161/106/165/106/170/106/174/106/179/106/183/106/188/106/192/106/197/106/201/106/206/106/211/106/215/106/220/106/224/106/229/106/233/106/238/106/242/106/247/106/251/106/256/106/260/105/265/105/269/104/274/104/278/104/283/104/287/103/292/102/296/102/301/102/305/102/310/101/314/100/319/99/323/98/327/97/332/96/336/96/341/96/345/94/349/93/354/92/358/91/363/90/367/89/371/88/376/86/380/84/384/84/388/82/392/79/396/78/400/78/404/76/408/74/412/72/416/70/421/70/425/69/429/66/433/64/437/62/441/61/445/59/449/58/454/56/457/53/460/51/464/47/466/45/470/41/472/38/475/34/477/30/477/@]]
	local Level = 3

    local UnitRule = [[//##begin
    //##:110009:----
    local bh0= GetBH(0)
    if(IsPie(bh0,bl) ==false) then
	bflag = 0
    end

    //##:110035:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    if(IsHengZheGou(bh1,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh1_bd0=GetBDByBH(bh1,0)
        local bh1_bd1=GetBDByBH(bh1,1)
        local bh1_bd2=GetBDByBH(bh1,2)
        if(BH2BHXiangJie(bh0_bd0,bh1_bd0,0,0) == false) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    if(IsHeng(bh2,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh2_bd0=GetBDByBH(bh2,0)
        if(BH2BHXiangJie(bh0_bd0,bh2_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh2_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd0,1,bh2_bd0) ~= 2) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    IsHeng(bh2,bl)
    local bh2_bd0=GetBDByBH(bh2,0)

    local bh3= GetBH(3)
    if(IsHeng(bh3,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh3_bd0=GetBDByBH(bh3,0)
        if(BH2BHXiangJie(bh0_bd0,bh3_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh3_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh2_bd0,1,bh3_bd0) ~= 2) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd2,2,bh3_bd0) ~= 1) then
	    pflag = 0
        end
    end

    //&&:240108
    if( bflag ~= 0 and pflag ~= 0)then

        -------------水平平齐

        -------------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh1
        HengBHtable[2]=bh2
        HengBHtable[3]=bh3
        HengBHtable[4]=bh1
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        bdtable[4]=2
        if(IsShuZhiDengFen(HengBHtable, bdtable) == false) then
		 uflag = 0

		 print"ShuZhiDengFen"

	end

        -------------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin
	//^^澹細----
	if( uflag ~= 0)then
		if(IsAspectRatio() == false)then
			--cflag = 0
			print"瀹介珮姣攂udui"
		end
	end
	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

function func2()
	local WriteZi = [[106/57/106/61/106/66/106/70/106/75/106/79/106/84/106/88/106/93/106/97/106/102/106/106/106/111/106/115/106/120/106/124/106/129/106/134/106/138/106/143/106/147/106/152/106/156/106/161/106/165/106/170/106/174/106/179/106/183/106/188/106/192/106/197/106/201/106/206/106/211/106/215/106/220/106/224/106/229/106/233/106/238/106/242/106/247/106/251/106/256/106/260/105/265/105/269/104/274/104/278/104/283/104/287/103/292/102/296/102/301/102/305/102/310/101/314/100/319/99/323/98/327/97/332/96/336/96/341/96/345/94/349/93/354/92/358/91/363/90/367/89/371/88/376/86/380/84/384/84/388/82/392/79/396/78/400/78/404/76/408/74/412/72/416/70/421/70/425/69/429/66/433/64/437/62/441/61/445/59/449/58/454/56/457/53/460/51/464/47/466/45/470/41/472/38/475/34/477/30/477/@102/53/111/53/120/53/128/53/137/53/146/53/155/53/163/51/172/51/180/51/189/51/198/51/207/51/215/51/224/51/233/51/241/51/250/51/259/51/268/51/276/51/285/51/294/51/302/51/311/51/320/52/328/54/337/55/345/55/354/55/362/53/370/51/378/49/387/49/396/49/404/49/413/49/422/49/417/51/416/59/415/67/415/76/415/85/415/93/415/102/415/111/415/119/415/128/415/137/415/146/415/154/415/163/415/172/415/180/415/189/415/198/415/207/415/215/415/224/415/233/415/241/415/250/415/259/415/268/415/276/415/285/415/294/415/302/415/311/415/320/415/329/415/337/415/346/415/355/415/363/415/372/415/381/415/390/415/398/415/407/415/416/415/424/415/433/415/442/415/451/415/459/415/468/415/477/413/483/404/483/395/483/388/479/380/475/374/468/369/461/367/454/367/445/364/438/359/434/351/433/@]]
	local Level = 3

    local UnitRule = [[//##begin
    //##:110009:----
    local bh0= GetBH(0)
    if(IsPie(bh0,bl) ==false) then
	bflag = 0
    end

    //##:110035:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    if(IsHengZheGou(bh1,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh1_bd0=GetBDByBH(bh1,0)
        local bh1_bd1=GetBDByBH(bh1,1)
        local bh1_bd2=GetBDByBH(bh1,2)
        if(BH2BHXiangJie(bh0_bd0,bh1_bd0,0,0) == false) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    if(IsHeng(bh2,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh2_bd0=GetBDByBH(bh2,0)
        if(BH2BHXiangJie(bh0_bd0,bh2_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh2_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd0,1,bh2_bd0) ~= 2) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    IsHeng(bh2,bl)
    local bh2_bd0=GetBDByBH(bh2,0)

    local bh3= GetBH(3)
    if(IsHeng(bh3,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh3_bd0=GetBDByBH(bh3,0)
        if(BH2BHXiangJie(bh0_bd0,bh3_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh3_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh2_bd0,1,bh3_bd0) ~= 2) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd2,2,bh3_bd0) ~= 1) then
	    pflag = 0
        end
    end

    //&&:240108
    if( bflag ~= 0 and pflag ~= 0)then

        -------------水平平齐

        -------------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh1
        HengBHtable[2]=bh2
        HengBHtable[3]=bh3
        HengBHtable[4]=bh1
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        bdtable[4]=2
        if(IsShuZhiDengFen(HengBHtable, bdtable) == false) then
		 uflag = 0

		 print"ShuZhiDengFen"

	end

        -------------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin
	//^^澹細----
	if( uflag ~= 0)then
		if(IsAspectRatio() == false)then
			--cflag = 0
			print"瀹介珮姣攂udui"
		end
	end
	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

function func3()
	local WriteZi = [[106/57/106/61/106/66/106/70/106/75/106/79/106/84/106/88/106/93/106/97/106/102/106/106/106/111/106/115/106/120/106/124/106/129/106/134/106/138/106/143/106/147/106/152/106/156/106/161/106/165/106/170/106/174/106/179/106/183/106/188/106/192/106/197/106/201/106/206/106/211/106/215/106/220/106/224/106/229/106/233/106/238/106/242/106/247/106/251/106/256/106/260/105/265/105/269/104/274/104/278/104/283/104/287/103/292/102/296/102/301/102/305/102/310/101/314/100/319/99/323/98/327/97/332/96/336/96/341/96/345/94/349/93/354/92/358/91/363/90/367/89/371/88/376/86/380/84/384/84/388/82/392/79/396/78/400/78/404/76/408/74/412/72/416/70/421/70/425/69/429/66/433/64/437/62/441/61/445/59/449/58/454/56/457/53/460/51/464/47/466/45/470/41/472/38/475/34/477/30/477/@102/53/111/53/120/53/128/53/137/53/146/53/155/53/163/51/172/51/180/51/189/51/198/51/207/51/215/51/224/51/233/51/241/51/250/51/259/51/268/51/276/51/285/51/294/51/302/51/311/51/320/52/328/54/337/55/345/55/354/55/362/53/370/51/378/49/387/49/396/49/404/49/413/49/422/49/417/51/416/59/415/67/415/76/415/85/415/93/415/102/415/111/415/119/415/128/415/137/415/146/415/154/415/163/415/172/415/180/415/189/415/198/415/207/415/215/415/224/415/233/415/241/415/250/415/259/415/268/415/276/415/285/415/294/415/302/415/311/415/320/415/329/415/337/415/346/415/355/415/363/415/372/415/381/415/390/415/398/415/407/415/416/415/424/415/433/415/442/415/451/415/459/415/468/415/477/413/483/404/483/395/483/388/479/380/475/374/468/369/461/367/454/367/445/364/438/359/434/351/433/@112/121/115/121/118/121/121/121/124/121/127/121/130/121/133/121/136/121/138/121/141/121/144/121/147/121/150/121/153/121/156/121/159/121/162/121/165/121/168/121/171/121/173/121/176/121/179/121/182/121/185/121/188/121/191/121/194/121/197/121/200/121/203/121/206/121/208/121/211/121/214/121/217/121/220/121/223/121/226/121/229/121/232/121/235/121/238/121/240/121/243/121/246/121/249/121/252/121/255/121/258/121/261/121/264/121/267/121/270/121/273/121/275/121/278/121/281/121/284/121/287/121/290/121/293/121/296/121/299/121/302/121/305/121/307/121/310/121/313/121/316/121/319/121/322/121/325/121/328/121/331/121/334/121/337/121/340/121/342/121/345/121/348/121/351/121/354/121/357/121/360/121/363/121/366/121/369/121/372/121/375/121/377/121/380/121/383/121/386/121/389/121/392/121/395/121/398/121/401/121/@]]
	local Level = 3

    local UnitRule = [[//##begin
    //##:110009:----
    local bh0= GetBH(0)
    if(IsPie(bh0,bl) ==false) then
	bflag = 0
    end

    //##:110035:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    if(IsHengZheGou(bh1,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh1_bd0=GetBDByBH(bh1,0)
        local bh1_bd1=GetBDByBH(bh1,1)
        local bh1_bd2=GetBDByBH(bh1,2)
        if(BH2BHXiangJie(bh0_bd0,bh1_bd0,0,0) == false) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    if(IsHeng(bh2,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh2_bd0=GetBDByBH(bh2,0)
        if(BH2BHXiangJie(bh0_bd0,bh2_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh2_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd0,1,bh2_bd0) ~= 2) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    IsHeng(bh2,bl)
    local bh2_bd0=GetBDByBH(bh2,0)

    local bh3= GetBH(3)
    if(IsHeng(bh3,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh3_bd0=GetBDByBH(bh3,0)
        if(BH2BHXiangJie(bh0_bd0,bh3_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh3_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh2_bd0,1,bh3_bd0) ~= 2) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd2,2,bh3_bd0) ~= 1) then
	    pflag = 0
        end
    end

    //&&:240108
    if( bflag ~= 0 and pflag ~= 0)then

        -------------水平平齐

        -------------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh1
        HengBHtable[2]=bh2
        HengBHtable[3]=bh3
        HengBHtable[4]=bh1
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        bdtable[4]=2
        if(IsShuZhiDengFen(HengBHtable, bdtable) == false) then
		 uflag = 0

		 print"ShuZhiDengFen"

	end

        -------------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin
	//^^澹細----
	if( uflag ~= 0)then
		if(IsAspectRatio() == false)then
			--cflag = 0
			print"瀹介珮姣攂udui"
		end
	end
	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

function func4()
	local WriteZi = [[106/57/106/61/106/66/106/70/106/75/106/79/106/84/106/88/106/93/106/97/106/102/106/106/106/111/106/115/106/120/106/124/106/129/106/134/106/138/106/143/106/147/106/152/106/156/106/161/106/165/106/170/106/174/106/179/106/183/106/188/106/192/106/197/106/201/106/206/106/211/106/215/106/220/106/224/106/229/106/233/106/238/106/242/106/247/106/251/106/256/106/260/105/265/105/269/104/274/104/278/104/283/104/287/103/292/102/296/102/301/102/305/102/310/101/314/100/319/99/323/98/327/97/332/96/336/96/341/96/345/94/349/93/354/92/358/91/363/90/367/89/371/88/376/86/380/84/384/84/388/82/392/79/396/78/400/78/404/76/408/74/412/72/416/70/421/70/425/69/429/66/433/64/437/62/441/61/445/59/449/58/454/56/457/53/460/51/464/47/466/45/470/41/472/38/475/34/477/30/477/@102/53/111/53/120/53/128/53/137/53/146/53/155/53/163/51/172/51/180/51/189/51/198/51/207/51/215/51/224/51/233/51/241/51/250/51/259/51/268/51/276/51/285/51/294/51/302/51/311/51/320/52/328/54/337/55/345/55/354/55/362/53/370/51/378/49/387/49/396/49/404/49/413/49/422/49/417/51/416/59/415/67/415/76/415/85/415/93/415/102/415/111/415/119/415/128/415/137/415/146/415/154/415/163/415/172/415/180/415/189/415/198/415/207/415/215/415/224/415/233/415/241/415/250/415/259/415/268/415/276/415/285/415/294/415/302/415/311/415/320/415/329/415/337/415/346/415/355/415/363/415/372/415/381/415/390/415/398/415/407/415/416/415/424/415/433/415/442/415/451/415/459/415/468/415/477/413/483/404/483/395/483/388/479/380/475/374/468/369/461/367/454/367/445/364/438/359/434/351/433/@112/121/115/121/118/121/121/121/124/121/127/121/130/121/133/121/136/121/138/121/141/121/144/121/147/121/150/121/153/121/156/121/159/121/162/121/165/121/168/121/171/121/173/121/176/121/179/121/182/121/185/121/188/121/191/121/194/121/197/121/200/121/203/121/206/121/208/121/211/121/214/121/217/121/220/121/223/121/226/121/229/121/232/121/235/121/238/121/240/121/243/121/246/121/249/121/252/121/255/121/258/121/261/121/264/121/267/121/270/121/273/121/275/121/278/121/281/121/284/121/287/121/290/121/293/121/296/121/299/121/302/121/305/121/307/121/310/121/313/121/316/121/319/121/322/121/325/121/328/121/331/121/334/121/337/121/340/121/342/121/345/121/348/121/351/121/354/121/357/121/360/121/363/121/366/121/369/121/372/121/375/121/377/121/380/121/383/121/386/121/389/121/392/121/395/121/398/121/401/121/@104/333/107/333/110/333/113/333/116/333/119/333/123/333/126/333/129/333/132/333/135/333/138/333/141/333/144/333/147/333/150/333/153/333/156/333/159/333/162/333/165/333/168/333/171/333/174/333/177/333/180/333/183/333/186/333/189/333/193/333/196/333/199/333/202/333/205/333/208/333/211/333/214/333/217/334/220/334/223/334/226/335/229/335/232/335/235/335/238/335/241/335/244/335/247/335/250/335/253/335/256/335/259/335/262/335/266/335/269/335/272/335/275/335/278/335/281/335/284/335/287/335/290/335/293/335/296/335/299/335/302/335/305/335/308/335/311/335/314/335/317/335/320/335/323/335/326/335/329/334/332/333/335/333/338/333/341/333/344/333/347/333/350/333/353/333/356/333/359/333/362/333/365/333/368/333/371/333/374/333/377/333/380/333/383/333/386/333/390/333/391/331/394/331/397/331/400/331/403/331/@]]
	local Level = 3

    local UnitRule = [[//##begin
    //##:110009:----
    local bh0= GetBH(0)
    if(IsPie(bh0,bl) ==false) then
	bflag = 0
    end

    //##:110035:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    if(IsHengZheGou(bh1,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh1_bd0=GetBDByBH(bh1,0)
        local bh1_bd1=GetBDByBH(bh1,1)
        local bh1_bd2=GetBDByBH(bh1,2)
        if(BH2BHXiangJie(bh0_bd0,bh1_bd0,0,0) == false) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    if(IsHeng(bh2,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh2_bd0=GetBDByBH(bh2,0)
        if(BH2BHXiangJie(bh0_bd0,bh2_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh2_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd0,1,bh2_bd0) ~= 2) then
	    pflag = 0
        end
    end

    //##:110007:----
    local bh0= GetBH(0)
    IsPie(bh0,bl)
    local bh0_bd0=GetBDByBH(bh0,0)

    local bh1= GetBH(1)
    IsHengZheGou(bh1,bl)
    local bh1_bd0=GetBDByBH(bh1,0)
    local bh1_bd1=GetBDByBH(bh1,1)
    local bh1_bd2=GetBDByBH(bh1,2)

    local bh2= GetBH(2)
    IsHeng(bh2,bl)
    local bh2_bd0=GetBDByBH(bh2,0)

    local bh3= GetBH(3)
    if(IsHeng(bh3,bl) ==false) then
	bflag = 0
    end
    if(bflag~=0)then
        local bh3_bd0=GetBDByBH(bh3,0)
        if(BH2BHXiangJie(bh0_bd0,bh3_bd0,2,0) == false) then
	    pflag = 0
        end
        if(BH2BHXiangJie(bh1_bd1,bh3_bd0,2,1) == false) then
	    pflag = 0
        end
        if(DirectionUD(bh2_bd0,1,bh3_bd0) ~= 2) then
	    pflag = 0
        end
        if(DirectionUD(bh1_bd2,2,bh3_bd0) ~= 1) then
	    pflag = 0
        end
    end

    //&&:240108
    if( bflag ~= 0 and pflag ~= 0)then

        -------------水平平齐

        -------------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh1
        HengBHtable[2]=bh2
        HengBHtable[3]=bh3
        HengBHtable[4]=bh1
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        bdtable[4]=2
        if(IsShuZhiDengFen(HengBHtable, bdtable) == false) then
		 uflag = 0

		 print"ShuZhiDengFen"

	end

        -------------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin
	//^^澹細----
	if( uflag ~= 0)then
		if(IsAspectRatio() == false)then
			--cflag = 0
			print"瀹介珮姣攂udui"
		end
	end
	//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

func1()
func2()
func3()
func4()
