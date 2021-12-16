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

math.randomseed(os.time())
math.random(); math.random(); math.random()

-- Configure the size of the game window.
Kestrel.setGameWindowTitle("Kestrel Example Project")
Kestrel.setGameWindowSize(1280, 800)

-- Build up a reference of all of the namespaces that we've defined in the kdlproj.
-- This allows us quick access in the future.
Namespace = {
    Example = Namespace("example")
}

-- We're going to construct a game scene and then present it. We'll define a helper
-- function for this, for presenting future scenes if required.
presentScene = function(ns)
    local ref = ns:typedIdentifiedResource("LuaS", 128):bestResource()
    if (not ref or not ref:valid()) then
        abort("Invalid script reference for scene.")
    end

    Kestrel.scene("scene.name", ref):present()
end

presentScene(Namespace.Example)