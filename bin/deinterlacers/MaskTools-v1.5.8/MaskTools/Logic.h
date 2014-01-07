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

#ifndef	_LOGIC_
#define	_LOGIC_

#include "BaseFilter.h"

class Logic : public BaseFilter {
private:
	const PClip	clip2;
protected:
	void (*LogicProcessY)(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
                          int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
	void (*LogicProcessUV)(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
                           int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	Logic(PClip _cl1, PClip cl2, const char *mode, int _X, int _Y, int _w, int _h, 
          int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif