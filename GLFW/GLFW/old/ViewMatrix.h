#ifndef VIEWMATRIX_H
#define VIEWMATRIX_H

using namespace glm;

mat4 getProjectionMatrix();
mat4 getViewMatrix();
mat4 getViewMatrixAxes();
void checkmove(GLFWwindow* window);
void checkmove(GLFWwindow* window, vec3 direction, vec3 right);
vec3 ComputeViewMatrix(GLFWwindow* window, int cameramode);

#endif