#pragma once
#include "Includes.h"

class RENDERER
{
private:
    /* VAO - Vertex Array Object*/
    GLuint VAO;

    /* ”казатели дл€ шейдера*/
    GLuint TextureID, VertexBufferID, UVBufferID;
public:
    RENDERER();

    ~RENDERER();

    void DrawScene();

    void DrawText(TEXT Text, const char* buffer);

    void DrawButtons();

    void DrawWindow();
};
