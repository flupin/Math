#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"
#include "Mesh.h"
#include "Sprite.h"

#include <algorithm>
using namespace std;

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * pt.Y) + (halfWidth + pt.X);
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

Mesh mesh;

Texture* bono_tex;
Texture* bear_tex;
Sprite bono_spr;
Sprite bear_spr;
Sprite** sprites;
int spritesSize;

void InitFrame(void)
{
	int vertexSize, indexSize;
	// 텍스쳐 초기화
	bear_tex = new Texture();
	bear_tex->LoadBMP("test.bmp");
	bono_tex = new Texture();
	bono_tex->LoadBMP("est.bmp");

	// 메테리얼 초기화
	bear_spr.drawLayer = 1;
	bear_spr.SetTexture(bear_tex);
	bono_spr.drawLayer = 1;
	bono_spr.SetTexture(bono_tex);

	spritesSize = 2;
	sprites = new Sprite*[spritesSize];
	sprites[0] = &bear_spr;
	sprites[1] = &bono_spr;

	std::sort(sprites, sprites + spritesSize, Sprite::comp);

	// 메쉬 초기화
	// 점 생성
	Vector3 p1, p2, p3, p4;

	p1.SetPoint(-50.0f, 50.0f);
	p2.SetPoint(50.0f, 50.0f);
	p3.SetPoint(50.0f, -50.0f);
	p4.SetPoint(-50.0f, -50.0f);

	vertexSize = 4;
	indexSize = 6;

	// Vertex
	mesh.vertexSize = vertexSize;
	mesh.vertices = new Vertex[vertexSize];
	mesh.vertices[0].position = p1;
	mesh.vertices[0].uv = Vector2(0.125f, 0.125f);
	mesh.vertices[0].color = RGB(255, 255, 255);

	mesh.vertices[1].position = p2;
	mesh.vertices[1].uv = Vector2(0.25f, 0.125f);
	mesh.vertices[1].color = RGB(255, 255, 255);

	mesh.vertices[2].position = p3;
	mesh.vertices[2].uv = Vector2(0.25f, 0.25f);
	mesh.vertices[2].color = RGB(255, 255, 255);

	mesh.vertices[3].position = p4;
	mesh.vertices[3].uv = Vector2(0.125f, 0.25f);
	mesh.vertices[3].color = RGB(255, 255, 255);

	// Index
	mesh.indexSize = indexSize;
	mesh.indices = new unsigned int[indexSize];
	mesh.indices[0] = 0;
	mesh.indices[1] = 1;
	mesh.indices[2] = 2;
	mesh.indices[3] = 2;
	mesh.indices[4] = 3;
	mesh.indices[5] = 0;
}

void UpdateFrame(void)
{
	SetColor(32, 128, 255);
	Clear();

	static float pos = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) pos -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) pos += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		bear_spr.drawLayer = 0;
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}

	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		bono_spr.drawLayer = 2;
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(pos, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	bear_spr.SetMatrix(TRSMat);

	for (int i = 0; i < spritesSize; i++)
	{
		sprites[i]->Render(&mesh);
	}

	// Buffer Swap 
	BufferSwap();
}
