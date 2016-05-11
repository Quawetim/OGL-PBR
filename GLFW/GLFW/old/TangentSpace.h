#ifndef TANGENTSPACE_H
#define TANGENTSPACE_H
using namespace std;
using namespace glm;

void computeTangentBasis(vector<vec3> &vertices, vector<vec2> &uvs, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents);

#endif