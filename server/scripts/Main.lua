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





