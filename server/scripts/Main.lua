local Test1 = require("Test1")

local This , super = class("Person")

print(This,super)

function This:create(...)
	local o = {}
	setmetatable(o,self)
	o:init(...)
	return o
end

function This:init(info)
	self.m_id = info.id
	self.m_name = info.name
end

function This:getId()
	return self.m_id
end

function This:getName()
	return self.m_name
end

local a = This:create({
	id = 1000,
	name = "dddddd"
})

print(string.format("id = %d",a:getId()))
print(string.format("name = %s",a:getName()))

local NetworkManager = require("cpp.NetworkManager")

local ret = NetworkManager:init(8080)
if ret then
	print("network init secceed......")
end

NetworkManager:setLuaUpdateHandler(function(dt)
	print(string.format("lua update dt = %d",dt))
end)

NetworkManager:dispatch()
