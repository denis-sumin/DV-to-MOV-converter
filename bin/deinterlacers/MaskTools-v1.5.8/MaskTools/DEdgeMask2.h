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

#ifndef __DEM2__
#define __DEM2__

#include "BaseFilter.h"

class DEdgeMask2 : public BaseFilter {
private:
   PClip    child2, child3;
	int		array[9];
	int		divisor;

	void (DEdgeMask2::*BM_Fun)(unsigned char *dstp, const unsigned char *srcp,
                              const int dst_pitch, const int src_pitch,
                              const unsigned char *th_low, int low_pitch,
                              const unsigned char *th_high, int high_pitch,
                              int row_size, const int height);

	void BuildMask(unsigned char *dstp, const unsigned char *srcp,
                  const int dst_pitch, const int src_pitch,
                  const unsigned char *th_low, int low_pitch,
                  const unsigned char *th_high, int high_pitch,
                  int row_size, const int height);

	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

	int ConvertCharToInt(const char c);
	int ConvertStringToInt(const char *string, int end);
	bool AnalyseString(const char *string, int *vect, IScriptEnvironment* env);

public:
	DEdgeMask2(PClip _child, PClip clip2,  PClip clip3, const char * s, 
			     bool setDivisor, int div, int _X, int _Y, int _w, int _h, int y, int u, int v,
              bool _mmx, bool _isse, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};
#endif