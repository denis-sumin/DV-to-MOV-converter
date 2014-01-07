// EdgeMask - builds the edge map of a video plane based on selectable
// differential operator
// EdgeMask (C) 2003 Kurosu (kurosu@inforezo.org)

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


#include <string.h>
#include "EdgeMask.h"
#include "EM_func.h"

EdgeMask::EdgeMask(PClip _child, unsigned int thY1, unsigned int thY2,
				   unsigned int thC1, unsigned int thC2, const char *string, 
                   int _X, int _Y, int _w, int _h, int y, int u, int v,
                   bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "EdgeMask"), Yth1(thY1), Yth2(thY2), Cth1(thC1), Cth2(thC2)
{
   CheckColorSpace(vi, env);
	
	Yth1 = Saturate(Yth1, 0, 255);
   Yth2 = Saturate(Yth2, Yth1, 255);
   Cth1 = Saturate(Cth1, 0, 255);
   Cth2 = Saturate(Cth2, Cth1, 255);

	/* Check mode and if width is MOD8 */
	if (!lstrcmpi(string,"line"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Line_C : Line_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Line_C : Line_C;
	}
	else if (!lstrcmpi(string,"roberts"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Roberts_MMX : Roberts_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Roberts_MMX : Roberts_C;
	}
	else if (!lstrcmpi(string,"sobel"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Sobel_MMX : Sobel_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Sobel_MMX : Sobel_C;
	}
	else if (!lstrcmpi(string,"special"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Special_MMX : Special_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Special_MMX : Special_C;
	}
	else if (!lstrcmpi(string,"cartoon"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Cartoon_C : Cartoon_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Cartoon_C : Cartoon_C;
	}
	else if (!lstrcmpi(string,"laplace"))
	{
      BuildMaskY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? Laplace_C : Laplace_C;
      BuildMaskUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_1) ? Laplace_C : Laplace_C;
	}
   else env->ThrowError("EdgeMask: unvalid edge detector : 'type' must be either 'line', 'roberts', 'sobel', 'special', 'cartoon' or 'laplace'");
}

void EdgeMask::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BuildMaskY(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight, Yth1, Yth2);
}

void EdgeMask::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BuildMaskUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, Cth1, Cth2);
}

void EdgeMask::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                     const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   BuildMaskUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, Cth1, Cth2);
}


PVideoFrame __stdcall EdgeMask::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src = child->GetFrame(n, env);
	PVideoFrame	dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}
