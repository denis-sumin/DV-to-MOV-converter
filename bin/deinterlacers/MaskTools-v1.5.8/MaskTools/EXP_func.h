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

#ifndef __Expand_F__
#define __Expand_F__
void Expand_C(unsigned char *dstp, const unsigned char *srcp,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height);
//void Expand_MMX(unsigned char *dstp, const unsigned char *srcp,
//			const int dst_pitch, const int src_pitch,
//			const int row_size, const int height);
void Inpand_C(unsigned char *dstp, const unsigned char *srcp,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height);
//void Inpand_MMX(unsigned char *dstp, const unsigned char *srcp,
//			const int dst_pitch, const int src_pitch,
//			const int row_size, const int height);
void Inflate_C(unsigned char *dstp, const unsigned char *srcp,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height);
//void Inflate_MMX(unsigned char *dstp, const unsigned char *srcp,
//			const int dst_pitch, const int src_pitch,
//			const int row_size, const int height);
void Deflate_C(unsigned char *dstp, const unsigned char *srcp,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height);
//void Deflate_MMX(unsigned char *dstp, const unsigned char *srcp,
//			const int dst_pitch, const int src_pitch,
//			const int row_size, const int height);
#endif