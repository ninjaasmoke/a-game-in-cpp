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
	glm::vec2 playerPos = {100, 100};
};

GamePlayData gamePlayData;

gl2d::Renderer2D renderer;

constexpr int BACKGROUND_COUNT = 3;

gl2d::Texture spaceshipTexture;
gl2d::Texture backgroundTextures[BACKGROUND_COUNT];

TileRenderer tileRenderers[BACKGROUND_COUNT];

bool initGame()
{
	// initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceshipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true);
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

	// renderer.currentCamera.follow(gamePlayData.playerPos, deltaTime * 100, 10, 200, w, h);

	// renderer.renderRectangle({glm::vec2{0, 0}, w, h}, backgroundTexture);

	// renderer.flush();

	for (int i = 0; i < BACKGROUND_COUNT; i++)
	{
		tileRenderers[i].render(renderer);
	}

	// return true;

#pragma endregion

	renderer.currentCamera.follow(gamePlayData.playerPos, deltaTime * 100, 10, 200, w, h);

	// renderer.renderRectangle({glm::vec2{0, 0}, w, h}, backgroundTexture);

	renderer.flush();

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

	static glm::vec2 velocity = {0, 0};
	const float acceleration = 300.0f; // Acceleration factor
	const float maxSpeed = 500.0f;	   // Maximum speed

	if (movement.x != 0 || movement.y != 0)
	{
		movement = glm::normalize(movement);

		velocity += movement * acceleration * deltaTime;

		if (glm::length(velocity) > maxSpeed)
		{
			velocity = glm::normalize(velocity) * maxSpeed;
		}
	}
	else
	{
		velocity *= 0.999f;
		if (glm::length(velocity) < 0.1f)
		{
			velocity = {0, 0};
		}
	}

	gamePlayData.playerPos += velocity * deltaTime;

	renderer.renderRectangle({gamePlayData.playerPos, 100, 100}, spaceshipTexture);

	renderer.flush();

	// ImGui::ShowDemoWindow();

	return true;
#pragma endregion
}

// This function might not be be called if the program is forced closed
void closeGame()
{
}
