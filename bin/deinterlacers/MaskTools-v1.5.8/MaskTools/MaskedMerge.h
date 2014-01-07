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

#ifndef __MM__
#define __MM__

#include "BaseFilter.h"

class MaskedMerge : public BaseFilter {
private:
	PClip		clip2,clip3;
protected:
	//Destination is used as mask
	void (*MMergeY) (unsigned char *pDst, const unsigned char *pSrc1,
                     const unsigned char *pSrc2, const unsigned char *pMask,
                     int dstPitch, int src1Pitch, int src2Pitch, int maskPitch,
                     int width, int height);
	void (*MMergeUV) (unsigned char *pDst, const unsigned char *pSrc1,
                      const unsigned char *pSrc2, const unsigned char *pMask,
                      int dstPitch, int src1Pitch, int src2Pitch, int maskPitch,
                      int width, int height);
	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	MaskedMerge(PClip _child, PClip cl2, PClip cl3, int _X, int _Y, int _w, int _h,
				int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};
#endif