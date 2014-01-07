// FastFitPlane - fits a plane into another one of different dimensions
// using own resizers
// FastFitPlane (C) 2003 Kurosu (kurosu@inforezo.org)

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

#ifndef __FFP_FUNC___
#define __FFP_FUNC___

void FastReduceBy2_iSSE(unsigned char *dstp, const unsigned char *scrp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height);
void FastReduceBy2_MMX(unsigned char *dstp, const unsigned char *scrp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height);
void FastReduceBy2_C(unsigned char *dstp, const unsigned char *scrp, unsigned char *workp,
					 const int dst_pitch, const int src_pitch, const int work_pitch,
					 const int width, const int height);

void FastIncreaseBy2_MMX(unsigned char *dstp, const unsigned char *scrp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height);
void FastIncreaseBy2_C(unsigned char *dstp, const unsigned char *scrp, unsigned char *workp,
					 const int dst_pitch, const int src_pitch, const int work_pitch,
					 const int width, const int height);
#endif