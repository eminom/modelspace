	
local function add()
	local root = ddcore.GetEsNodeRoot()
	print("Site 1000")
	local box = dd.createBlenderNormal('objects/uvsphere.json')
	print("Site 2000")
	box:setTransform( math.random(5) * 0.15, math.random(5) * 0.15, math.random(5) * 0.15)
	box:setScale(0.2, 0.2, 0.2)

	local color = {
		math.random(100) + 120,
		math.random(100) + 120,
		math.random(100) + 120,
		255
	}
	for i=1,#color do
		color[i] = color[i] / 255
	end

	box:setColor(color[1], color[2], color[3], color[4])
	root:addSub(box)
	print("added another one")
end

function step()
	print("Step !!!")
	local root = ddcore.GetEsNodeRoot()
	for i=0, 10 do
		print(" >>> ", i)
		add()
		exp.wait(100)
	end
	print("Step is finished")
end
print("Loads done")