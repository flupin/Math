
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include "Mesh.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + pt.X + g_nClientWidth * -pt.Y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3& start, const Vector3& end)
{
	float length = (end - start).Dist();
	float inc = 1.0f / length;

	int maxLength = RoundToInt(length);
	for (int i = 0; i <= maxLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 Pt = start * (1.0f - t) + end * t;
		PutPixel(Pt.ToIntPoint());
	}

}

void Draw2DTriangle(Triangle t)
{
	for (int y = RoundToInt(t.sbbMin.Y); y < RoundToInt(t.sbbMax.Y); y++)
	{
		for (int x = RoundToInt(t.sbbMin.X); x < RoundToInt(t.sbbMax.X); x++)
		{
			Vector3 target((float)x, (float)y, 0.0f);
			float outS, outT;
			t.CalcBaryCentricCoord(target, &outS, &outT);
			if (t.IsInTrianble(outS, outT))
			{
				ULONG vColor = t.GetPixelColor(target, outS, outT);
				if (g_Texture->IsLoaded())
				{
					g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, t);
				}

				PutPixel(IntPoint(x, y));
			}			
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	//if (g_Texture->IsLoaded())
	//{		
	//	int width = g_Texture->width;
	//	ULONG Color32Bit = g_Texture->GetPixel(9, 1);
	//	BYTE* Color = (BYTE *)&Color32Bit;
	//	BYTE BValue = Color[0];
	//	BYTE GValue = Color[1];
	//	BYTE RValue = Color[2];
	//	BYTE AValue = Color[3];
	//}

	Vector3 Pt1, Pt2, Pt3, Pt4;

	static float offsetX = 0.0f;
	//static float offsetY = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(offsetX, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	Pt1.SetPoint(-150, 150.0f);
	Pt2.SetPoint(150.0f, 150.0f);
	Pt3.SetPoint(150.0f, -150.0f);
	Pt4.SetPoint(-150.0f, -150.0f);

	Vertex v1(Pt1);
	v1.uv = Vector2(0.0f, 0.0f);
	Vertex v2(Pt2);
	v2.uv = Vector2(1.0f, 0.0f);
	Vertex v3(Pt3);
	v3.uv = Vector2(1.0f, 1.0f);
	Triangle T1(v1, v2, v3);

	Vertex v4(Pt4);
	v4.uv = Vector2(0.0f, 1.0f);

	int triangleNum = 2;
	int indexCount = triangleNum * 3;
	int i;
	unsigned long *indices = new unsigned long[indexCount];
	if (!indices) {
		return;
	}
	for (i = 0; i < indexCount; i++) {
		indices[i] = i;
	}

	Vertex* vert = new Vertex[indexCount];
	if (!vert) {
		return;
	}
	vert[0] = v1;
	vert[1] = v2;
	vert[2] = v3;
	vert[3] = v1;
	vert[4] = v3;
	vert[5] = v4;

	Mesh mesh;
	mesh.Initialize(vert, indices, 6);
	mesh.RenderMesh(TRSMat, g_Texture);
	// Buffer Swap 
	BufferSwap();
}
