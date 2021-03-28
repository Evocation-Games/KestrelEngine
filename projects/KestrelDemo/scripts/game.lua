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
c:setFont("Geneva", 11)
c.penColor = Color.colorValue(0xFFFFFF)
local textSize = c:layoutText("abcdefghijklmnopqrstuvwayz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789")
c:drawText(Point(100.5, 100))

local e = c:spawnEntity(Vec2(200, 200))

local s = Scene.current()
s:render(function()

    s:onKeyEvent(function(event)
        if event.pressed and event.keycode == Key.ESCAPE then
            print("Escaping!")
        end
    end)

    e:draw()
end)