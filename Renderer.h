#pragma once
#include "Vector.h"
#include "stdafx.h"

void UpdateFrame(void);
void PutPixel(IntPoint pt);
ULONG Interpolation(ULONG current, ULONG Target, float alpha);