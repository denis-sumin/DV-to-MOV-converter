// MaskTools - various mask management tools for Avisynth
// MaskTools (C) 2003 Kurosu (kurosu@inforezo.org)

// MaskedMerge, Invert, EdgeMask, Binarize, Expand, Logic, YV12Layer,
// YV12Subtract, MaskOverlay, FitPlane and FastFitPlane are 
// (C) 2003 Kurosu (kurosu@inforezo.org)

// Masktools: http://kurosu.inforezo.org/avs/MaskTools/index.html
// Avisynth: www.avisynth.org

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

#ifndef			__GENERIC_TOOLS__
#define			__GENERIC_TOOLS__

#define			NO_OPERATION	1
#define			COPY_SOURCE		2
#define			OPERATION		3

#define         CHK_MOD_16      0x0000000F
#define         CHK_MOD_8       0x00000007
#define         CHK_MOD_4       0x00000003
#define         CHK_MOD_2       0x00000001
#define         CHK_MOD_1       0x00000000



bool Verify(int n);
void MemZoneSet(unsigned char *ptr, unsigned char value, int width,
				int height, int offsetX, int offsetY, int pitch);

unsigned char SaturateNRound(double x, double min, double max);

__forceinline int MyAbs(int x)
{
	return (x < 0) ? -x : x;
};

#endif