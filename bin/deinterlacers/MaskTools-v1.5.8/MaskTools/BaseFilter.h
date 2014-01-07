// Base class for all the filters of that set

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

#ifndef __BASE_FILTER__
#define __BASE_FILTER__

#pragma warning(disable:4103) // disable pack to change alignment warning ( stlport related )

#include "avisynth.h"

#define			NO_OPERATION	1
#define			COPY_SOURCE		2
#define			OPERATION		3

#define         CHK_MOD_16      0x0000000F
#define         CHK_MOD_8       0x00000007
#define         CHK_MOD_4       0x00000003
#define         CHK_MOD_2       0x00000001
#define         CHK_MOD_1       0x00000000

class BaseFilter : public GenericVideoFilter {
protected:
	int 			Y, U, V;
	int				nOffX, nOffY, nWidth, nHeight;
	char			filterName[32];
    bool            mmx, isse;
    
	virtual void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                          const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch) = 0;

	virtual void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                         const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch) = 0;

	virtual void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                         const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch) = 0;

    void ProcessFrame(PVideoFrame &dst, PVideoFrame src1, PVideoFrame src2, PVideoFrame src3,
                      bool inPlace, IScriptEnvironment *env);

    void CheckFrameSize(const VideoInfo &vi1, const VideoInfo &vi2, IScriptEnvironment *env);

    void CheckColorSpace(const VideoInfo &vip, IScriptEnvironment *env);

    bool CheckMMXOptimizations(IScriptEnvironment *env, int nModWith, int nModHeight);

    bool CheckiSSEOptimizations(IScriptEnvironment *env, int nModWith, int nModHeight);

    int Saturate(int a, int b, int c);

public:
	BaseFilter(PClip _child, int _X, int _Y, int _w, int _h, int y, int u, int v,
               bool _mmx, bool _isse, IScriptEnvironment* env, const char *name);
	~BaseFilter();
};

#endif