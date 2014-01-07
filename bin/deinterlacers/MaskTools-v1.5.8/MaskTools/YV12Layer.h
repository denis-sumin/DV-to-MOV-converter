// YV12Layer - port of AviSynth's Layer functions to YV12
// YV12Layer (C) 2003 Kurosu (kurosu@inforezo.org)
// Layer (C) Poptones

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

#ifndef __LAYER__
#define __LAYER__

#include "avisynth.h"

class YV12Layer : public GenericVideoFilter
{
public:
	YV12Layer(PClip _child1, PClip _child2, const char *mode,
			int _level, bool Chroma, int _x, int _y,
			int _w, int _h, int _Y, int _U,	int _V, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
protected:
void (* Process_Y)(unsigned char *src1p, const unsigned char *src2p,
				const int src1_pitch, const int src2_pitch,
				const int row_size, const int height,
				unsigned char level);
void (* Process_C)(unsigned char *src1p, const unsigned char *src2p,
				const int src1_pitch, const int src2_pitch,
				const int row_size, const int height,
				unsigned char level);
	
private:
	PClip			child2;
	unsigned char	level;
	int				Y, U, V;
	int				xdest, ydest, xsrc, ysrc, xcount, ycount;
};

#endif