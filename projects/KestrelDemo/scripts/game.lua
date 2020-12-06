--
-- Copyright (c) 2020 Tom Hancocks
--

local c = Canvas(Size(300, 100))

c.penColor = Color.white()
c:setFont("Times", 20)
c:layoutTextInBounds("Hello, World\nThis is a test of the drawing. This line should wrap on to a new line.", Size(300, 100))
c:drawText(Point(10, 10))

local e = c:spawnEntity(Vec2(200, 100))

local s = Scene.current()
s:render(function()
    e:draw()
end)

local r = Resource.allWithType("LuaC")
print("Found " .. r.count .. " resources")
for i = 1, r.count do
    local res = ResourceData(r:at(i))
    print(res.id .. ": " .. res.name)
end