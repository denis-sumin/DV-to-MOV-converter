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
#include "RGBLUT.h"

double RGBLUT::clip(double x)
{
	if (x < 0.5) return 0;
	if (x > 254.5) return 255;
	return x+0.5;
}

RGBLUT::RGBLUT(PClip _child, const char *rExpr, const char *gExpr, const char *bExpr,
			 int _X, int _Y, int _w, int _h, int r, int g, int b, const char *ampfile, 
			IScriptEnvironment* env) : GenericVideoFilter(_child), w(_w), h(_h),
			offX(_X), offY(_Y), R(r), G(g), B(b)
{
	if (!vi.IsRGB())
		env->ThrowError("RGBLUT: Needs RGB data");
	
	if (w<0) w = vi.width;
	if (w>vi.width-offX) w = vi.width-offX;
	if (h<0) h = vi.height;
	if (h>vi.height-offY) h = vi.height-offY;

	char separator = ' ';

	__asm { emms }

	if (!Verify(R)) env->ThrowError("RGBLUT: Unvalid operation mode on R");
	if (!Verify(G)) env->ThrowError("RGBLUT: Unvalid operation mode on G");
	if (!Verify(B)) env->ThrowError("RGBLUT: Unvalid operation mode on B");

	Expression *Rexpr = Expression::BuildExpression(rExpr,separator);
	Expression *Gexpr = Expression::BuildExpression(gExpr,separator);
	Expression *Bexpr = Expression::BuildExpression(bExpr,separator);

	RLUT = new unsigned char[256];
	GLUT = new unsigned char[256];
	BLUT = new unsigned char[256];

	FILE *file = fopen(ampfile,"rb");
	if ( file == NULL )
	{
		for ( int x = 0; x < 256; x++ )
		{
			switch (R)
			{
			case 3 : RLUT[x] = (unsigned char)(clip(Rexpr->process((double)x,0))); break;
			case 2 :
			case 1 : RLUT[x] = x; break;
			default : RLUT[x] = -R;
			}
			switch (G)
			{
			case 3 : GLUT[x] = (unsigned char)(clip(Gexpr->process((double)x,0))); break;
			case 2 :
			case 1 : GLUT[x] = x; break;
			default : GLUT[x] = -G;
			}
			switch (B)
			{
			case 3 : BLUT[x] = (unsigned char)(clip(Bexpr->process((double)x,0))); break;
			case 2 :
			case 1 : BLUT[x] = x; break;
			default : BLUT[x] = -B;
			}
		}
	}
	else {
		fread(RLUT,sizeof(unsigned char),256,file);
		if ( feof(file) )
		{
			for ( int i = 0; i < 256; i++ )
			{
				GLUT[i] = RLUT[i];
				BLUT[i] = RLUT[i];
			}
		}
		else {
			fread(GLUT,sizeof(unsigned char),256,file);
			if ( feof(file) )
			{
				for ( int i = 0; i < 256; i++ )
					BLUT[i] = GLUT[i];
			}
			else {
				fread(BLUT,sizeof(unsigned char),256,file);
			}
		}
		fclose(file);
	}

	for ( int x = 0; x < 256; x++ )
	{
		switch (R)
		{
		case 3 : break;
		case 2 :
		case 1 : RLUT[x] = x; break;
		default : RLUT[x] = -R;
		}
		switch (G)
		{
		case 3 : break;
		case 2 :
		case 1 : GLUT[x] = x; break;
		default : GLUT[x] = -G;
		}
		switch (B)
		{
		case 3 : break;
		case 2 :
		case 1 : BLUT[x] = x; break;
		default : BLUT[x] = -B;
		}
	}
	
	delete Rexpr;
	delete Gexpr;
	delete Bexpr;

}

RGBLUT::~RGBLUT()
{
	delete RLUT;
	delete GLUT;
	delete BLUT;
}

PVideoFrame __stdcall RGBLUT::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame frame = child->GetFrame(n, env);
	env->MakeWritable(&frame);
	BYTE* p = frame->GetWritePtr();
	const int pitch = frame->GetPitch();

	if (vi.IsRGB32())
	{
		p += offY * pitch;
		for (int y=offY; y < h; ++y) 
		{
			for (int x=offX; x < w; ++x) 
			{
		        p[x*4] = BLUT[p[x*4]];
				p[x*4+1] = GLUT[p[x*4+1]];
				p[x*4+2] = RLUT[p[x*4+2]];
			}
			p += pitch;
		}
	}
    else if (vi.IsRGB24()) {
		p += offY * pitch;
		for (int y=offY; y < h; ++y) 
		{
			for (int x=offX; x < w; ++x) 
			{
				p[x*3] = BLUT[p[x*3]];
				p[x*3+1] = GLUT[p[x*3+1]];
				p[x*3+2] = RLUT[p[x*3+2]];
			}
			p += pitch;
		}
	}

	return frame;
}