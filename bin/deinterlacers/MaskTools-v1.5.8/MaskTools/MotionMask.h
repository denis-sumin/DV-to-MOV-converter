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

#ifndef __MOTION_DETECTION__
#define __MOTION_DETECTION__

#include "BaseFilter.h"

class MotionMask : public BaseFilter {
private:
	int Yth1, Yth2, Cth1, Cth2, SDth;
   bool isSceneChange;

protected:
	void (*MotionDetection) (unsigned char *derp, const unsigned char *srcp,
                            const unsigned char *srcp_prev, const int der_pitch,
                            const int src_pitch, const int src_prev_pitch, 
                            const int row_size, const int height,
                            const unsigned char thresinf, const unsigned char thressup);
	bool (*SCDetection) (const unsigned char *srcp, const unsigned char *srcp_prev,
				            const int src_pitch, const int row_size, const int height, 
				            const unsigned char thressd);
   void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                 const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                 const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                 const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	MotionMask(PClip _child, unsigned int thY1, unsigned int thY2,
	           unsigned int thC1, unsigned int thC2, unsigned int thSD,
              int _X, int _Y, int _w, int _h, int y, int u, int v, bool _mmx,
              bool _isse, IScriptEnvironment* env);
	~MotionMask();
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif