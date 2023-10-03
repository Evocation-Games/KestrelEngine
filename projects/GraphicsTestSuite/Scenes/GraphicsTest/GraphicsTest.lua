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

GraphicsTestScene = Scene.current

print("  -> Building Scene...")

createSquare = function(frame, color)
    local canvas = Canvas(frame.size)
    local entity = SceneEntity(canvas)
    entity.position = frame.origin

    canvas.penColor = color
    canvas:fillRect(Rect(0, 0, frame.size.width, frame.size.height))
    canvas:rebuildEntityTexture()

    return entity
end

local image = StaticImage(Container.global():identifiedResource(128))
local imageEntity = SceneEntity(image)
imageEntity.anchorPoint = AxisOrigin.Center
imageEntity.size = Size(1440, 900)
imageEntity.scalingMode = ScalingMode.AspectFill
GraphicsTestScene:addEntity(imageEntity)

local redSquare = createSquare(Rect(50, 50, 100, 100), Color(200, 0, 0, 255))

local greenSquare = createSquare(Rect(100, 100, 100, 100), Color(0, 200, 0, 255))
greenSquare.anchorPoint = AxisOrigin.TopRight

local yellowSquare = createSquare(Rect(0, 0, 100, 100), Color(200, 200, 0, 255))
yellowSquare.anchorPoint = AxisOrigin.BottomRight
GraphicsTestScene:addEntity(yellowSquare)

GraphicsTestScene:render(function()
    redSquare:draw()
    greenSquare:draw()
end)