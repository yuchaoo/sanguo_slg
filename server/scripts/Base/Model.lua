local This = class(...)

function This:init()
    self.m_observers = {}
end

function This:addObserver(observer)
    for i , o in ipairs(self.m_observers) do
        if o == observer then
            return 
        end
    end
    table.insert(self.m_observers,observer)
end

function This:removeObserver(observer)
    for i , o in ipairs(self.m_observers) do
        if o == observer then
            table.remove(self.m_observers,i)
            return
        end
    end
end

function This:notifyObserver(funcname,...)
    for i , o in ipairs(self.m_observers) do
        if o[funcname] then
            o[funcname](o,...)
        end
    end
end


