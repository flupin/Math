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

Mesh set_plane_mesh(Vector2 _start, Vector2 _size)
{
	Mesh plane;
	plane.vertexSize = 4;
	plane.vertices = new Vertex[plane.vertexSize];

	// left up
	plane.vertices[0].position = Vector3(_start.X, _start.Y, 0.f);
	plane.vertices[0].color = RGB(255, 255, 255);
	plane.vertices[0].uv = Vector2(0.f, 0.f);

	// right up
	plane.vertices[1].position = Vector3(_start.X + _size.X, _start.Y, 0.f);
	plane.vertices[1].color = RGB(255, 255, 255);
	plane.vertices[1].uv = Vector2(1.f, 0.f);

	// right bottom
	plane.vertices[2].position = Vector3(_start.X + _size.X, _start.Y - _size.Y, 0.f);
	plane.vertices[2].color = RGB(255, 255, 255);
	plane.vertices[2].uv = Vector2(1.f, 1.f);

	// left bottom
	plane.vertices[3].position = Vector3(_start.X, _start.Y - _size.Y, 0.f);
	plane.vertices[3].color = RGB(255, 255, 255);
	plane.vertices[3].uv = Vector2(0.f, 1.f);

	plane.indexSize = 6;
	plane.indices = new unsigned int[plane.indexSize];
	plane.indices[0] = 0;
	plane.indices[1] = 1;
	plane.indices[2] = 2;
	plane.indices[3] = 2;
	plane.indices[4] = 3;
	plane.indices[5] = 0;

	return plane;
}

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

Mesh bear_mesh;
Mesh bono_mesh;

Texture* bear_tex;
Sprite bear_spr;
Texture* bono_tex;
Sprite bono_spr;
Sprite** sprites;
int spritesSize;

// initialize texture, sprite 
void init_sprite()
{
	bear_tex = new Texture();
	bear_tex->LoadBMP("test.bmp");

	bono_tex = new Texture();
	bono_tex->LoadBMP("bono.bmp");

	bear_spr.drawLayer = 0;
	bear_spr.SetTexture(bear_tex);
	bear_spr.set_mesh(&bear_mesh);

	bono_spr.drawLayer = 1;
	bono_spr.SetTexture(bono_tex);
	bono_spr.set_mesh(&bono_mesh);

	spritesSize = 2;
	sprites = new Sprite*[spritesSize];
	sprites[0] = &bear_spr;
	sprites[1] = &bono_spr;

	std::sort(sprites, sprites + spritesSize, Sprite::comp);
}

void InitFrame(void)
{
	bono_mesh = set_plane_mesh(Vector2(-100, 100), Vector2(200, 200));
	bear_mesh = set_plane_mesh(Vector2(-50, 50), Vector2(100, 100));  
	init_sprite();

}

// make and returned base option plane

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	static Vector2 pos;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState('A'))angle -= 1.0f;
	if (GetAsyncKeyState('D'))angle += 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) pos.X -= 10.f;
	if (GetAsyncKeyState(VK_RIGHT)) pos.X += 10.f;
	if (GetAsyncKeyState(VK_UP)) pos.Y += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) pos.Y -= 1.0f;

	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		bear_spr.drawLayer = 0;
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		bear_spr.drawLayer = 2;
		std::sort(sprites, sprites + spritesSize, Sprite::comp);
	}

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(pos.X, pos.Y);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	bear_spr.SetMatrix(TRSMat);

	for (int i = 0; i < spritesSize; i++)
	{
		sprites[i]->Render();
	}

	// Buffer Swap 
	BufferSwap();
}
