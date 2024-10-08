#pragma once
#include <gl2d/gl2d.h>

constexpr float enemyShipSize = 100.f;

struct Enemy
{
    glm::uvec2 type = {};
    glm::vec2 position = {};

    float speed = 50.0f;
    float turnSpeed = 1.0f;
    float maxSpeed = 1000.0f;
    float accelerationRate = 1000.0f;

    glm::vec2 wanderOffset = glm::vec2(0.0f);
    float wanderRadius = 300.0f;
    float wanderTimer = 0.0f;
    float wanderInterval = 2.0f;
    float totalTime = 0.0f;

    glm::vec2 viewDirection = {1, 0};
    float angle = 0.0f;

    void render(gl2d::Renderer2D &rendered, gl2d::Texture &sprites, gl2d::TextureAtlasPadding &atlas);

    void update(float deltaTime, glm::vec2 playerPos);
};