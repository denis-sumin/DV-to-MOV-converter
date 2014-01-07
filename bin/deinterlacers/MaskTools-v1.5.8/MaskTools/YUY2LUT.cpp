// YV12LUT - LookUp Table transform for YV12
// YV12LUT (C) 2003 Kurosu (kurosu@inforezo.org)

// Based on "Levels" function from AviSynth software

// See legal notice in Interface.cpp and Readme.txt for more information

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .

#include <stdio.h>
#include <string.h>
#include "Expressions.h"
#include "GenericTools.h"
#include "YUY2LUT.h"

double YUY2LUT::clip(double x)
{
	if (x < 0.5) return 0;
	if (x > 254.5) return 255;
	return x+0.5;
}

YUY2LUT::YUY2LUT(PClip _child, const char *yExpr, const char *uExpr, const char *vExpr,
			 int _X, int _Y, int _w, int _h, int y, int u, int v,  
			IScriptEnvironment* env) : GenericVideoFilter(_child), w(_w), h(_h),
			offX(_X), offY(_Y), Y(y), U(u), V(v)
{
   if (!vi.IsYUY2())
		env->ThrowError("YUY2LUT: Needs YUY2 data");
	
	if (w<0) w = vi.width;
	if (w>vi.width-offX) w = vi.width-offX;
	if (h<0) h = vi.height;
	if (h>vi.height-offY) h = vi.height-offY;

	char separator = ' ';

	__asm { emms }

	if (!Verify(Y)) env->ThrowError("YUY2LUT: Unvalid operation mode on Y");
	if (!Verify(U)) env->ThrowError("YUY2LUT: Unvalid operation mode on U");
	if (!Verify(V)) env->ThrowError("YUY2LUT: Unvalid operation mode on V");

	Expression *Yexpr = Expression::BuildExpression(yExpr,separator);
	Expression *Uexpr = Expression::BuildExpression(uExpr,separator);
	Expression *Vexpr = Expression::BuildExpression(vExpr,separator);

	YLUT = new unsigned char[256];
	ULUT = new unsigned char[256];
	VLUT = new unsigned char[256];

	for ( int x = 0; x < 256; x++ )
	{
		switch (Y)
		{
		case 3 : YLUT[x] = (unsigned char)(clip(Yexpr->process((double)x,0))); break;
		case 2 :
		case 1 : YLUT[x] = x; break;
		default : YLUT[x] = -Y;
		}
		switch (U)
		{
		case 3 : ULUT[x] = (unsigned char)(clip(Uexpr->process((double)x,0))); break;
		case 2 :
		case 1 : ULUT[x] = x; break;
		default : ULUT[x] = -U;
		}
		switch (V)
		{
		case 3 : VLUT[x] = (unsigned char)(clip(Vexpr->process((double)x,0))); break;
		case 2 :
		case 1 : VLUT[x] = x; break;
		default : VLUT[x] = -V;
		}
	}

	delete Yexpr;
	delete Uexpr;
	delete Vexpr;

}

YUY2LUT::~YUY2LUT()
{
	delete YLUT;
	delete ULUT;
	delete VLUT;
}

PVideoFrame __stdcall YUY2LUT::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame frame = child->GetFrame(n, env);
	env->MakeWritable(&frame);
	BYTE* p = frame->GetWritePtr();
	const int pitch = frame->GetPitch();

	p += offY * pitch;
	for (int y=offY; y < h; ++y) 
	{
		for (int x=offX; x < w; x+=2) 
		{
		      p[x*2] = YLUT[p[x*2]];
			p[x*2+1] = ULUT[p[x*2+1]];
			p[x*2+2] = YLUT[p[x*2+2]];
         p[x*2+3] = VLUT[p[x*2+3]];
		}
		p += pitch;
	}

	return frame;
}