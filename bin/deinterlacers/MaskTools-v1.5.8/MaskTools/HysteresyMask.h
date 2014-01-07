// YV12Subtract - expansion of AviSynth's Subtract
// YV12Subtract (C) 2003 Kurosu (kurosu@inforezo.org)
// Subtract (C) AviSynth's team

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

#ifndef __HYSTERY__
#define __HYSTERY__

#include "BaseFilter.h"

/*
 * Class for subtracting one clip from another
 */
class HysteresyMask : public BaseFilter
{
private:
	PClip			child2;
	bool			isInitialized;
	unsigned char	*used;
	int				*Xarray;
	int				*Yarray;

protected:
	void BuildMask(unsigned char *ResultMask, const unsigned char *RMask,
                   const unsigned char *EMask, const int row_size, const int height,
				   const int ResMPitch, const int RMPitch, const int EMPitch);
	void ExpandMask(unsigned char *ResultMask, const unsigned char *RMask,
                    const unsigned char *EMask, const int row_size, const int height,
					const int ResMPitch, const int RMPitch, const int EMPitch, int x, int y);
	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	HysteresyMask(PClip _child1, PClip _child2, int _X, int _Y, int _w, int _h, 
			      int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env);
	~HysteresyMask();
    PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif