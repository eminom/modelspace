
local SimpleNode = {}

function SimpleNode:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self

	o.x = 0
	o.y = 0
	o.z = 0
	o.spinSpeed = math.random(300) / 600
	o._a = math.random(10) / 10
	o.wSpeed = 0.05 + math.random(45) / 60

	local s = 0.2 + math.random(3) / 10
	o.oScale = s
	return o
end

function SimpleNode:update(dt)
	self._t = self._t + dt * self.wSpeed
	self._a = self._a + dt * self.spinSpeed * 5

	self.x = math.cos(self._t) * self._radius
	self.z = math.sin(self._t) * self._radius
	mcore.setEsNodeTransform(self.__originHost, self.x, self.y, self.z)
	mcore.setEsNodeRotate(self.__originHost, self._a)
	mcore.setEsNodeScale(self.__originHost, self.oScale, self.oScale, self.oScale)
	--print('in here simple-node update:'..tostring(dt))
end

function SimpleNode:setPos(x, y, z)
	self.x = x
	self.y = y
	self.z = z

	self._radius = math.sqrt(x*x + z*z)
	self._t = 0
end

function SimpleNode:setHost(host)
	self.__originHost = host
end

local function createSimpleNode()
	return SimpleNode:new()
end

return createSimpleNode