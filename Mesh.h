#pragma once
#include "Triangle.h"
#include "Matrix.h"

struct Mesh
{
public:
	Mesh();

	int vertexSize;
	int indexSize;
	Vertex* vertices = 0;
	unsigned int* indices;
};