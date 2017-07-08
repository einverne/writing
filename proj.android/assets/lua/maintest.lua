

function func1()
	local WriteZi = [[202/18/203/18/204/18/205/18/206/18/206/18/207/18/208/18/209/18/210/18/210/18/211/18/212/18/213/18/214/18/215/18/215/18/216/18/217/18/218/18/219/18/219/18/220/18/221/19/221/19/222/20/223/20/223/20/224/20/225/20/226/20/227/20/227/20/228/20/229/20/230/20/231/20/231/20/232/20/233/20/234/20/235/21/235/21/236/21/237/22/238/22/238/22/239/23/240/23/240/23/241/24/242/24/243/25/243/25/244/25/245/25/246/25/247/25/247/25/248/25/249/25/250/25/251/25/251/25/252/25/253/25/254/25/255/25/256/25/256/25/257/25/258/25/259/25/260/25/260/25/261/25/262/25/263/25/264/25/264/25/265/25/266/25/267/25/268/25/268/25/269/25/270/25/271/25/272/25/272/25/273/25/274/25/275/25/276/25/277/25/277/25/278/25/279/25/280/25/281/25/@]]
	local Level = 3

    local UnitRule = [[//##begin

    //##:110007:----

    local bh0 = GetBH(0)

    if(IsHeng(bh0,bl) ==false) then

	bflag = 0

    end



    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)

    local bh1 = GetBH(1)

    if(IsHeng(bh1,bl) ==false) then

	bflag = 0

    end


    if(DirectionUD(bh0,1,bh1) ~= 2) then
	pflag = 0
    end

    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)
    local bh1 = GetBH(1)

    IsHeng(bh1,bl)

    local bh2 = GetBH(2)

    if(IsHeng(bh2,bl) ==false) then

	bflag = 0

    end

    if(DirectionUD(bh1,1,bh2) ~= 2) then
	pflag = 0
    end

    //&&:230071

    if( bflag ~= 0 and pflag ~= 0)then

        --------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsShuZhiDengFen(HengBHtable,bdtable) == false) then
                 uflag = 0
        end


        --------中点竖直对齐
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsZhongDianShuZhiDuiQi(HengBHtable,bdtable) == false) then
                uflag = 0

		print"IsZhongDianShuZhiDuiQi"

        end

        --------长度比例
        if(IsChangDuBiLi(bh2,0,bh0,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh2,0,bh1,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh0,0,bh1,0,0.8) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end


        --------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin

    //^^三：----

    if( uflag ~= 0)then

	if(IsAspectRatioRight() == false) then

		cflag = 0

		print"宽高比budui"

	end

	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

function func2()
	local WriteZi = [[202/18/203/18/204/18/205/18/206/18/206/18/207/18/208/18/209/18/210/18/210/18/211/18/212/18/213/18/214/18/215/18/215/18/216/18/217/18/218/18/219/18/219/18/220/18/221/19/221/19/222/20/223/20/223/20/224/20/225/20/226/20/227/20/227/20/228/20/229/20/230/20/231/20/231/20/232/20/233/20/234/20/235/21/235/21/236/21/237/22/238/22/238/22/239/23/240/23/240/23/241/24/242/24/243/25/243/25/244/25/245/25/246/25/247/25/247/25/248/25/249/25/250/25/251/25/251/25/252/25/253/25/254/25/255/25/256/25/256/25/257/25/258/25/259/25/260/25/260/25/261/25/262/25/263/25/264/25/264/25/265/25/266/25/267/25/268/25/268/25/269/25/270/25/271/25/272/25/272/25/273/25/274/25/275/25/276/25/277/25/277/25/278/25/279/25/280/25/281/25/@212/249/213/249/213/248/213/248/214/248/214/247/214/247/215/247/215/247/216/247/216/247/217/247/217/247/217/247/218/247/218/247/219/247/219/247/220/247/220/247/221/247/221/247/222/247/222/247/222/247/223/247/223/247/224/247/224/247/225/247/225/247/226/247/226/247/227/247/227/247/227/247/228/247/228/247/229/247/229/247/230/247/230/247/230/247/230/246/230/246/230/245/230/245/231/245/231/245/232/245/232/245/233/245/233/245/234/245/234/245/235/245/235/245/235/245/236/245/236/245/237/245/237/245/238/245/238/245/239/245/239/245/240/245/240/245/240/245/241/245/241/245/242/245/242/245/243/245/243/245/244/245/244/245/245/245/245/245/245/245/246/245/246/245/247/245/247/245/248/245/248/245/249/245/249/245/250/245/250/245/250/245/251/245/251/245/252/245/252/245/253/245/253/245/254/245/254/245/254/245/@]]
	local Level = 3

    local UnitRule = [[//##begin

    //##:110007:----

    local bh0 = GetBH(0)

    if(IsHeng(bh0,bl) ==false) then

	bflag = 0

    end



    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)

    local bh1 = GetBH(1)

    if(IsHeng(bh1,bl) ==false) then

	bflag = 0

    end


    if(DirectionUD(bh0,1,bh1) ~= 2) then
	pflag = 0
    end

    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)
    local bh1 = GetBH(1)

    IsHeng(bh1,bl)

    local bh2 = GetBH(2)

    if(IsHeng(bh2,bl) ==false) then

	bflag = 0

    end

    if(DirectionUD(bh1,1,bh2) ~= 2) then
	pflag = 0
    end

    //&&:230071

    if( bflag ~= 0 and pflag ~= 0)then

        --------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsShuZhiDengFen(HengBHtable,bdtable) == false) then
                 uflag = 0
        end


        --------中点竖直对齐
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsZhongDianShuZhiDuiQi(HengBHtable,bdtable) == false) then
                uflag = 0

		print"IsZhongDianShuZhiDuiQi"

        end

        --------长度比例
        if(IsChangDuBiLi(bh2,0,bh0,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh2,0,bh1,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh0,0,bh1,0,0.8) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end


        --------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin

    //^^三：----

    if( uflag ~= 0)then

	if(IsAspectRatioRight() == false) then

		cflag = 0

		print"宽高比budui"

	end

	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end

function func3()
	local WriteZi = [[202/18/203/18/204/18/205/18/206/18/206/18/207/18/208/18/209/18/210/18/210/18/211/18/212/18/213/18/214/18/215/18/215/18/216/18/217/18/218/18/219/18/219/18/220/18/221/19/221/19/222/20/223/20/223/20/224/20/225/20/226/20/227/20/227/20/228/20/229/20/230/20/231/20/231/20/232/20/233/20/234/20/235/21/235/21/236/21/237/22/238/22/238/22/239/23/240/23/240/23/241/24/242/24/243/25/243/25/244/25/245/25/246/25/247/25/247/25/248/25/249/25/250/25/251/25/251/25/252/25/253/25/254/25/255/25/256/25/256/25/257/25/258/25/259/25/260/25/260/25/261/25/262/25/263/25/264/25/264/25/265/25/266/25/267/25/268/25/268/25/269/25/270/25/271/25/272/25/272/25/273/25/274/25/275/25/276/25/277/25/277/25/278/25/279/25/280/25/281/25/@212/249/213/249/213/248/213/248/214/248/214/247/214/247/215/247/215/247/216/247/216/247/217/247/217/247/217/247/218/247/218/247/219/247/219/247/220/247/220/247/221/247/221/247/222/247/222/247/222/247/223/247/223/247/224/247/224/247/225/247/225/247/226/247/226/247/227/247/227/247/227/247/228/247/228/247/229/247/229/247/230/247/230/247/230/247/230/246/230/246/230/245/230/245/231/245/231/245/232/245/232/245/233/245/233/245/234/245/234/245/235/245/235/245/235/245/236/245/236/245/237/245/237/245/238/245/238/245/239/245/239/245/240/245/240/245/240/245/241/245/241/245/242/245/242/245/243/245/243/245/244/245/244/245/245/245/245/245/245/245/246/245/246/245/247/245/247/245/248/245/248/245/249/245/249/245/250/245/250/245/250/245/251/245/251/245/252/245/252/245/253/245/253/245/254/245/254/245/254/245/@168/503/168/502/169/501/171/501/172/501/173/501/175/501/176/501/177/501/179/501/180/501/181/500/182/499/184/499/185/499/186/499/188/499/189/499/190/499/192/499/193/499/195/499/196/499/197/499/199/499/200/499/202/499/203/499/204/499/206/499/207/499/208/499/210/499/211/499/213/499/214/499/215/499/217/499/218/499/220/499/221/499/222/499/224/499/225/499/226/499/228/499/229/499/231/499/232/499/233/499/235/499/236/499/238/499/239/499/240/499/242/499/243/499/244/499/246/499/247/499/249/499/250/499/251/499/253/499/254/499/256/499/257/499/258/499/260/499/261/499/262/499/264/499/265/499/267/499/268/499/269/499/271/499/272/499/274/499/275/499/276/499/278/499/279/499/280/499/282/499/283/499/285/499/286/499/287/499/289/499/290/499/292/499/293/499/294/499/296/499/297/499/298/499/300/499/301/499/303/499/@]]
	local Level = 3

    local UnitRule = [[//##begin

    //##:110007:----

    local bh0 = GetBH(0)

    if(IsHeng(bh0,bl) ==false) then

	bflag = 0

    end



    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)

    local bh1 = GetBH(1)

    if(IsHeng(bh1,bl) ==false) then

	bflag = 0

    end


    if(DirectionUD(bh0,1,bh1) ~= 2) then
	pflag = 0
    end

    //##:110007:----

    local bh0 = GetBH(0)

    IsHeng(bh0,bl)
    local bh1 = GetBH(1)

    IsHeng(bh1,bl)

    local bh2 = GetBH(2)

    if(IsHeng(bh2,bl) ==false) then

	bflag = 0

    end

    if(DirectionUD(bh1,1,bh2) ~= 2) then
	pflag = 0
    end

    //&&:230071

    if( bflag ~= 0 and pflag ~= 0)then

        --------竖直等分
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsShuZhiDengFen(HengBHtable,bdtable) == false) then
                 uflag = 0
        end


        --------中点竖直对齐
        local HengBHtable={}
        local bdtable={}
        HengBHtable[1]=bh0
        HengBHtable[2]=bh1
        HengBHtable[3]=bh2
        bdtable[1]=0
        bdtable[2]=0
        bdtable[3]=0
        if(IsZhongDianShuZhiDuiQi(HengBHtable,bdtable) == false) then
                uflag = 0

		print"IsZhongDianShuZhiDuiQi"

        end

        --------长度比例
        if(IsChangDuBiLi(bh2,0,bh0,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh2,0,bh1,0,1.05) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end

        if(IsChangDuBiLi(bh0,0,bh1,0,0.8) == false)then

		uflag = 0

		print"IsChangDuBiLi"

	end


        --------返回结果
	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local CharacterRule = [[//##begin

    //^^三：----

    if( uflag ~= 0)then

	if(IsAspectRatioRight() == false) then

		cflag = 0

		print"宽高比budui"

	end

	errorBHPoint=ReturntypeInfo()

    end

//##end]]

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	print(result)
end



func1()
func2()
func3()

