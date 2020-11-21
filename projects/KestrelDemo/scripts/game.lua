--
-- Copyright (c) 2020 Tom Hancocks
--

local c = Canvas(Size(300, 100))

c.penColor = Color.white()
c:setFont("Times", 20)
c:drawText("Hello, World", Point(10, 10))

local e = c:spawnEntity(Vec2(200, 100))

local s = Scene.current()
s:render(function()
    e:draw()
end)