luaStr="I' m Himi"
luaStr2="are you ok!"
luaTable={age=23,name="Himi",sex="M"}
function luaLogString(_logStr,_logNum,_logBool)
    print("Lua 脚本打印从C传来的字符串：",_logStr,_logNum,_logBool)
    return "call lua function OK"
end
function call_Cpp(_logStr,_logNum,_logBool)
    num,str = cppFunction(999,"I'm a lua string")
    print("从cpp函数中获得两个返回值：",num,str)
end