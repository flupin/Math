#include "stdafx.h"
#include "Mesh.h"

void Mesh::Initialize(Vertex * vert, unsigned long * index, int indexLength)
{
	vertices = vert;
	indices = index;
	indicesLength = indexLength;
}

void Mesh::RenderMesh(Matrix3 mat, Texture * tex)
{
	int i, j;
	Vertex* vert = new Vertex[indicesLength];
	if (!vert) {
		return;
	}
	Triangle* tri = new Triangle[indicesLength / 3];
	if (!tri) {
		return;
	}
	for (j = 0; j < indicesLength / 3; j++) {
		for (i = 0; i < indicesLength; i++) {
			vert[i].position = vertices[indices[i]].position * mat;
			vert[i].uv = vertices[indices[i]].uv;
			vert[i].color = vertices[indices[i]].color;
		}
		tri[j] = Triangle(vert[j*3], vert[j*3+1], vert[j*3+2]);

		for (int y = RoundToInt(tri[j].sbbMin.Y); y < RoundToInt(tri[j].sbbMax.Y); y++)
		{
			for (int x = RoundToInt(tri[j].sbbMin.X); x < RoundToInt(tri[j].sbbMax.X); x++)
			{
				Vector3 target((float)x, (float)y, 0.0f);
				float outS, outT;
				tri[j].CalcBaryCentricCoord(target, &outS, &outT);
				if (tri[j].IsInTrianble(outS, outT))
				{
					ULONG vColor = tri[j].GetPixelColor(target, outS, outT);
					if (tex->IsLoaded())
					{
						g_CurrentColor = tex->GetTexturePixel(outS, outT, tri[j]);
					}
					
					PutPixel(IntPoint(x, y));
				}
			}
		}
	}
}

