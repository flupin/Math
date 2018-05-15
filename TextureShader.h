#pragma once
#include "GDIHelper.h"
#include "Vector.h"
#include "Renderer.h"
#include "Matrix.h"
#include "Texture.h"
#include "Mesh.h"

struct vInput
{
	Vector3 position;
	Vector2 tex;
};
struct vOutput
{
	Vector3 position;
	Vector2 tex;
};
struct pInput
{
	Vector2 tex;
};
class TextureShader
{
private :
	Matrix3 worldMatrix;
	Texture* texture;

	vOutput VShader(vInput input);
	ULONG PShader(pInput input);
public :
	TextureShader()
	{
		worldMatrix.SetIdentity();
		texture = 0;
	}

	void Render(Mesh* mesh);
	void SetTexture(Texture* texture);
	void SetWorldMatrix(Matrix3 worldMatrix);
};