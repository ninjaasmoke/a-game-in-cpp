#pragma once
#include <gl2d/gl2d.h>

struct TileRenderer
{
    float backgroundSize = 1080*6;
    gl2d::Texture texture;

    float parallaxStrength = 1.0;

    void render(gl2d::Renderer2D &renderer);
};

void renderSpaceShip(
    gl2d::Renderer2D &renderer,
    glm::vec2 position, 
    float size,
    gl2d::Texture texture,
    glm::vec4 uvs, 
    float angle  
);