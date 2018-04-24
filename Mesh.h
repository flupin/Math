#pragma once


#include "Vector.h"
#include "Triangle.h"
#include "Texture.h"
#include "GDIHelper.h"
#include "SoftRenderer.h"
#include "Renderer.h"

struct Mesh {
	Vertex* vertices;
	unsigned long* indices;
	int indicesLength;


	Mesh() {
		vertices = 0;
		indices = 0;
		indicesLength = 0;
	};
	~Mesh() {
		if (vertices) {
			delete[] vertices;
			vertices = 0;
		}
		if (indices) {
			delete[] indices;
			indices = 0;
		}
	};

	void Initialize(Vertex* vert, unsigned long* index, int indexLength);
	void RenderMesh(Matrix3 mat, Texture* tex);
};