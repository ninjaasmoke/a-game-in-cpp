#include "tileRenderer.h"

void TileRenderer::render(gl2d::Renderer2D &renderer)
{
    auto viewRect = renderer.getViewRect();

    glm::vec2 parallaxDistance{viewRect.x, viewRect.y};
    parallaxDistance *= -parallaxStrength;

    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            int posX = x + int((viewRect.x - parallaxDistance.x) / backgroundSize);
            int posY = y + int((viewRect.y - parallaxDistance.y) / backgroundSize);

            renderer.renderRectangle(glm::vec4{glm::vec2{posX, posY}, 1, 1} * backgroundSize + glm::vec4(parallaxDistance, 0, 0), texture);
        }
    }
}

void renderSpaceShip(
    gl2d::Renderer2D &renderer,
    glm::vec2 position,
    float size,
    gl2d::Texture texture,
    glm::vec4 uvs,
    float angle)
{

	renderer.renderRectangle(
        {position - glm::vec2(size / 2, size / 2), size, size},
        texture,
        Colors_White,
        uvs,
        glm::degrees(angle) - 90.0f
    );
}