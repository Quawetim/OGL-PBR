#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "VboIndexer.h"
#include <string.h> // for memcmp

bool is_near(float v1, float v2)
{
	return fabs(v1 - v2) < 0.01f;
}

/*
����� ���������� ������
����������, ���� ���� �������, ���� UV, ���� �������.
*/
bool getSimilarVertexIndex(vec3 &in_vertex, vec2 &in_uv, vec3 &in_normal, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals, unsigned short &result)
{
	int i;

	for (i = 0; i < out_vertices.size(); i++)
	{
		if (is_near(in_vertex.x, out_vertices[i].x) && is_near(in_vertex.y, out_vertices[i].y) && is_near(in_vertex.z, out_vertices[i].z) && is_near(in_uv.x, out_uvs[i].x) && is_near(in_uv.y, out_uvs[i].y) && is_near(in_normal.x, out_normals[i].x) && is_near(in_normal.y, out_normals[i].y) && is_near(in_normal.z, out_normals[i].z))
		{
			result = i;
			return true;
		}
	}
	return false;
}

struct PackedVertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;

	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast(PackedVertex &packed, map<PackedVertex, unsigned short> &VertexToOutIndex, unsigned short &result)
{
	map<PackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) return false;
	else
	{
		result = it->second;
		return true;
	}
}

void indexVBO_fast(vector<vec3> &in_vertices, vector<vec2> &in_uvs, vector<vec3> &in_normals, vector<unsigned short> &out_indices, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals)
{
	map<PackedVertex,unsigned short> VertexToOutIndex;

	int i;

	for (i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};		

		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) out_indices.push_back(index);
		else
		{
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void indexVBO(vector<vec3> &in_vertices, vector<vec2> &in_uvs, vector<vec3> &in_normals, vector<vec3> &in_tangents, vector<vec3> &in_bitangents, vector<unsigned short> &out_indices, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals, vector<vec3> &out_tangents, vector<vec3> &out_bitangents)
{
	int i;

	for (i = 0; i < in_vertices.size(); i++)
	{
		// ��� ������ ����� ������� ���� ���������� � out
		unsigned short index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if ( found )
		{
			// ����� ����� ��, ��������� ������
			out_indices.push_back(index);
			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
		}
		else
		{ 
			// �� �����, ���������
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_tangents.push_back(in_tangents[i]);
			out_bitangents.push_back(in_bitangents[i]);
			out_indices.push_back((unsigned short)out_vertices.size() - 1);
		}
	}
}
