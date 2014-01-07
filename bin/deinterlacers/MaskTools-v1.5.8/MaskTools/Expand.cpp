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

#include "Expand.h"
#include "EXP_func.h"

Expand::Expand(PClip _child, int mode, int _X, int _Y, int _w, int _h,
			   int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) : 
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "Morphologic Operator")
{
    CheckColorSpace(vi, env);
	
	switch(mode)
	{
		case MODE_EXPAND:
         ExpandFrameY = Expand_C;
         ExpandFrameUV = Expand_C;
			break;
		case MODE_INPAND:
			ExpandFrameY = Inpand_C;
         ExpandFrameUV = Inpand_C;
			break;
		case MODE_INFLATE:
			ExpandFrameY = Inflate_C;
         ExpandFrameUV = Inflate_C;
			break;
		case MODE_DEFLATE:
			ExpandFrameY = Deflate_C;
         ExpandFrameUV = Deflate_C;
			break;
		default:
			env->ThrowError("Morphologic Operator : Invalid operation mode");
			break;
	}
}

void Expand::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                      const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    ExpandFrameY(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight);
}

void Expand::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                      const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    ExpandFrameUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2);
}

void Expand::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                      const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    ExpandFrameUV(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2);
}

PVideoFrame __stdcall Expand::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src = child->GetFrame(n, env);
   PVideoFrame	dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}