#include <bullet.h>

void Bullet::render(gl2d::Renderer2D &renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding &bulletAtlas)
{
    float spaceShipAngle = atan2(fireDirection.y, -fireDirection.x);
    spaceShipAngle = glm::degrees(spaceShipAngle) + 90.f;

    renderer.renderRectangle({position - glm::vec2(25,25), 40,40},
		bulletsTexture, Colors_White, {}, spaceShipAngle, bulletAtlas.get(1, 0));
}

void Bullet::update(float deltaTime)
{
    position += fireDirection * deltaTime * bulletSpeed;
}