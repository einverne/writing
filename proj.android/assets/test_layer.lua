

	-- 这里定义一个函数cclog，用来打印字符串参数  
    local cclog = function(...)
        print(string.format(...))
    end
	
	local curr_scene = CCDirector:sharedDirector():getRunningScene()
	