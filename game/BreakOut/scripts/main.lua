--
-- Copyright (c) 2020 Tom Hancocks
--

-- Setup the game window
Kestrel.setGameWindowTitle("Break Out")
Kestrel.setGameWindowSize(1040, 800)

-- Create the a new game session
local game = Kestrel.scene("MainGame", Resource.id(128))
game:present()
