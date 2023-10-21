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

MouseInteractionTests = Scene.current

createColorEntity = function(frame, color)
    local canvas = Canvas(frame.size)
    local entity = SceneEntity(canvas)
    entity.position = frame.origin

    canvas.penColor = color
    canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
    canvas:rebuildEntityTexture()

    return entity
end

createInteractableEntity = function(frame, color)
    local canvas = Canvas(frame.size)
    local entity = SceneEntity(canvas)
    entity.position = frame.origin

    canvas.penColor = color
    canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
    canvas:rebuildEntityTexture()

    entity:onMouseEnter(function()
        canvas.penColor = Color(200, 0, 0, 255)
        canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
        canvas:rebuildEntityTexture()
    end)
    
    entity:onMouseExit(function()
        canvas.penColor = Color(0, 200, 0, 255)
        canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
        canvas:rebuildEntityTexture()
    end)

    return entity
end

-- Add some buttons with different anchor points to test coordinate transformations
button = createInteractableEntity(Rect(100, 100, 200, 200), Color(0, 200, 0, 255))
button.anchorPoint = AxisOrigin.TopLeft
MouseInteractionTests:addEntity(button)

button2 = createInteractableEntity(Rect(100, 100, 200, 200), Color(0, 200, 0, 255))
button2.anchorPoint = AxisOrigin.BottomLeft
MouseInteractionTests:addEntity(button2)

-- Add a "window" in order to test child elements
window = createColorEntity(Rect(300, 50, 250, 800), Color(255, 255, 70, 255))
window.anchorPoint = AxisOrigin.TopRight
MouseInteractionTests:addEntity(window)

button3 = createInteractableEntity(Rect(10, 10, 100, 50), Color(0, 200, 0, 255))
button3.anchorPoint = AxisOrigin.TopLeft
window:addChildEntity(button3)