// Invert - returns the complement to 255
// Invert (C) 2003 Kurosu (kurosu@inforezo.org)

// Similar functionnalities (but not directly linked to it) can be found
// for instance in Invert from VirtualDub (www.virtualdub.org) by
// Avery Lee (phaeron@virtualdub.org)

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

#ifndef __INV__
#define __INV__

#include "avisynth.h"

class Invert : public GenericVideoFilter {
private:
	int			Y,U,V;
	int			offX, offY, w, h;
protected:
	void (*InvY) (unsigned char *srcp, const int src_pitch,
				const int row_size, const int height);
	void (*InvUV) (unsigned char *srcp, const int src_pitch,
				const int row_size, const int height);
public:
	Invert(PClip _child, int _X, int _Y, int _w, int _h,
			int y, int u, int v, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};
#endif