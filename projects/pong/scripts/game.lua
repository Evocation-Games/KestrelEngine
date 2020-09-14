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

print("Starting new game...")

-- Get the current scene and prepare to populate it.
local scene = Scene.current()

-- Setup a function for constructing paddles and balls.
local createPaddle = function(height)
	local image = Image(Size(20, height), Color.white())
	local paddle = image:spawnEntity(Vec2(0, 0))
	return paddle
end

local createBall = function(size)
	local image = Image(Size(size, size), Color.white())
	local ball = image:spawnEntity(Vec2(0, 0))
	return ball
end

local createScoreDigit = function()
	local image = MacintoshPicture.load(Resource.id(128))
	image:setSpriteSize(Size(50, 50))
	return image:spawnEntity(Vec2(0, 0))
end

-- Construct the score indicators
local playerScoreEntity = createScoreDigit()
playerScoreEntity.position = Vec2(600, 60)

local opponentScoreEntity = createScoreDigit()
opponentScoreEntity.position = Vec2(680, 60)

-- Construct the initial paddles
local playerPaddleSize = 100
local playerPaddle = createPaddle(playerPaddleSize)
local playerSpeed = 0
local playerScore = 0
playerPaddle.position = Vec2(30, 400 - (playerPaddleSize / 2))

local opponentPaddleSize = 100
local opponentPaddle = createPaddle(opponentPaddleSize)
local opponentSpeed = 5
local opponentScore = 0
opponentPaddle.position = Vec2(1280 - 30, 400 - (opponentPaddleSize / 2))

-- Construct the ball
local ball = createBall(20)
local ballVelocity = Vec2(0, 0)
ballAttachment = "player"
ballCounter = 0

local paddleReflection = function(ballY, paddleY, paddleHeight)
	local relativeY = paddleY - ballY
	relativeY = 1.0 / relativeY
	return relativeY * 10
end


-- Setup the drawing function for the scene
scene:render(function()
	-- Check the score and end if the max score is reached.
	if opponentScore == 9 or playerScore == 9 then
		return
	end

	-- Move the ball. This involves movement of the ball whilst it is attached to either paddle,
	-- and the movement when it is actually in play.
	if ballAttachment == "player" then
		ball.position = Vec2(playerPaddle.position.x + 21, playerPaddle.position.y)
	elseif ballAttachment == "opponent" then
		ballCounter = ballCounter - 1
		ball.position = Vec2(opponentPaddle.position.x - 21, opponentPaddle.position.y)
		if ballCounter < 0 then
			ballAttachment = nil
			ballVelocity = Vec2(-10, 0)
		end
	else
		ball.position = ball.position:add(ballVelocity)

		-- Check the bounds of the pitch, for either a bounce or a goal.
		if ball.position.y < 0 or ball.position.y > 800 then
			ballVelocity.y = -ballVelocity.y

		elseif ball.position.x < playerPaddle.position.x then
			local paddleSize = (playerPaddleSize / 2)
			if ball.position.y > (playerPaddle.position.y - paddleSize) and ball.position.y < (playerPaddle.position.y + paddleSize) then
				ballVelocity.x = -ballVelocity.x
				ballVelocity.y = paddleReflection(ball.position.y, playerPaddle.position.y)
			else
				-- Opponent Goal
				ballAttachment = "player"
				opponentScore = opponentScore + 1
			end

		elseif ball.position.x > opponentPaddle.position.x then
			local paddleSize = (opponentPaddleSize / 2)
			if ball.position.y > (opponentPaddle.position.y - paddleSize) and ball.position.y < (opponentPaddle.position.y + paddleSize) then
				ballVelocity.x = -ballVelocity.x
				ballVelocity.y = paddleReflection(ball.position.y, opponentPaddle.position.y)
			else
				-- Player Goal
				ballAttachment = "opponent"
				ballCounter = 60
				playerScore = playerScore + 1
			end

		end
	end

	-- Draw the score
	playerScoreEntity.frame = playerScore
	playerScoreEntity:draw()

	opponentScoreEntity.frame = opponentScore
	opponentScoreEntity:draw()

	-- Draw the paddles and the ball.
	playerPaddle:draw()
	opponentPaddle:draw()
	ball:draw()

	-- Move the opponent paddle, to this we're going to lag behind the position of the ball.
	local paddleMove = ball.position.y - opponentPaddle.position.y
	if paddleMove < -1 then
		paddleMove = -1
	elseif paddleMove > 1 then
		paddleMove = 1
	end

	opponentPaddle.position = Vec2(1280 - 30, opponentPaddle.position.y + (paddleMove * opponentSpeed))

end)


-- Setup mouse events
scene:onMouseEvent(function(event)

	if event.moved then
		playerSpeed = event.point.y - playerPaddle.position.y
		playerPaddle.position = Vec2(30, event.point.y)
	end

	if event.isPressed and ballAttachment == "player" then
		ballAttachment = nil
		ballVelocity = Vec2(10, playerSpeed * 2)
	end

end)