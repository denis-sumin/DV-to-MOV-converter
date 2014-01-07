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

#ifndef __DEM__
#define __DEM__

#include "BaseFilter.h"

class DEdgeMask : public BaseFilter {
private:
	int 		Yth1, Yth2, Cth1, Cth2;
	int			array[9];
	int			divisor;
	double		fdivisor;
	bool		isFloat;
    bool        vMode;

	void (DEdgeMask::*BM_Fun)(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				              const int src_pitch, int row_size, const int height,
				              const unsigned char th1, const unsigned char th2);
	void BuildMask(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				   const int src_pitch, int row_size, const int height,
				   const unsigned char th1, const unsigned char th2);
	void FBuildMask(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				    const int src_pitch, int row_size, const int height,
				    const unsigned char th1, const unsigned char th2);
	void VBuildMask(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				    const int src_pitch, int row_size, const int height,
				    const unsigned char th1, const unsigned char th2);
	void VFBuildMask(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				     const int src_pitch, int row_size, const int height,
				     const unsigned char th1, const unsigned char th2);
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
	DEdgeMask(PClip _child, unsigned int thY1, unsigned int thY2,
			  unsigned int thC1, unsigned int thC2, const char * s, 
			  bool setDivisor, int div, double fdiv, bool isf, bool vm,
              int _X, int _Y, int _w, int _h, int y, int u, int v,
              bool _mmx, bool _isse, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};
#endif