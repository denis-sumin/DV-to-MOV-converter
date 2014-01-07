// Logic - very basic mask logic manipulations
// Logic (C) 2003 Kurosu (kurosu@inforezo.org)

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
#include "Logic.h"
#include "LGC_func.h"

Logic::Logic(PClip _cl1, PClip cl2, const char *mode, int _X, int _Y, int _w, int _h, 
          int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) : 
BaseFilter(_cl1, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "Logic"), clip2(cl2)
{
	const VideoInfo& vi2 = cl2->GetVideoInfo();
   CheckFrameSize(vi, vi2, env);
   CheckColorSpace(vi, env);
   CheckColorSpace(vi2, env);
	
	if (!lstrcmpi(mode, "And"))
	{
      if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = And_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_1))
         LogicProcessY = And_C4;
      else LogicProcessY = And_C1;

      if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = And_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessUV = And_C4;
      else LogicProcessUV = And_C1;
	}
	else if (!lstrcmpi(mode, "Or"))
	{
      if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = Or_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_1))
         LogicProcessY = Or_C4;
      else LogicProcessY = Or_C1;

      if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Or_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessUV = Or_C4;
      else LogicProcessUV = Or_C1;
	}
	else if (!lstrcmpi(mode, "Xor"))
	{
      if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = Xor_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_1))
         LogicProcessY = Xor_C4;
      else LogicProcessY = Xor_C1;

      if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Xor_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessUV = Xor_C4;
      else LogicProcessUV = Xor_C1;
	}
	else if (!lstrcmpi(mode, "Andn"))
	{
      if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessY = Andn_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_4, CHK_MOD_1))
         LogicProcessY = Andn_C4;
      else LogicProcessY = Andn_C1;

      if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Andn_MMX;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessUV = Andn_C4;
      else LogicProcessUV = Andn_C1;
	}
	else if (!lstrcmpi(mode, "Min"))
	{
      if (CheckiSSEOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = Min_iSSE;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_2))
         LogicProcessY = Min_MMX;
      else LogicProcessY = Min_C;

      if (CheckiSSEOptimizations(env, CHK_MOD_16, CHK_MOD_1))
         LogicProcessUV = Min_iSSE;
      else if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Min_MMX;
      else LogicProcessUV = Min_C;
	}
	else if (!lstrcmpi(mode, "Max"))
	{
      if (CheckiSSEOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = Max_iSSE;
      else if (CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1))
         LogicProcessY = Max_MMX;
      else LogicProcessY = Max_C;

      if (CheckiSSEOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Max_iSSE;
      else if (CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2))
         LogicProcessUV = Max_MMX;
      else LogicProcessUV = Max_C;
	}
	else env->ThrowError("Logic : unvalid operator, only 'And', 'Or', 'Xor', 'Andn', 'Min' and 'Max' are allowed");
}

void Logic::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LogicProcessY(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth, nHeight);
}

void Logic::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LogicProcessUV(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth / 2, nHeight / 2);
}

void Logic::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LogicProcessUV(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch, nSrc2Pitch, nWidth / 2, nHeight / 2);
}

PVideoFrame __stdcall Logic::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src1 = child->GetFrame(n, env);
	PVideoFrame	src2 = clip2->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src1, src2, NULL, false, env);
	return dst;
}
