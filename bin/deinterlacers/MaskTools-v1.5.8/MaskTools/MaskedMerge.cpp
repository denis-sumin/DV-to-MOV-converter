// MaskedMerge - Weighed merge of video planes depending on masks
// MaskedMerge (C) 2003 Kurosu (kurosu@inforezo.org)

// Similar functionnalities (but not directly linked to it) can be found
// in MergeLuma and MergeChroma from Avisynth (www.avisynth.org) by
// Sh0dan (Sh0dan@avisynth.org)

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

#include "MaskedMerge.h"
#include "MM_func.h"

MaskedMerge::MaskedMerge(PClip _child, PClip cl2, PClip cl3, int _X, int _Y, int _w, int _h,
						 int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) : 
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "MaskedMerge"), clip2(cl2), clip3(cl3)
{
   const VideoInfo& vi2 = clip2->GetVideoInfo();
	const VideoInfo& vi3 = clip3->GetVideoInfo();

   CheckFrameSize(vi, vi2, env);
   CheckFrameSize(vi, vi3, env);
   CheckColorSpace(vi, env);
   CheckColorSpace(vi2, env);
   CheckColorSpace(vi3, env);

   MMergeY = CheckMMXOptimizations(env, CHK_MOD_8, CHK_MOD_1) ? MMerge_MMX : MMerge_C;
   MMergeUV = CheckMMXOptimizations(env, CHK_MOD_16, CHK_MOD_2) ? MMerge_MMX : MMerge_C;
}

void MaskedMerge::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                           const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   MMergeY(pDst, pSrc1, pSrc2, pSrc3, nDstPitch, nSrc1Pitch, nSrc2Pitch, nSrc3Pitch, nWidth, nHeight);
}

void MaskedMerge::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                           const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   MMergeUV(pDst, pSrc1, pSrc2, pSrc3, nDstPitch, nSrc1Pitch, nSrc2Pitch, nSrc3Pitch, nWidth / 2, nHeight / 2);
}

void MaskedMerge::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                           const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   MMergeUV(pDst, pSrc1, pSrc2, pSrc3, nDstPitch, nSrc1Pitch, nSrc2Pitch, nSrc3Pitch, nWidth / 2, nHeight / 2);
}

PVideoFrame __stdcall MaskedMerge::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src1 = child->GetFrame(n, env);
	PVideoFrame	src2 = clip2->GetFrame(n, env);
	PVideoFrame	mask = clip3->GetFrame(n, env);
   PVideoFrame dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src1, src2, mask, true, env);

   if ( Y == 1 )
      env->BitBlt(dst->GetWritePtr(PLANAR_Y), dst->GetPitch(PLANAR_Y),
         src1->GetReadPtr(PLANAR_Y), src1->GetPitch(PLANAR_Y), nWidth, nHeight);
   if ( Y == 2 )
      env->BitBlt(dst->GetWritePtr(PLANAR_Y), dst->GetPitch(PLANAR_Y),
         src2->GetReadPtr(PLANAR_Y), src2->GetPitch(PLANAR_Y), nWidth, nHeight);

   if ( U == 1 )
      env->BitBlt(dst->GetWritePtr(PLANAR_U), dst->GetPitch(PLANAR_U),
         src1->GetReadPtr(PLANAR_U), src1->GetPitch(PLANAR_U), nWidth / 2, nHeight / 2);
   if ( U == 2 )
      env->BitBlt(dst->GetWritePtr(PLANAR_U), dst->GetPitch(PLANAR_U),
         src2->GetReadPtr(PLANAR_U), src2->GetPitch(PLANAR_U), nWidth / 2, nHeight / 2);

   if ( V == 1 )
      env->BitBlt(dst->GetWritePtr(PLANAR_V), dst->GetPitch(PLANAR_V),
         src1->GetReadPtr(PLANAR_V), src1->GetPitch(PLANAR_V), nWidth / 2, nHeight / 2);
   if ( V == 2 )
      env->BitBlt(dst->GetWritePtr(PLANAR_V), dst->GetPitch(PLANAR_V),
         src2->GetReadPtr(PLANAR_V), src2->GetPitch(PLANAR_V), nWidth / 2, nHeight / 2);

	return dst;
}