// Expand - set of tools for region growth of at best one pixel
// Expand (C) 2003 Kurosu (kurosu@inforezo.org)

// This work can also be referred to by the word AreaModifier

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

#ifndef	_EXPAND_
#define	_EXPAND_

#define	MODE_EXPAND		0
#define	MODE_INPAND		1
#define	MODE_INFLATE	2
#define	MODE_DEFLATE	3

#include "BaseFilter.h"

class Expand : public BaseFilter {
protected :
	void (*ExpandFrameY)(unsigned char *dstp, const unsigned char *srcp,
					     const int dst_pitch, const int src_pitch,
					     const int row_size, const int height);
    void (*ExpandFrameUV)(unsigned char *dstp, const unsigned char *srcp,
					      const int dst_pitch, const int src_pitch,
					      const int row_size, const int height);
	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	Expand(PClip _child, int mode, int _X, int _Y, int _w, int _h,
			int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif
