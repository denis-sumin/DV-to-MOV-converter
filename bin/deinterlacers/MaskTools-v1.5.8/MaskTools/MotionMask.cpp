// Motion Detection : create a mask from a basic motion detection
// Motion Detection (C) 2004 Manao (manao@melix.net)

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

#include "MotionMask.h"
#include "MD_Func.h"

MotionMask::MotionMask(PClip _child, unsigned int thY1, unsigned int thY2,
			           unsigned int thC1, unsigned int thC2, unsigned int thSD,
                       int _X, int _Y, int _w, int _h, int y, int u, int v, bool _mmx,
                       bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "MotionMask"),
Yth1(thY1), Yth2(thY2), Cth1(thC1), Cth2(thC2), SDth(thSD)
{
   CheckColorSpace(vi, env);

   Yth1 = Saturate(Yth1, 0, 255);
   Yth2 = Saturate(Yth2, Yth1, 255);
   Cth1 = Saturate(Cth1, 0, 255);
   Cth2 = Saturate(Cth2, Cth1, 255);

   MotionDetection = MD_C;

   SCDetection = CheckiSSEOptimizations(env, CHK_MOD_8, CHK_MOD_2) ? SD_ISSE : SD_C;
}

MotionMask::~MotionMask()
{
}

void MotionMask::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                          const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isSceneChange )
      memset(pDst, 0, nDstPitch * nHeight);
   else
      MotionDetection(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch,
                     nSrc2Pitch, nWidth, nHeight, Yth1, Yth2);
}

void MotionMask::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                          const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isSceneChange )
      memset(pDst, 0, nDstPitch * (nHeight / 2));
   else
      MotionDetection(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch,
                     nSrc2Pitch, nWidth / 2, nHeight / 2, Cth1, Cth2);
}

void MotionMask::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                          const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isSceneChange )
      memset(pDst, 0, nDstPitch * (nHeight / 2));
   else
      MotionDetection(pDst, pSrc1, pSrc2, nDstPitch, nSrc1Pitch,
                     nSrc2Pitch, nWidth / 2, nHeight / 2, Cth1, Cth2);
}

PVideoFrame __stdcall MotionMask::GetFrame(int n, IScriptEnvironment* env)
{
   PVideoFrame	dst = env->NewVideoFrame(vi);
   PVideoFrame	src = child->GetFrame(n, env);
   PVideoFrame	srcp = child->GetFrame(n ? n - 1 : 0, env);

   isSceneChange = SCDetection(src->GetReadPtr(PLANAR_Y), srcp->GetReadPtr(PLANAR_Y), 
      src->GetPitch(PLANAR_Y), nWidth, nHeight, SDth);
   ProcessFrame(dst, src, srcp, NULL, false, env);

   return dst;
}
