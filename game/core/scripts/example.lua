-- Copyright (c) 2020 Tom Hancocks
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

local scene = Scene.current()

print("Loading scene...")

local img = SpriteSheet.load(Resource.id(128))
local ship = img:spawnEntity(Vec2(600, 400))

local text = Text("Kestrel Engine Test")
text.fontSize = 12
text.font = "Geneva"
text.position = Point(50, 50)
text.color = Color.red()

local name = Text("Voinian Frigate")
name.fontSize = 18
name.font = "Geneva"
name.position = Point(50, 60)
name.color = Color.lime()

local rotation = 0

scene:render(function()
    ship:draw()
    text:draw()
    name:draw()
    ship.frame = rotation
end)

scene:onKeyEvent(function(event)
    if event.keycode == 262 and (event.pressed or event.held) then
        -- Right
        rotation = rotation - 1
        if rotation < 0 then
            rotation = 89
        end
    elseif event.keycode == 263 and (event.pressed or event.held) then
        -- Left
        rotation = rotation + 1
        if rotation > 89 then
            rotation = 0
        end
    end
end)
