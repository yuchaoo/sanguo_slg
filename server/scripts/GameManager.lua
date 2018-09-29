local This , super = class(...,"Base.Model")
local Connection = require("cpp.Connection")

function This:init()
    super.init(self)
    self.m_players = {}
    self:initData()
end

function This:initData()
    local s = debug.getinfo(2,'S')
    print(s.source)
end

function This:createConnection(connection)

end

function This:removeConnection(connection)
    
end

function This:update(dt)
    
end

