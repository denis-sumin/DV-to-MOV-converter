// Comb Mask : create a mask from a combing detection
// Comb Mask (C) 2004 Manao (manao@melix.net)

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

#ifndef __CM_FUNC__
#define __CM_FUNC__

void CM_C(const unsigned char *srcp, unsigned char *derp,
		  const int src_pitch, const int der_pitch, const int row_size,
		  const int height, const unsigned char thresinf, const unsigned char thressup);

void CM_MMX(const unsigned char *srcp, unsigned char *derp,
		  const int src_pitch, const int der_pitch, const int row_size,
		  const int height, const unsigned char thresinf, const unsigned char thressup);

#endif