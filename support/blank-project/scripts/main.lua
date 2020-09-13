--
-- Copyright (c) %%YEAR %%NAME
--

-- Setup the game window
Kestrel.setGameWindowTitle("%%PROJECT_NAME")
Kestrel.setGameWindowSize(1280, 800)

-- Create the a new game session
local game = Kestrel.scene("MainGame", Resource.id(128))
game:present()