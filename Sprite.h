#pragma once
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureShader.h"

class Sprite
{
private:
	TextureShader * shader;
	Mesh *mesh;
public:
	Sprite() { shader = new TextureShader; }

	int drawLayer;
	void set_mesh(Mesh* _mesh)
	{
		mesh = _mesh;
	}
	void Render()
	{
		shader->Render(mesh);
	}
	void Render(Mesh* mesh)
	{
		shader->Render(mesh);
	}
	void SetMatrix(Matrix3 matrix)
	{
		shader->SetWorldMatrix(matrix);
	}
	void SetTexture(Texture* texture)
	{
		shader->SetTexture(texture);
	}

	static bool comp(const Sprite* a, const Sprite* b)
	{
		return (a->drawLayer > b->drawLayer);
	}
};