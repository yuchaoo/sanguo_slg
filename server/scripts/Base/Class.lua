print("require class...")

function class(name,supername)
    local o = {}
    o.__class_name = name
    o.__index = o
    
    if supername then
        local super = require(supername)
        if not super then
            assert(false)
            print(string.format("the super class is require failed!!!",supername))
        end
        setmetatable(o,super)
        o.__super_name = supername
        o.__super = super
    else
        o.create = function(self,...)
            local t = {}
            setmetatable(t,o)
            if t.init then
                t:init(...)
            end
            return t
        end
        o.getClassName = function(self,...)
            return o.__class_name
        end
    end
    package.loaded[name] = o
    return o , o.__super
end
