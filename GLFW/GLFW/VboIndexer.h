#ifndef VBOINDEXER_H
#define VBOINDEXER_H

using namespace std;
using namespace glm;

//void indexVBO_fast(vector<vec3> &in_vertices, vector<vec2> &in_uvs, vector<vec3> &in_normals, vector<unsigned short> & out_indices, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals);

/* Индексация вершин */
void IndexVBO(vector<vec3> vertices, vector<vec2> uvs, vector<vec3> normals, vector<vec3> tangents, vector<vec3> bitangents, vector<unsigned short> &out_indices, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals, vector<vec3> &out_tangents, vector<vec3> &out_bitangents);

#endif