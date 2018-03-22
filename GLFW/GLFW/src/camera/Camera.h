#pragma once
#include "..\includes\Includes.h"

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 lookTo;
    glm::vec3 up;

    float speed;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    void move();

public:
    Camera();
    ~Camera();

    glm::mat4 getProjectionMatrix();

    glm::mat4 getViewMatrix();
};

// First person camera
class FPC : Camera
{

};

// Third person camera
class TPC : Camera
{

};

class Static : Camera
{

};