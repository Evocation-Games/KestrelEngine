--
-- Copyright (c) 2020 Tom Hancocks
--

local scene = Scene.current()

-- ----------------------------------------------------------------------------

local brickSize = Size(100, 20)
local gridSize = Size(10, 10)
local gridWidth = (gridSize.width * brickSize.width)
local gridPosition = Point(52, 50)
local ballVelocity = Vec2(2, 2)

local ballSprite = SpriteSheet.load(Resource.id(129))
local ball = ballSprite:spawnEntity(Vec2(520, 400))



-- ----------------------------------------------------------------------------
-- Brick
Brick = {}
Brick.__index = Brick

setmetatable(Brick, {
	__call = function (cls, ...)
		local self = setmetatable({}, cls)
		self:__init(...)
		return self
	end
})

function Brick:__init(brickNumber)

	-- Load the brick sprites
	local color = math.floor((brickNumber / gridSize.width) / 2.0)

	local sprite = MacintoshPicture.load(Resource.id(128 + color))
	sprite:setSpriteSize(Size(100, 20))

	local gridX = math.floor(brickNumber % gridSize.width) * (brickSize.width + 4)
	local gridY = math.floor(brickNumber / gridSize.width) * (brickSize.height + 4)
	local entity = sprite:spawnEntity(Vec2(gridPosition.x + gridX, gridPosition.y + gridY))

	self.entity = entity
	self.destroyed = false

end

function Brick:draw()
	self.entity:draw()
end

function Brick:handleBallCollision()
	if self.entity:intersects(ball) then
		ballVelocity.y = -ballVelocity.y
		self.destroyed = true
	end 
end

local bricks = {}
for i = 1, 100 do
	bricks[i] = Brick(i - 1) 
end


-- ----------------------------------------------------------------------------
-- Paddle
Paddle = {}
Paddle.__index = Paddle

setmetatable(Paddle, {
	__call = function (cls, ...)
		local self = setmetatable({}, cls)
		self:__init(...)
		return self
	end
})

function Paddle:__init()

	-- Load the paddle sprite
	local sprite = SpriteSheet.load(Resource.id(128))
	self.entity = sprite:spawnEntity(Vec2(520, 760))

	-- Configure
	self.speed = 20

end

function Paddle:move(x)
	local x = self.entity.position.x + (x * self.speed)
	if x > 50 and x < 980 then
		self.entity.position = Vec2(x, 760)
	end
end

function Paddle:draw()
	self.entity:draw()
end

function Paddle:handleBallCollision()
	if self.entity:intersects(ball) then
		ballVelocity.y = -ballVelocity.y
	end 
end

local paddle = Paddle()

-- ----------------------------------------------------------------------------

scene:render(function()
	-- Update the game state
	ball.position = ball.position:add(ballVelocity)
	paddle:handleBallCollision()

	-- Draw the sprites
	for i = 1, 100 do
		if not bricks[i].destroyed then
			bricks[i]:draw()
			bricks[i]:handleBallCollision()
		end
	end

	paddle:draw()
	ball:draw()

	-- Edge detection
	if ball.position.x < 0 or ball.position.x > 1040 then
		ballVelocity.x = -ballVelocity.x
	end

	if ball.position.y < 0 or ball.position.y > 800 then
		ballVelocity.y = -ballVelocity.y
	end

end)

scene:onKeyEvent(function(event)
	if event.keycode == 263 or event.keycode == 65 then
		paddle:move(-1)
	elseif event.keycode == 262 or event.keycode == 68 then
		paddle:move(1)
	end
end)

