// YV12LUT - LookUp Table transform for YV12
// YV12LUT (C) 2003 Kurosu (kurosu@inforezo.org)

// Based on "Levels" function from AviSynth software

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

#ifndef __YUY2LUT__
#define __YUY2LUT__

#include "avisynth.h"

class YUY2LUT : public GenericVideoFilter {
private:
	int 			Y, U, V;
	unsigned char	*YLUT, *ULUT, *VLUT;
	int				offX, offY, w, h;
	double clip(double x);
public:
	YUY2LUT(PClip _child, const char *yExpr, const char *uExpr, const char *vExpr, int _X, int _Y,
		int _w, int _h, int y, int u, int v, IScriptEnvironment* env);
	~YUY2LUT();
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif