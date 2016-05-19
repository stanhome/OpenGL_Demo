/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include <vector>
#include <GL/glew.h>
#include <Importer.hpp>		// c++ importer interface
#include <scene.h>			// output data structure
#include <postprocess.h>	// post processing flags

#include "ogl/math_3d.h"
#include "ogl/util.h"
#include "ogl/texture.h"

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(const Vector3f &pos, const Vector2f &tex, const Vector3f &normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};


class Mesh
{
private:
#define INVALID_METERIAL 0xFFFFFFFF

	struct MeshEntry {
		MeshEntry();
		~MeshEntry();

		void init(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

		GLuint VB;
		GLuint IB;
		unsigned int numIndices;
		unsigned int materialIndex;
	};

public:
	Mesh();
	~Mesh();

	bool loadMesh(const std::string &filename);

	void render();

private:
	bool initFromScene(const aiScene *pScene, const std::string &fileName);
	void initMesh(unsigned int index, const aiMesh *paiMesh);
	bool initMaterials(const aiScene *pScene, const std::string &filename);
	void clear();

	std::vector<MeshEntry> m_entries;
	std::vector<Texture *> m_textures;
};
