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
	local conn = require("cpp.Connection")
	local gamemanager = require("GameManager")
	net:init()
	
	local connection = net:createConnection()

    function connected()
		print("connected...")
		connection:send(100,"dfafaa")
    end

    function recv(cmd,data,len)
        print("cmd",cmd)
    end

    function close()
        print("close")
    end

    function timeout()

    end

	
    connection:setMaxRewriteCount(3)
    connection:setLuaConnectedHandler(connected)
    connection:setLuaRecvHandler(recv)
    connection:setLuaCloseHandler(close)
    connection:setLuaTimeoutHandler(timeout)
    connection:connect("127.0.0.1",8080);
    net:dispatch()
end

function main()
	requires()
	parseproto()
	rungame()
end

xpcall(main,logError)