function logError(...)
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
	require("Base.Class")
	require("Base.Model")
end

function rungame()
	local net = require("cpp.net")
	local gamemanager = require("GameManager")

	function createConnection(connect)
		gamemanager:createConnection(connect)
	end

	function update(dt)
		gamemanager:update(dt)
	end

	local ret = net:init(8080)
	if ret then
		gamemanager:init()
		net:setLuaCreateConnHandler(createConnection)
		net:setLuaUpdateHandler(update)
		net:dispatch()
	end
end

function main()
	requires()
	rungame()
end

xpcall(main,logError)





