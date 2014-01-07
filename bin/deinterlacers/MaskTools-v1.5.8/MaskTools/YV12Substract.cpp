// YV12Subtract - expansion of AviSynth's Subtract
// YV12Subtract (C) 2003 Kurosu (kurosu@inforezo.org)
// Subtract (C) AviSynth's team

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

#include "GenericTools.h"
#include "YV12Substract.h"
#include "YV12S_Func.h"
#include <math.h>

YV12Substract::YV12Substract(PClip _child1, PClip _child2, double _tol, bool wideRange, int _X, int _Y,
				  int _w, int _h, int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child1, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "YV12Subtract"), child2(_child2), tolerance(_tol)
{
	const VideoInfo& vi2 = child2->GetVideoInfo();
   CheckFrameSize(vi, vi2, env);
   CheckColorSpace(vi, env);
   CheckColorSpace(vi2, env);

	if ( wideRange )
	{
		double alpha;
		if ( _tol <= 0 ) alpha = 0.5;
		else alpha = 1.0 / _tol;
		LUT = new unsigned char[511];
		for ( int i = 0; i < 511; i++ )
		{
			double x = i - 255;
			if ( i < 255 ) LUT[i] = 128.0 - 128.0 * pow(-x / 255.0, alpha);
			else LUT[i] = 128.0 + 127.0 * pow(x / 255.0, alpha);
		}
        if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
            SubtractY = SubstractW_MMX;
        else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_2))
            SubtractY = SubstractW_C2;
        else SubtractY = SubstractW_C1;
        if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_4))
            SubtractUV = SubstractW_MMX;
        else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_4))
            SubtractUV = SubstractW_C2;
        else SubtractUV = SubstractW_C1;
	}
	else if (_tol<0)
	{
        if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
            SubtractY = Substract_MMX;
        else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_2))
            SubtractY = Substract_C2;
        else SubtractY = Substract_C1;
        if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_4))
            SubtractUV = Substract_MMX;
        else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_4))
            SubtractUV = Substract_C2;
        else SubtractUV = Substract_C1;
	}
	else
	{
		if (tolerance > 255)
			env->ThrowError("YV12Substract: tolerance %i isn't in [0..255]",_tol);

        if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
            SubtractY = Diff_MMX;
        else SubtractY = Diff_C;
        if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_4))
            SubtractUV = Diff_MMX;
        else SubtractUV = Diff_C;
	}
}

void YV12Substract::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   SubtractY(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth, nHeight, tolerance, LUT);
}

void YV12Substract::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   SubtractUV(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth / 2, nHeight / 2, tolerance, LUT);
}

void YV12Substract::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   SubtractUV(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth / 2, nHeight / 2, tolerance, LUT);
}


PVideoFrame __stdcall YV12Substract::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src1 = child->GetFrame(n, env);
   PVideoFrame src2 = child2->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src1, src2, NULL, false, env);
	return dst;
}
