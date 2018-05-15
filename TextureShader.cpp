#include "stdafx.h"
#include "TextureShader.h"

vOutput TextureShader::VShader(vInput input)
{
	vOutput output;

	output.position = input.position * worldMatrix;
	output.tex = input.tex;

	return output;
}
ULONG TextureShader::PShader(pInput input)
{
	ULONG color;
	color = texture->GetTexturePixel(input.tex);

	return color;
}

void TextureShader::Render(Mesh* mesh)
{
	vInput vsInput;
	pInput psInput;
	for (int i = 0; i < mesh->indexSize; i += 3)
	{
		vOutput vOutput[3];
		Vertex vertex[3];
		for (int j = 0; j < 3; j++)
		{
			vsInput.position = mesh->vertices[mesh->indices[i + j]].position;
			vsInput.tex = mesh->vertices[mesh->indices[i + j]].uv;
			vOutput[j] = VShader(vsInput);
			vertex[j].position = vOutput[j].position;
			vertex[j].uv = vOutput[j].tex;
		}

		Triangle tri(vertex[0], vertex[1], vertex[2]);

		Vector2 UV0 = tri.vt[0].uv;
		Vector2 UV0ToUV1 = tri.vt[1].uv - tri.vt[0].uv;
		Vector2 UV0ToUV2 = tri.vt[2].uv - tri.vt[0].uv;

		for (int y = tri.Min.Y; y <= tri.Max.Y; y++)
		{
			for (int x = tri.Min.X; x <= tri.Max.X; x++)
			{
				Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
				float outS, outT;
				tri.CalcBaryCentricCoord(target, &outS, &outT);
				if (tri.IsInTrianble(outS, outT))
				{
					Vector2 UVResult = UV0 + UV0ToUV1 * outS + UV0ToUV2 * outT;
					psInput.tex = UVResult;

					g_CurrentColor = PShader(psInput);
					PutPixel(IntPoint(x, y));
				}
			}
		}
	}
}

void TextureShader::SetTexture(Texture* texture)
{
	this->texture = texture;
}
void TextureShader::SetWorldMatrix(Matrix3 worldMatrix)
{
	this->worldMatrix = worldMatrix;
}