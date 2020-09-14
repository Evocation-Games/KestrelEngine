--
-- Copyright (c) 2020 Tom Hancocks
--

-- Setup the game window
Kestrel.setGameWindowTitle("Kestrel Demo")
Kestrel.setGameWindowSize(1280, 800)

-- Create the a new game session
local game = Kestrel.scene("MainGame", Resource.id(128))
game:present()
