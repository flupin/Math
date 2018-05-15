#pragma once

#include "stdafx.h"
#include "Vector.h"
#include "Vertex.h"

struct Triangle
{
public:
	Triangle() {}
	Triangle(Vertex vert1, Vertex vert2, Vertex vert3)
	{
		vt[0] = vert1;
		vt[1] = vert2;
		vt[2] = vert3;

		Vector2 sbbMin = Vector2(INFINITY, INFINITY);
		Vector2 sbbMax = Vector2(-INFINITY, -INFINITY);

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

		Min.X = RoundToInt(sbbMin.X);
		Min.Y = RoundToInt(sbbMin.Y);
		Max.X = RoundToInt(sbbMax.X);
		Max.Y = RoundToInt(sbbMax.Y);
	}

	Vertex vt[3];

	Vector2 u;
	Vector2 v;
	IntPoint Min;
	IntPoint Max;
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

};

