// EdgeMask - builds the edge map of a video plane based on selectable
// differential operator
// EdgeMask (C) 2003 Kurosu (kurosu@inforezo.org)

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

#ifndef __EM_FUNC__
#define __EM_FUNC__

void Line_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int rowsize, const int height,
			  const unsigned char th1, const unsigned char th2);
void Line_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
		    const int src_pitch, const int rowsize, const int height,
		    const unsigned char th1, const unsigned char th2);
void Roberts_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int rowsize, const int height,
			  const unsigned char th1, const unsigned char th2);
void Roberts_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2);
//void Sobel_iSSE(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
//			  const int src_pitch, int rowsize, const int height,
//			  const unsigned char th1, const unsigned char th2);
void Sobel_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int rowsize, const int height,
			  const unsigned char th1, const unsigned char th2);
void Sobel_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			 const int src_pitch, const int rowsize, const int height,
			 const unsigned char th1, const unsigned char th2);
void Special_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			 const int src_pitch, const int rowsize, const int height,
			 const unsigned char th1, const unsigned char th2);
void Special_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2);
void Cartoon_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int rowsize, const int height,
			  const unsigned char th1, const unsigned char th2);
void Cartoon_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2);
void Laplace_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2);

#endif