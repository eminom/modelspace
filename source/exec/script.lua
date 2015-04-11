

print("This is a simple test")
print('And i should give you a table')

local AppObj = {
	name = 'eminem'
}

function AppObj:Update()
	-- mcore.test();
	-- print('AppObj:Update')
end

function printDebugInfo(obj)
	print('Inspecting ', obj)
	obj = _G[obj]
	local d = obj
	print('Type = ' , type(d))
	if type(d) == 'table' then
		for k, v in pairs(d) do
			print(k,v)
		end
	end
	print("Inspecting done 123")
end


function checkEsNodeMethod(node)
	print("start checking node ####################: ", node)
	print(node['name'])
	local methods = {'setScale',
		'setColor',
		'setTransform',
		'setRotate',
		'setVisible',
		'setFaceDefaultColor', -- BlenderNode method
	}
	-- print(type(node))		--userdata
	-- 
	-- assert('table' == type(node) , 'must be table')
	-- local m = getmetatable(node)
	-- print('m = ', m)
	for i=1,#methods do
		local name = methods[i]
		if node[name] then
			print(name, " [OK]")
		else
			print(name, " [MISSING]")
		end
	end
	print("\nChecking done.<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>")
end

-- function initView()
-- 	local root = ddcore.GetEsNodeRoot()
-- 	-- local blenderNormal = dd.createBlenderNormal('objects/uvsphere.json', true)
-- 	-- blenderNormal:setScale(0.2, 0.2, 0.2)
-- 	-- print("Meta (blenderNormal) is ", getmetatable(blenderNormal))
-- 	local axis = dd.createAxisNode()
-- 	-- print("Meta (axis) is ", getmetatable(axis))
-- 	local blenderNode = dd.createBlenderNode('objects/cube.json', false)
-- 	print(blenderNode)
-- 	print('metatable is ', getmetatable(blenderNode))
-- 	print('metatable.__index is ', getmetatable(blenderNode).__index)
-- 	print('metatable\'s metatable is ', getmetatable(getmetatable(blenderNode)))
-- 	checkEsNodeMethod(blenderNode)
-- 	blenderNode:setScale(0.1, 0.1, 0.1)
-- 	if not blenderNode.setFaceDefaultColor then
-- 		print "No such method"
-- 	else
-- 		print("yes, there is this method")
-- 		print(blenderNode.setFaceDefaultColor)
-- 		print(">>>")
-- 		print("About to set-face-default-color")
-- 		 blenderNode:setFaceDefaultColor( 0.3, 0.5 , 0.7, 0.5)
-- 	end
-- 	root:addSub(blenderNode)
-- 	print("Job Done")
-- end


---- [[
function initView()
	local root = ddcore.GetEsNodeRoot()
	local light = dd.createLightNode(0.8, 0.8, 0.8)
	light:setVAngleSpeed(0.9)
	light:setHAngleSpeed(0.6)
	light:setScale(0.01,0.01, 0.01)
	root:addSub(light)
	ddcore.SetLight(light)

	light = dd.createLightNode(0.5, 0.6, 0.45)
	light:setScale(0.01, 0.01, 0.01)
	root:addSub(light)
	ddcore.SetLight(light)

	-- Test of BlenderParticle
	-- root:addSub(dd.createBlenderParticle("objects/uvsphere8.json", 60000, 0.03))
	-- do return end
	-- root:addSub(dd.createParticleNode(50000))

	-- ParticldeNode test
	-- root:addSub(dd.createParticleNode())
	-- do return end

	--[[
	local colors = {
		{0,0,1,0.5},
		{0,1,1,1},
		{1,0,0,0.5}
	}
	if true then
		for i=1, #colors do
			-- print(i)
			local v = i - 2
			local box = dd.createBlenderNormal('objects/uvsphere.json')
			box:setTransform( v * 0.75, 0, 0)
			box:setScale(0.2, 0.2, 0.2)
			box:setColor(colors[i][1], colors[i][2], colors[i][3], colors[i][4])
			root:addSub(box)
		end
	end
	root:addSub(dd.createAxisNode())
	--]]

	-- 	for(int i=-5;i<=0;++i){
	-- 	auto one = CubeNode::create(0.15 * i , 0.15 * i, 0.2, 0.2, 0.2);
	-- 	float scale = abs(i) * 0.07 + 0.3;
	-- 	//one->setScale(scale,scale,scale);
	-- 	one->shiftXZ();
	-- 	one->setColor(glm::vec4(0.7, 0.3, 0.3, 0.5));
	-- 	root->addSub(one);
	-- }
	--[[
	for i=-5,0 do
		-- local cube = dd.createCubeNode(0.15 * i, 0.15 * i, 0.1, 0.1, 0.1)
		cube = dd.createCubeNode(0.15 * i, 0.15 * i)  
		-- print("Creating with default xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
		cube:shiftXZ()
		cube:setColor(0.7, 0.3, 0.3, 0.5)
		root:addSub(cube)
	end
	--]]

	--[[
	local mimic = dd.createMimicNode()
	mimic:setScale(0.025,0.025,0.025);
	mimic:setTransform(0, 0, 0);
	root:addSub(mimic)
	--]]
end
--]]

function AppObj:OnAppReady()
	print('App is ready(Lua)')
	printDebugInfo('ddcore')
	printDebugInfo('DDGraphics_EsNode')
	-- local meta = getmetatable(light)
	-- print('LightNode\'s meta is ', meta.__name)
	initView()
	print("Finished onAppReady")
end

function AppObj:OnAppDidFinished()

end

function AppObj:doTest()
	print(self.name, ' says hello')
	mcore.test()
end

math.randomseed(os.time())
-- return 100
return AppObj
