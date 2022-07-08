-- Copyright (c) 2020-2021 Tom Hancocks
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

-- Fetch a random text resource that can be displayed in the window.
local messages = {}
Namespace.Example:typedResource('text'):matchingResources():each(function(textRef)
    local data = Legacy.Macintosh.Resource.Reader(textRef)
    if not data.isValid then
        return
    end
    messages[#messages + 1] = data:readCStr()
end)

-- Create a canvas and render a random message on to it.
local message = messages[math.floor(math.random() * #messages) + 1]
local messageCanvas = Canvas(Size(1280, 800))
messageCanvas.penColor = Color.white()
messageCanvas:setFont("Geneva", 15)
messageCanvas:layoutTextInBounds(message, Size(1200, 800))
messageCanvas:drawText(Point(40, 40))
local messageEntity = messageCanvas:spawnEntity(Vec2(0, 0))

-- Setup a scene render block so that we can draw the entity.
Scene.current():render(function()
    messageEntity:draw()
end)