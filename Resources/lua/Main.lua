function func1()
	local WriteZi = GetWriteInfoFromC()
	local StandardZiInfo = GetStandardZiInfoFromC()
	local Level = 3
	local UnitRule = GetUnitRulesFromC()

	local CharacterRule = GetZiRulesFromC()

	local RunAPI=require("RunAPI_1208")
	local result = RunAPI:PassParametersToAPI(WriteZi,Level,UnitRule,CharacterRule)
	return result

end

local ret=func1()
return ret
