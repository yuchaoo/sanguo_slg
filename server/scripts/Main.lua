function luaError(...)
    local traceDebug = debug.traceback()
    local argCount = select("#", ...)
    if argCount > 0 then
        traceDebug = traceDebug .. "\n"
        for i = 1, argCount do
            local arg = select(i, ...)
            traceDebug = traceDebug .. "\t" .. tostring(arg)
        end
    end
    print("----------------------------------------")
    print(traceDebug)
    print("----------------------------------------")
end

function requires()
	require("Proto.parser")
	require("Proto.protobuf")
	require("Base.Class")
	require("Base.Model")
end

function parseproto()
	local protobuf = require("Proto.protobuf")
	local parser = require("Proto.parser")
	local t = parser.register("../proto/type.proto")
	local msgitemreward = {
		id = 1,
		num = 2
	}
	local code = protobuf.encode("msg.msgitemreward",msgitemreward)
	local enco = protobuf.decode("msg.msgitemreward",code)
	
	local msgcardawake = {
		level = 1,
		awake = true,
		star_list = {
			{star_id = 1, star_level = 2, star_lucky = 3},
			{star_id = 2, star_level = 2, star_lucky = 3}
		}
	}
	local t = protobuf.encode("msg.msgcardawake",msgcardawake)
	local t1 = protobuf.decode("msg.msgcardawake",t)
	for i , info in ipairs(t1.star_list) do
		print(info.star_id, info.star_level, info.star_lucky)
	end
end

function rungame()
	local net = require("cpp.net")
	local gamemanager = require("GameManager")

	function createConnection(connect)
		print("add connection...")
		gamemanager:createConnection(connect)
	end

	function removeConnection(connect)
		print("remove connection...")
		gamemanager:removeConnection(connect)
	end

	function handleGm(gm)
		print("gm",gm)
		local slice = string.match(gm,"(.+ )*")
		
	end

	function update(dt)
		gamemanager:update(dt)
	end

	net:init()
	local ret = net:listen(8080)
	if ret then
		gamemanager:init()
		net:setLuaCreateConnHandler(createConnection)
		net:setLuaRemoveConnHandler(removeConnection)
		net:setLuaGmHandler(handleGm)
		net:setLuaUpdateHandler(update)
		net:dispatch()
	end
end

function main()
	requires()
	parseproto()
	rungame()
end

xpcall(main,logError)





