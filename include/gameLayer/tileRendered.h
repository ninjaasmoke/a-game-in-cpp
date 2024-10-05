#pragma once
#include <gl2d/gl2d.h>

struct TileRenderer
{
    float backgroundSize = 1080*4;
    gl2d::Texture texture;

    float parallaxStrength = 1.0;

    void render(gl2d::Renderer2D &renderer);
};