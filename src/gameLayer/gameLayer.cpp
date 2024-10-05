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

struct GamePlayData
{
	glm::vec2 playerPos = {100, 100};
};

GamePlayData gamePlayData;

gl2d::Renderer2D renderer;
gl2d::Texture spaceshipTexture;

bool initGame()
{
	// initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceshipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true);

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
	const float acceleration = 200.0f; // Acceleration factor
	const float maxSpeed = 300.0f;	   // Maximum speed

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
		velocity *= 0.99f; 
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
