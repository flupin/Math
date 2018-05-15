#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
{
	vertices = 0;
	indices = 0;
}

//void Mesh::CreateTriangle()
//{
//	triangleCount = (int)(sizeof(vertex) / sizeof(*vertex)) / 3;
//	triangle = new Triangle[triangleCount];
//
//	for (int i = 0; i < triangleCount; i++)
//	{
//		triangle[i] = Triangle(vertex[i * 3], vertex[i * 3 + 1], vertex[i * 3 + 2]);
//	}
//}