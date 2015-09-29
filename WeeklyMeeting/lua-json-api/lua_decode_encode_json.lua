--load lib file
JSON = assert(loadfile "JSON.lua")()

local raw_json_text = '{"error":[{"errortype":"A0001","errorstroke":[0,1],"errorpoint":[0.2,0.3]},{"errortype":"A0001","errorstroke":[0,1],"errorpoint":[0.3,0.3]}]}'

--decode json text
local lua_value = JSON:decode(raw_json_text)
l = lua_value["error"]

for k,v in pairs (l) do
  print("error type", v["errortype"])
  for i,j in pairs(v["errorstroke"]) do
    print("error stroke")
    print(j)
  end
  for i,j in pairs(v["errorpoint"]) do
    print("error point")
    print(j)
  end
end

--print(lua_value)


local errorstroke = {0, 1}
local errorpoint = {0.2, 0.4}

local error1 = { ["errortype"]="A0001" , ["errorstroke"] = errorstroke, ["errorpoint"]= errorpoint }
local error2 = { ["errortype"]="A0001" , ["errorstroke"] = errorstroke, ["errorpoint"]= errorpoint }
local lvalue = { error = { error1, error2}}

local raw = JSON:encode(lvalue)

print(raw)

--errortype string , errorstroke & errorpoint table
function error_string(errortype, errorstroke, errorpoint)
    local error = { ["errortype"]=errortype , ["errorstroke"] = errorstroke, ["errorpoint"]= errorpoint}
    return error
end;


errors = {}
table.insert(errors, error_string("A0001",{0.2, 0.3}, {0.3,0.3}))
table.insert(errors, error_string("A0002",{0.2, 0.3}, {0.3,0.3}))
--errors should be a list
function error_json_string(errors)
    return { error = errors }
end

print(JSON:encode(error_json_string(errors)))