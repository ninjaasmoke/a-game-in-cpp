#define GLM_ENABLE_EXPERIMENTAL

#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tileRendered.h>

struct GamePlayData
{
	glm::vec2 playerPos = {0, 0};
	float playerAngle = 0.0f;
};

GamePlayData gamePlayData;

gl2d::Renderer2D renderer;

constexpr int BACKGROUND_COUNT = 3;

gl2d::Texture spaceshipTexture;
gl2d::TextureAtlasPadding spaceshipAtlas;

gl2d::Texture backgroundTextures[BACKGROUND_COUNT];

TileRenderer tileRenderers[BACKGROUND_COUNT];

bool initGame()
{
	// initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceshipTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", 1, true);

	spaceshipAtlas = gl2d::TextureAtlasPadding(5, 2, spaceshipTexture.GetSize().x, spaceshipTexture.GetSize().y);

	backgroundTextures[0].loadFromFile(RESOURCES_PATH "backgrounds/level_1/dust.png", true);
	backgroundTextures[1].loadFromFile(RESOURCES_PATH "backgrounds/level_1/stars.png", true);
	backgroundTextures[2].loadFromFile(RESOURCES_PATH "backgrounds/level_1/planets.png", true);

	tileRenderers[0].texture = backgroundTextures[0];
	tileRenderers[0].parallaxStrength = 0.0;

	tileRenderers[1].texture = backgroundTextures[1];
	tileRenderers[1].parallaxStrength = 0.1;

	tileRenderers[2].texture = backgroundTextures[2];
	tileRenderers[2].parallaxStrength = 0.7;

	return true;
}

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0;
	int h = 0;
	w = platform::getFrameBufferSizeX(); // window w
	h = platform::getFrameBufferSizeY(); // window h

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); // clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion

#pragma region render background

	for (int i = 0; i < BACKGROUND_COUNT; i++)
	{
		tileRenderers[i].render(renderer);
	}

#pragma endregion

#pragma region render ship

	constexpr float shipSize = 100.0f;

	renderer.renderRectangle({gamePlayData.playerPos - glm::vec2(shipSize / 2, shipSize / 2), shipSize, shipSize}, spaceshipTexture, Colors_White, {}, glm::degrees(gamePlayData.playerAngle) - 90.0f, spaceshipAtlas.get(1, 1));

#pragma endregion

	renderer.currentCamera.follow(gamePlayData.playerPos, deltaTime * 100, 10, 200, w, h);

#pragma region movement

	glm::vec2 movement = {0, 0};

	if (
		platform::isButtonHeld(platform::Button::W) ||
		platform::isButtonHeld(platform::Button::Up))
	{
		movement.y -= 1;
	}
	if (
		platform::isButtonHeld(platform::Button::A) ||
		platform::isButtonHeld(platform::Button::Left))
	{
		movement.x -= 1;
	}
	if (
		platform::isButtonHeld(platform::Button::S) ||
		platform::isButtonHeld(platform::Button::Down))
	{
		movement.y += 1;
	}
	if (
		platform::isButtonHeld(platform::Button::D) ||
		platform::isButtonHeld(platform::Button::Right))
	{
		movement.x += 1;
	}

	static glm::vec2 velocity = {-100, -100};
	const float acceleration = 500.0f; // Acceleration factor
	const float maxSpeed = 500.0f;	   // Maximum speed

	if (movement.x != 0 || movement.y != 0)
	{
		// movement = glm::normalize(movement);

		velocity += movement * acceleration * deltaTime;

		if (glm::length(velocity) > maxSpeed)
		{
			velocity = glm::normalize(velocity) * maxSpeed;
		}

		// Update player angle
		gamePlayData.playerAngle = atan2(velocity.y * 2, -velocity.x * 2);
	}
	else
	{
		velocity *= 0.99f;
		if (glm::length(velocity) < 0.1f)
		{
			velocity = {0, 0};
		}
	}

	gamePlayData.playerPos += velocity * deltaTime;

	// renderer.renderRectangle({gamePlayData.playerPos, 100, 100}, spaceshipTexture);

	renderer.flush();

	// ImGui::ShowDemoWindow();

	return true;
#pragma endregion
}

// This function might not be be called if the program is forced closed
void closeGame()
{
}
