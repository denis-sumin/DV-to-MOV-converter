// General Convolution : allow to make a convolution in YV12 colorspace
// General Convolution (C) 2004 Manao (manao@melix.net)

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

#ifndef __GC__
#define __GC__

#include "BaseFilter.h"

class YV12Convolution : public BaseFilter {
private:
	int				*horvecti, *vervecti;
	double			*horvectf, *vervectf;
	double			totalf;
	int				totali;
    bool            isFloat;

	int				horres, verres;

protected:
	void (*GC) (unsigned char *derp, const unsigned char *srcp, const int der_pitch,
				const int src_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const int horres, const int verres, const int total);
	void (*GCf) (unsigned char *derp, const unsigned char *srcp, const int der_pitch,
				const int src_pitch, const int row_size, 
				const int height, const double *horvect, const double *vervect,
				const int horres, const int verres, const double total);
	bool AnalyseString(const char *string, double **vectf, int **vect, int *halflength, IScriptEnvironment* env, bool *isFloat);
	void ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);
	void ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                  const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch);

public:
	YV12Convolution(PClip _child, const char * horizontal, const char *vertical,
					int ttl, bool automatic, bool saturate,int _X, int _Y, int _w, int _h,
                    int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env);
	~YV12Convolution();
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
	
};

#endif