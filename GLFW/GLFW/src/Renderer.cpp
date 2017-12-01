#include "Renderer.h"
#include "Gui.h"

RENDERER::RENDERER() {};

RENDERER::~RENDERER() {};

void RENDERER::DrawScene()
{

}

void RENDERER::DrawText(TEXT Text, const char* buffer)
{
    Text.Render(buffer);

    glGenBuffers(1, &VertexBufferID);
    glGenBuffers(1, &UVBufferID);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, Text.getVertices.size() * sizeof(vec2), &Text.getVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
    glBufferData(GL_ARRAY_BUFFER, Text.getUVs.size() * sizeof(vec2), &Text.getUVs[0], GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLES, 0, Text.getVertices.size());

    glDisable(GL_BLEND);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VertexBufferID);
    glDeleteBuffers(1, &UVBufferID);
    glDeleteTextures(1, &TextureID);
}

void RENDERER::DrawButtons()
{

}

void RENDERER::DrawWindow()
{

}