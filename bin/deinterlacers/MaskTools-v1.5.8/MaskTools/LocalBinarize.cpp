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

#include "LocalBinarize.h"
//#include "Bin_func.h"

LocalBinarize::LocalBinarize(PClip _child, int _th1, int _th2, int rad, int _th3, int _bth1, int _bth2, float weight, bool up, int _X, int _Y, int _w, int _h, 
			                    int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "Binarize")
{
   nRadius = Saturate(rad, 1, 10);
   nThreshold1 = Saturate(_th1, 0, 256);
   nThreshold2 = Saturate(_th2, 0, 256);
   nDiscardThreshold = Saturate(_th3, 0, 256);
   nWeight = Saturate((int)(weight * 256.0 + 0.5), 0, 256);
   nBaseThreshold1 = Saturate(_bth1, 0, 256);
   nBaseThreshold2 = Saturate(_bth2, 0, 256);
   upper = up;

   CheckColorSpace(vi, env);
}

void LocalBinarize::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LocBinarize(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight, nThreshold1, nThreshold2, nRadius,
      nDiscardThreshold, nBaseThreshold1, nBaseThreshold2, nWeight, upper);
}

void LocalBinarize::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LocBinarize(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, nThreshold1, nThreshold2, nRadius,
      nDiscardThreshold, nBaseThreshold1, nBaseThreshold2, nWeight, upper);
}

void LocalBinarize::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   LocBinarize(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, nThreshold1, nThreshold2, nRadius,
      nDiscardThreshold, nBaseThreshold1, nBaseThreshold2, nWeight, upper);
}

PVideoFrame __stdcall LocalBinarize::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src = child->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}