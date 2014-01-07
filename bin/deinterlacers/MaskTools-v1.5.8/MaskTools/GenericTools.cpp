/// MaskTools - various mask management tools for Avisynth
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
#include <memory.h>
#include "GenericTools.h"

bool Verify(int n)
{
	if (n>3 || n<-255) return false;
	else return true;
}

void MemZoneSet(unsigned char *ptr, unsigned char value, int width,
				int height, int offsetX, int offsetY, int pitch)
{
	ptr += offsetX + offsetY*pitch;
	for (int y=offsetY; y<height+offsetY; y++)
	{
		memset(ptr, value, width);
		ptr += pitch;
	}
}

unsigned char SaturateNRound(double x, double min, double max)
{
    if ( x < min ) return (unsigned char)min;
    else if ( x > max ) return (unsigned char)max;
    else return (unsigned char)(x + 0.5);
}
