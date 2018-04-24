#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() {}
	Vertex(Vector3 v) { position = v; }
	Vector3 position;
	ULONG color;
	Vector2 uv;
};

struct Triangle
{
public:
	Triangle() {}
	Triangle(Vertex vert1, Vertex vert2, Vertex vert3)
	{
		vt[0] = vert1;
		vt[1] = vert2;
		vt[2] = vert3;

		sbbMin = Vector2(INFINITY, INFINITY);
		sbbMax = Vector2(-INFINITY, -INFINITY);

		for (int i = 0; i < 3; i++)
		{
			Vector3 t = vt[i].position;
			if (t.X < sbbMin.X) sbbMin.X = t.X;
			if (t.Y < sbbMin.Y) sbbMin.Y = t.Y;
			if (t.X > sbbMax.X) sbbMax.X = t.X;
			if (t.Y > sbbMax.Y) sbbMax.Y = t.Y;
		}

		u = (vt[1].position - vt[0].position).ToVector2();
		v = (vt[2].position - vt[0].position).ToVector2();
		dotUU = u.Dot(u);
		dotUV = u.Dot(v);
		dotVV = v.Dot(v);
		invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);
	}

	Vertex vt[3];

	Vector2 u;
	Vector2 v;
	Vector2 sbbMin;
	Vector2 sbbMax;
	float dotUU, dotUV, dotVV;
	float invDenom;

	void CalcBaryCentricCoord(Vector3 target, float *outS, float *outT) 
	{
		Vector2 w = (target - vt[0].position).ToVector2();
		float dotUW = u.Dot(w);
		float dotVW = v.Dot(w);
		*outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
		*outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;
	}

	bool IsInTrianble(float s, float t)
	{
		if (s < 0.0f) return false;
		if (t < 0.0f) return false;
		if (s + t > 1.0f) return false;
		return true;
	}

	ULONG GetPixelColor(Vector3 target, float s, float t)
	{
		BYTE RV0 = GetRValue(vt[0].color);
		BYTE RV1 = GetRValue(vt[1].color);
		BYTE RV2 = GetRValue(vt[2].color);

		BYTE GV0 = GetGValue(vt[0].color);
		BYTE GV1 = GetGValue(vt[1].color);
		BYTE GV2 = GetGValue(vt[2].color);

		BYTE BV0 = GetBValue(vt[0].color);
		BYTE BV1 = GetBValue(vt[1].color);
		BYTE BV2 = GetBValue(vt[2].color);

		BYTE FinalR = RoundToInt(RV0 * (1.0f - s - t) + RV1 * s + RV2 * t);
		BYTE FinalG = RoundToInt(GV0 * (1.0f - s - t) + GV1 * s + GV2 * t);
		BYTE FinalB = RoundToInt(BV0 * (1.0f - s - t) + BV1 * s + BV2 * t);
		return RGB(FinalB, FinalG, FinalR);
	}

};

