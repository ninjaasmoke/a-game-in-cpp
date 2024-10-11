#pragma once
#include <gl2d/gl2d.h>

struct Bullet {
    glm::vec2 position = {};
    glm::vec2 velocity = {};
    glm::vec2 fireDirection = {};

    float bulletSpeed = 1000.0f;

    void render(gl2d::Renderer2D &renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding &bulletAtlas);

    void update(float deltaTime);
};