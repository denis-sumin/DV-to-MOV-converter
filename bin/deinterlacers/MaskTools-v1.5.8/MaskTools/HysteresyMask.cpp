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

#include "GenericTools.h"
#include "HysteresyMask.h"

HysteresyMask::HysteresyMask(PClip _child1, PClip _child2, int _X, int _Y, int _w, int _h, 
			      int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child1, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "HysteresyMask"), child2(_child2)
{
	const VideoInfo& vi2 = child2->GetVideoInfo();
   CheckColorSpace(vi, env);
   CheckColorSpace(vi2, env);
   CheckFrameSize(vi, vi2, env);

	isInitialized = false;
}
HysteresyMask::~HysteresyMask()
{
	if (isInitialized)
	{
		delete[] Xarray;
		delete[] Yarray;
		delete[] used;
	}
}

void HysteresyMask::ExpandMask(unsigned char *ResultMask, const unsigned char *RMask,
                               const unsigned char *EMask, const int row_size, const int height,
							   const int ResMPitch, const int RMPitch, const int EMPitch, int x, int y)
{
	Xarray[0] = x;
	Yarray[0] = y;
	used[x + y * row_size] = 255;
	ResultMask[x + y * ResMPitch] = 255;
	int taille = 1;
	while ( taille > 0 )
	{
		taille--;
		int Xcurr = Xarray[taille];
		int Ycurr = Yarray[taille];
		for ( int a = -1; a < 2; a++ )
		{
			for ( int b = -1; b < 2; b++ )
			{
				if (( a + Xcurr >= 0 ) && ( a + Xcurr < row_size ) && ( b + Ycurr >= 0 )
					&& ( b + Ycurr < height ) && (used[Xcurr + a + (Ycurr + b) * row_size] == 0) &&
					( EMask[Xcurr + a + (Ycurr + b) * EMPitch] != 0 ))
				{
					Xarray[taille] = a + Xcurr;
					Yarray[taille] = b + Ycurr;
					used[Xcurr + a + (Ycurr + b) * row_size] = 255;
					ResultMask[Xcurr + a + (Ycurr + b) * ResMPitch] = EMask[Xcurr + a + (Ycurr + b) * EMPitch];
					taille++;
				}
			}
		}
	}

}

void HysteresyMask::BuildMask(unsigned char *ResultMask, const unsigned char *RMask,
                              const unsigned char *EMask, const int row_size, const int height,
							  const int ResMPitch, const int RMPitch, const int EMPitch)
{
	memset(used, 0, row_size * height);
	memset(ResultMask, 0, ResMPitch * height);
	int pos = 0;
	for ( int j = 0; j < height; j++)
	{
		for ( int i = 0; i < row_size; i++ )
		{
			if (( used[i + j * row_size] == 0 ) && ( RMask[i + j * RMPitch] != 0 ) && ( EMask[i + j * EMPitch] != 0))
				ExpandMask(ResultMask, RMask, EMask, row_size, height, ResMPitch, RMPitch, EMPitch, i, j);
		}
	}
}

void HysteresyMask::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    BuildMask(pDst, pSrc1, pSrc2, nWidth, nHeight, nDstPitch, nSrc1Pitch, nSrc2Pitch);
}

void HysteresyMask::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    BuildMask(pDst, pSrc1, pSrc2, nWidth / 2, nHeight / 2, nDstPitch, nSrc1Pitch, nSrc2Pitch);
}

void HysteresyMask::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                             const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    BuildMask(pDst, pSrc1, pSrc2, nWidth / 2, nHeight / 2, nDstPitch, nSrc1Pitch, nSrc2Pitch);
}

PVideoFrame __stdcall HysteresyMask::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	src1 = child->GetFrame(n, env);
   PVideoFrame	src2 = child2->GetFrame(n, env);
	PVideoFrame	dst = env->NewVideoFrame(vi);
	if ( !isInitialized )
	{
		used = new unsigned char[nWidth*nHeight];
		Xarray = new int[nWidth*nHeight];
		Yarray = new int[nWidth*nHeight];
		isInitialized = true;
	}
   ProcessFrame(dst, src1, src2, NULL, false, env);

   return dst;
}
