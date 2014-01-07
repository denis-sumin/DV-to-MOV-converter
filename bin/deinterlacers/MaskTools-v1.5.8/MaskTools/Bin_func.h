// Binarize - Binarize (outputs either 0 or 255) an video plane
// Binarize (C) 2003 Kurosu (kurosu@inforezo.org)

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

#ifndef __BIN_FUNC__
#define __BIN_FUNC__

void BinUpper_MMX(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold);
void BinLower_MMX(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold);
void BinUpper_SSE(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold);
void BinLower_SSE(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold);
void BinUpper_C(unsigned char *pDst, const unsigned char *pSrc,
                int dstPitch, int srcPitch, int width, int height, int threshold);
void BinLower_C(unsigned char *pDst, const unsigned char *pSrc,
                int dstPitch, int srcPitch, int width, int height, int threshold);
#endif