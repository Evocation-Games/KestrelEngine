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

SpriteSheet = {}
SpriteSheet.__index = SpriteSheet

setmetatable(SpriteSheet, {
    __call = function (cls, ...)
        return cls.new(...)
    end
})

function SpriteSheet.new(texture, spriteSize, frameCount)
    local self = setmetatable({}, SpriteSheet)
    self.texture = texture
    self.frameCount = frameCount
    self.spriteSize = spriteSize
    texture:initializeSpriteSheet(spriteSize, frameCount)
    return self
end

function SpriteSheet.fromMacintoshPicture(resource, spriteSize, frameCount)
    local pict = MacintoshPicture(resource)
    return SpriteSheet(pict, spriteSize, frameCount)
end

function SpriteSheet:draw(frame)
    self.texture:drawFrame({0, 0}, frame)
end