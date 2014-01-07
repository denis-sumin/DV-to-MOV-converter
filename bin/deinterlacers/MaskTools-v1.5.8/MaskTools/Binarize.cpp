// Binarize - Binarize (outputs either 0 or 255) an video plane
// Binarize (C) 2003 Kurosu (kurosu@inforezo.org)

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

#include "binarize.h"
#include "Bin_func.h"

Binarize::Binarize(PClip _child, int _th, bool up, int _X, int _Y, int _w, int _h, 
			       int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "Binarize"), threshold(_th)
{
   CheckColorSpace(vi, env);

   threshold = Saturate(threshold, 0, 255);

   if (up)
   {
      BinY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2) ? BinUpper_MMX : BinUpper_C;
      BinUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_4) ? BinUpper_MMX : BinUpper_C;
   }
   else {
      BinY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2) ? BinLower_MMX : BinLower_C;
      BinUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_4) ? BinLower_MMX : BinLower_C;
   }
}

void Binarize::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                        const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BinY(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight, threshold);
}

void Binarize::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                        const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BinUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, threshold);
}

void Binarize::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                        const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BinUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, threshold);
}

PVideoFrame __stdcall Binarize::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src = child->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}