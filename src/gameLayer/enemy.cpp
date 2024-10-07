#include <enemy.h>
#include <tileRenderer.h>

void Enemy::render(gl2d::Renderer2D &renderer, gl2d::Texture &sprites, gl2d::TextureAtlasPadding &atlas)
{
    // renderSpaceShip(renderer, position, enemyShipSize, sprites, atlas.get(type.x, type.y), angle);

    float spaceShipAngle = atan2(viewDirection.y, -viewDirection.x);

    renderer.renderRectangle(
        {position - glm::vec2(enemyShipSize / 2, enemyShipSize / 2), enemyShipSize, enemyShipSize},
        sprites,
        Colors_White,
        {},
        glm::degrees(spaceShipAngle) - 90.0f,
        atlas.get(type.x, type.y));
}
void Enemy::update(float deltaTime, glm::vec2 playerPos)
{
    glm::vec2 directionToPlayer = playerPos - position;
    if (glm::length(directionToPlayer) == 0)
    {
        directionToPlayer = {1, 0};
    }
    else
    {
        directionToPlayer = glm::normalize(directionToPlayer);
    }

    glm::vec2 newDirection = {};
    if (glm::length(directionToPlayer + viewDirection) <= 0.1)
    {
        newDirection = glm::vec2(directionToPlayer.y, -directionToPlayer.x);
    }
    else
    {
        newDirection = deltaTime * turnSpeed * directionToPlayer + viewDirection;
    }
    viewDirection = glm::normalize(newDirection);

    // Apply acceleration
    glm::vec2 acceleration = viewDirection * accelerationRate;
    glm::vec2 velocity = speed * acceleration * deltaTime;

    // Cap the velocity to maxSpeed
    if (glm::length(velocity) > maxSpeed)
    {
        velocity = glm::normalize(velocity) * maxSpeed;
    }

    position += velocity * deltaTime;
}