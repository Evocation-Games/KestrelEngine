-- Copyright (c) 2023 Tom Hancocks
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

BasicDrawingTests = Scene.current

createColorEntity = function(frame, color)
    local canvas = Canvas(frame.size)
    local entity = SceneEntity(canvas)
    entity.position = frame.origin

    canvas.penColor = color
    canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
    canvas:rebuildEntityTexture()

    return entity
end

createTextEntity = function(origin, text)
    local textEntity = TextEntity(text)
    textEntity.textColor = Color(255, 255, 255, 255)
    textEntity.position = origin
    textEntity.anchorPoint = AxisOrigin.TopLeft
    return textEntity
end

-- Add a "window" entity that can be used to test child entities
window = createColorEntity(Rect(300, 50, 250, 800), Color(255, 255, 70, 255))
window.anchorPoint = AxisOrigin.TopRight
BasicDrawingTests:addEntity(window)

child = createColorEntity(Rect(5, 5, 240, 240), Color(0, 0, 0, 255))
child.anchorPoint = AxisOrigin.TopLeft
window:addChildEntity(child)

-- Add a message entity to test text rendering
message = createTextEntity(Point(20, 20), "The quick brown fox jumped over the lazy dogs back.")
message.anchorPoint = AxisOrigin.BottomLeft
BasicDrawingTests:addEntity(message)

-- Add an entity to the center
centerEntity = createColorEntity(Rect(0, 0, 100, 100), Color(200, 0, 0, 255))
centerEntity.anchorPoint = AxisOrigin.Center
BasicDrawingTests:addEntity(centerEntity)