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

#include "Expressions.h"
#include "GenericTools.h"
#include "LUT.h"

LUT::LUT(PClip _child, const char *yExpr, const char *uExpr, const char *vExpr,
		 int _X, int _Y, int _w, int _h, int y, int u, int v, bool _mmx, bool _isse, 
		 IScriptEnvironment* env) : BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "YV12LUT")
{
   CheckColorSpace(vi, env);

	char separator = ' ';

	__asm { emms }

	Expression *Yexpr = Expression::BuildExpression(yExpr,separator);
	Expression *Uexpr = Expression::BuildExpression(uExpr,separator);
	Expression *Vexpr = Expression::BuildExpression(vExpr,separator);

	YLUT = new unsigned char[256];
	ULUT = new unsigned char[256];
	VLUT = new unsigned char[256];
    	
	for ( int x = 0; x < 256; x++ )
	{
      YLUT[x] = SaturateNRound(Yexpr->process((double)x,0), 0.0, 255.0);
		ULUT[x] = SaturateNRound(Uexpr->process((double)x,0), 0.0, 255.0);
		VLUT[x] = SaturateNRound(Vexpr->process((double)x,0), 0.0, 255.0);
	}
	
	delete Yexpr;
	delete Uexpr;
	delete Vexpr;

}

LUT::~LUT()
{
	delete YLUT;
	delete ULUT;
	delete VLUT;
}

void LUT::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                   const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   for ( int i = 0; i < nHeight; i++ )
	{
		for ( int j = 0; j < nWidth; j++ )
			pDst[j] = YLUT[pSrc1[j]];
		pDst += nDstPitch;
      pSrc1 += nSrc1Pitch;
	}
}

void LUT::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                   const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   for ( int i = 0; i < nHeight / 2; i++ )
	{
		for ( int j = 0; j < nWidth / 2; j++ )
			pDst[j] = ULUT[pSrc1[j]];
		pDst += nDstPitch;
      pSrc1 += nSrc1Pitch;
	}
}

void LUT::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                   const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   for ( int i = 0; i < nHeight / 2; i++ )
	{
		for ( int j = 0; j < nWidth / 2; j++ )
			pDst[j] = VLUT[pSrc1[j]];
		pDst += nDstPitch;
      pSrc1 += nSrc1Pitch;
	}
}

PVideoFrame __stdcall LUT::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame src = child->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}