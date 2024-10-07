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
#include <cstdio>

#include <tileRenderer.h>
#include <enemy.h>
#include <bullet.h>

struct GamePlayData
{
	glm::vec2 playerPos = {0, 0};
	float playerAngle = -0.360f;
	glm::vec2 velocity = {0, 0};

	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;

	int currentBullets = 10;
	constexpr static int MAX_BULLETS = 10;
	float timeSinceLastRefill = 0.0f;
	constexpr static float BULLET_REFILL_TIME = 3.0f;
};

GamePlayData gamePlayData;

gl2d::Renderer2D renderer;

constexpr int BACKGROUND_COUNT = 3;

gl2d::Texture spaceshipTexture;
gl2d::Texture spaceshipsTexture;
gl2d::TextureAtlasPadding spaceshipAtlas;

gl2d::Texture bulletsTexture;
gl2d::TextureAtlasPadding bulletsAtlas;

gl2d::Texture backgroundTextures[BACKGROUND_COUNT];

TileRenderer tileRenderers[BACKGROUND_COUNT];

bool initGame()
{
	std::srand(std::time(0));
	// initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceshipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/level_1.png", 1, true);

	spaceshipsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", 128, true);
	spaceshipAtlas = gl2d::TextureAtlasPadding(5, 2, spaceshipsTexture.GetSize().x, spaceshipsTexture.GetSize().y);

	bulletsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png", 500, true);
	bulletsAtlas = gl2d::TextureAtlasPadding(3, 2, bulletsTexture.GetSize().x, bulletsTexture.GetSize().y);

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

	// renderer.renderRectangle({gamePlayData.playerPos - glm::vec2(shipSize / 2, shipSize / 2), shipSize, shipSize}, spaceshipTexture, Colors_White, {}, glm::degrees(gamePlayData.playerAngle) - 90.0f);

	renderSpaceShip(renderer, gamePlayData.playerPos, shipSize, spaceshipTexture, spaceshipAtlas.get(0, 0), gamePlayData.playerAngle);

#pragma endregion

#pragma region handle bullets

	if (platform::isButtonPressedOn(platform::Button::Space) && gamePlayData.currentBullets > 0)
	{
		Bullet b;

		b.position = gamePlayData.playerPos;
		b.fireDirection = glm::vec2(cos(gamePlayData.playerAngle), -sin(gamePlayData.playerAngle));

		gamePlayData.bullets.push_back(b);

		gamePlayData.currentBullets--;
	}

	for (int i = 0; i < gamePlayData.bullets.size(); i++)
	{
		if (glm::distance(gamePlayData.bullets[i].position, gamePlayData.playerPos) > 5'000)
		{
			gamePlayData.bullets.erase(gamePlayData.bullets.begin() + i);
			i--;
			continue;
		}

		gamePlayData.bullets[i].update(deltaTime);
	}

	gamePlayData.timeSinceLastRefill += deltaTime;

	if (gamePlayData.currentBullets < gamePlayData.MAX_BULLETS && gamePlayData.timeSinceLastRefill >= gamePlayData.BULLET_REFILL_TIME)
	{
		// Add one bullet
		gamePlayData.currentBullets++;

		// Reset the timer after refilling one bullet
		gamePlayData.timeSinceLastRefill = 0.0f;
	}

#pragma endregion

#pragma region render bullets

	for (auto &b : gamePlayData.bullets)
	{
		b.render(renderer, bulletsTexture, bulletsAtlas);
	}

#pragma endregion

#pragma region handle enemies

	for (int i = 0; i < gamePlayData.enemies.size(); i++)
	{
		gamePlayData.enemies[i].update(deltaTime, gamePlayData.playerPos);
	}

#pragma endregion

#pragma region render enemies

	for (auto &e : gamePlayData.enemies)
	{
		e.render(renderer, spaceshipsTexture, spaceshipAtlas);
	}

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

	const float acceleration = 500.0f; // Acceleration factor
	const float maxSpeed = 1000.0f;	   // Maximum speed

	if (movement.x != 0 || movement.y != 0)
	{
		// movement = glm::normalize(movement);

		gamePlayData.velocity += movement * acceleration * deltaTime;

		if (glm::length(gamePlayData.velocity) > maxSpeed)
		{
			gamePlayData.velocity = glm::normalize(gamePlayData.velocity) * maxSpeed;
		}

		// Update player angle
		gamePlayData.playerAngle = atan2(gamePlayData.velocity.y * 2, -gamePlayData.velocity.x * 2);
	}
	else
	{
		gamePlayData.velocity *= 0.9f;
		if (glm::length(gamePlayData.velocity) < 0.1f)
		{
			gamePlayData.velocity = {0, 0};
		}
	}

	gamePlayData.playerPos += gamePlayData.velocity * deltaTime;

	// renderer.renderRectangle({gamePlayData.playerPos, 100, 100}, spaceshipTexture);

	renderer.flush();

	// ImGui::ShowDemoWindow();

	ImGui::Begin("Gameplay data");

	ImGui::Text("Bullets count: %d", (int)gamePlayData.bullets.size());
	ImGui::Text("Bullets count: %d", gamePlayData.currentBullets);
	ImGui::Text("Enemies count: %d", (int)gamePlayData.enemies.size());
	ImGui::Text("Player position: (%.2f, %.2f)", gamePlayData.playerPos.x, gamePlayData.playerPos.y);
	ImGui::Text("Player angle: %.2f", glm::degrees(gamePlayData.playerAngle));
	ImGui::Text("Player velocity: (%.2f, %.2f)", gamePlayData.velocity.x, gamePlayData.velocity.y);

	if (ImGui::Button("Spawn enemy"))
	{
		glm::uvec2 shipTypes[] = {{0, 0}, {0, 1}, {2, 0}, {3, 1}};
		Enemy e;
		e.position = gamePlayData.playerPos;
		e.speed = 700 + rand() % 1000;
		e.turnSpeed = 2.f + (rand() & 1000) / 500.f;
		e.type = shipTypes[rand() % 4];

		gamePlayData.enemies.push_back(e);
	}

	ImGui::End();

	return true;
#pragma endregion
}

// This function might not be be called if the program is forced closed
void closeGame()
{
}
