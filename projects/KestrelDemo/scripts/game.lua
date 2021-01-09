--
-- Copyright (c) 2020 Tom Hancocks
--

local c = Canvas(Size(300, 300))
c:clear()

c.penColor = Color.red()
c:fillRect(Rect(0, 0, 100, 100))

c.penColor = Color(0, 255, 0, 128)
c:fillRect(Rect(50, 50, 100, 100))

c.penColor = Color(200, 200, 200, 255)
c:drawLine(Point(50, 50), Point(250, 250))

--c.penColor = Color.yellow()
c:setFont("Geneva", 13)
local textSize = c:layoutText("Hello, World")
c:drawText(Point(100, 100))

local e = c:spawnEntity(Vec2(200, 200))

local s = Scene.current()
s:render(function()
    e:draw()
end)