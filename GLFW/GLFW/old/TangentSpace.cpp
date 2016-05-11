#include <vector>
#include <glm/glm.hpp>

#include "TangentSpace.h"

using namespace std;
using namespace glm;

void computeTangentBasis(vector<vec3> &vertices, vector<vec2> &uvs, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents)
{
	int i;
	float r;

	vec3 v0, v1, v2, deltaPos1, deltaPos2, tangent, bitangent, normal;
	vec2 uv0, uv1, uv2, deltaUV1, deltaUV2;

	for (i = 0; i < vertices.size(); i += 3)
	{
		v0 = vertices[i+0];
		v1 = vertices[i+1];
		v2 = vertices[i+2];

		uv0 = uvs[i+0];
		uv1 = uvs[i+1];
		uv2 = uvs[i+2];

		// Postion delta
		deltaPos1 = v1 - v0;
		deltaPos2 = v2 - v0;

		// UV delta
		deltaUV1 = uv1 - uv0;
		deltaUV2 = uv2 - uv0;

		r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		tangent = r*(deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
		bitangent = r*(deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	for (i = 0; i < vertices.size(); i ++)
	{
		normal = normals[i];
		tangent = tangents[i];
		bitangent = bitangents[i];
		
		// Ортогонализация
		tangent = normalize(tangent - normal * dot(normal, tangent));
		
		// Направление
		if (dot(cross(normal, tangent), bitangent) < 0.0f) tangent = tangent * -1.0f;
	}
}


