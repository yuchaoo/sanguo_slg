local a = 1
local b = 2
local c = a + b
print(string.format("a + b = %d",a , b))

local This , super = class("Person")

function This:create(...)
	local o = {}
	setmetatable(self,o)
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
