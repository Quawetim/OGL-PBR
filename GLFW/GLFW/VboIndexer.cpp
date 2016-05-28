#include "VboIndexer.h"

/* Проверяет близость точек */
bool is_near(float v1, float v2)
{
	return fabs(v1 - v2) < 0.01f;
}

/* Поиск одинаковых вершин */
/* Одинаковые, если одни позиции, одни UV, одни нормали */
bool getSimilarVertexIndex(vec3 vertex, vec2 uv, vec3 normal, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals, unsigned short &result)
{
	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		if (is_near(vertex.x, out_vertices[i].x) && is_near(vertex.y, out_vertices[i].y) && is_near(vertex.z, out_vertices[i].z) && is_near(uv.x, out_uvs[i].x) && is_near(uv.y, out_uvs[i].y) && is_near(normal.x, out_normals[i].x) && is_near(normal.y, out_normals[i].y) && is_near(normal.z, out_normals[i].z))
		{
			result = i;
			return true;
		}
	}
	return false;
}

/*
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
*/

/* Индексация вершин */
void IndexVBO(vector<vec3> vertices, vector<vec2> uvs, vector<vec3> normals, vector<vec3> tangents, vector<vec3> bitangents, vector<unsigned short> &out_indices, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals, vector<vec3> &out_tangents, vector<vec3> &out_bitangents)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		/* Для каждой новой вершины ищем одинаковую в out */
		unsigned short index;
		bool found = getSimilarVertexIndex(vertices[i], uvs[i], normals[i], out_vertices, out_uvs, out_normals, index);

		if (found)
		{
			/* Нашли такую же, дублируем индекс */
			out_indices.push_back(index);
			out_tangents[index] += tangents[i];
			out_bitangents[index] += bitangents[i];
		}
		else
		{ 
			/* Не нашли, добавляем */
			out_vertices.push_back(vertices[i]);
			out_uvs.push_back(uvs[i]);
			out_normals.push_back(normals[i]);
			out_tangents.push_back(tangents[i]);
			out_bitangents.push_back(bitangents[i]);
			out_indices.push_back((unsigned short)out_vertices.size() - 1);
		}
	}
}
