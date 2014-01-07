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

#ifndef __YV12L_FUNC__
#define __YV12L_FUNC__

void LAdd_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LAdd_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);

/* Same as above but with chroma src2 = 128 */
void LAdd_NoCr_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level);
void LAdd_NoCr_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level);

/* Sub (in really invert of src2) with or without chroma of src2 considered */
void LSub_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LSub_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);
void LSub_NoCr_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LSub_NoCr_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);

/* Fast - a simple way to average 2 clips */
void LFast_C(unsigned char *src1p, const unsigned char *src2p,
			 const int src1_pitch, const int src2_pitch,
			 const int row_size, const int height, unsigned char level);
void LFast_MMX(unsigned char *src1p, const unsigned char *src2p,
			   const int src1_pitch, const int src2_pitch,
			   const int row_size, const int height, unsigned char level);

/* Multiply Luma */
void LMul_Y_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LMul_Y_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);

/* Multiply Chroma of src1, chroma of src 2 considered */
void LMul_C_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LMul_C_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);
/* Chroma of src2 NOT considered */
void LMul_C_NoCr_C(unsigned char *src1p, const unsigned char *src2p,
			const int src1_pitch, const int src2_pitch,
			const int row_size, const int height, unsigned char level);
void LMul_C_NoCr_MMX(unsigned char *src1p, const unsigned char *src2p,
			  const int src1_pitch, const int src2_pitch,
			  const int row_size, const int height, unsigned char level);

#endif __YV12L_FUNC__