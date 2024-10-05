#include "tileRendered.h"

void TileRenderer::render(gl2d::Renderer2D &renderer)
{
    auto viewRect = renderer.getViewRect();

    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            int posX = x + int(viewRect.x / backgroundSize);
            int posY = y + int(viewRect.y / backgroundSize);

            renderer.renderRectangle(glm::vec4{glm::vec2{posX, posY}, 1, 1} * backgroundSize, texture);
        }
    }
}